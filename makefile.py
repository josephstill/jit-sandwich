#!/bin/python3

from re import S
import subprocess
import traceback

from pathlib  import Path
from os.path  import commonprefix
from argparse import ArgumentParser
from typing   import List, Tuple, Dict, Any


def file_check(source: Path, artifact: Path) -> bool:
    return True
    # if not artifact.exists():
    #     return True
    # return source.stat().st_mtime_ns > artifact.stat().st_mtime_ns

def file_check_many(sources: List[Path], artifact: Path):
    return True
    # if not artifact.exists():
    #     return True
    # for source in sources:
    #     if source.stat().st_mtime > artifact.stat().st_mtime:
    #         return True
    # return False
        

class DependencyError(Exception):

    def __init__(self, dependency: str, message: str) -> None:
        super().__init__(message)
        self._dependency = dependency

    def get_dependency(self) -> str: 
        return self._dependency

class BuildError(Exception):

    def __init__(self, build_command: str, compiler_stdout: str, compiler_stderr: str) -> None:
        super().__init__(compiler_stderr)
        self._build_cmd = build_command
        self._stdout = compiler_stdout
        self._stderr = compiler_stderr

    def __str__(self) -> str:
        ret = f'There was an error running this command:\n\t{self._build_cmd}\n'
        ret += f'\nStandard Out:\n\t{self._stdout}\n'
        ret += f'\nStandard Error:\n\t{self._stderr}\n'
        return ret

    def get_stdout(self) -> str:
        return self._stdout

class Build(object):

    def __init__(self, target: str, project_path: Path, debug: bool) -> None:
        self._target = target
        self._project_path = project_path.resolve()
        self._depenencies = ['Qt5Widgets', 'Qt5Core']
        self._test_depenencies = ['Qt5Test']
        self._initialized = False
        self._debug = debug

        target_include_dir = (self._project_path/'include'/'arch'/target).resolve() 
        target_source_dir = (self._project_path/'source'/'arch'/target).resolve()

        test_qrc = (self._project_path/'test'/'resources'/'test.qrc').resolve()

        self._include_dirs = self.collect_include_dirs((self._project_path/'include').resolve())
        self._include_dirs.append(target_include_dir)
        self._headers = []
        for include_dir in self._include_dirs:
            self._headers += self.collect_headers(include_dir)
        self._moc_pairs = self.generate_moc_pairs(self._headers)

        self._source = self.collect_source((self._project_path/'source').resolve())
        self._source += self.collect_source(target_source_dir)
        for moc_pair in self._moc_pairs:
            self._source.append(moc_pair[1])

        self._obj_pairs = self.generate_obj_pairs(self._source)
        self._bin_path = (self._project_path/'bin'/self.generate_bin_name(self._target)).resolve()

        self._tests = self.collect_tests((self._project_path/'test').resolve())
        self._test_moc_pairs = self.generate_test_moc_pairs(self._tests)
        self._test_bins = self.generate_test_bin_mappings(self._test_moc_pairs)
        self._test_rcc_pair = self.generate_test_res_pairs(test_qrc)

        self._agenda = []
        self._clean_dirs = [self._project_path/'moc', self._project_path/'obj', self._project_path/'bin']

        self._debug = False
        self._verbose = False

    def __str__(self) -> str:
        ret = f'Build Configuration:\n\tTarget: {self._bin_path}\n\tCompiler: {str(self.get_compiler())}\n\tQt Meta Object Compiler: {str(self.get_moc())}\n'
        ret += f'\nCompiler Info:\n\t{self.get_compiler_version()}\nQt Meta Object Compiler Info:\n\t{self.get_moc_version()}\n'
        ret += '\nInclude Directories: \n'
        for inc_dir in self._include_dirs:
            ret += f'\t{str(inc_dir)}\n'
        ret += '\nSource Files: \n'
        for src_file in self._source:
            ret += f'\t{str(src_file)}\n'
        ret += '\nTest Sources: \n'
        for test_src in self._tests:
            ret += f'\t{str(test_src)}\n'
        ret += '\nQt MOC Artifacts: \n'
        for moc_pair in self._moc_pairs:
            ret += f'\t{str(moc_pair[0])} -> {str(moc_pair[1])}\n'
        ret += '\nBuild Artifacts: \n'
        for obj_pair in self._obj_pairs:
            ret += f'\t{str(obj_pair[0])} -> {str(obj_pair[1])}\n' 
        ret += '\nTest Artifacts: \n'
        for test_pair in self._test_moc_pairs:
            ret += f'\t{str(test_pair[0])} -> {str(test_pair[1])}\n'
        ret += '\nTest Binaries: \n'
        for test in self._test_bins.keys():
            ret += f'{str(test)}\n'

        if not self._agenda:
            raise NotImplementedError('No build procedure has been constructed')

        ret += '\nBuild Procedure:'
        for cmd in self._agenda:
            ret += '\n' + ' '.join(cmd) + '\n'
        return ret

    def collect_include_dirs(self, directory: Path) -> List[Path]:
        ret = []
        for item in directory.iterdir():
            if item.is_dir() and item.name != 'arch':
                ret += self.collect_include_dirs(item.resolve())
            if item.is_file() and item.suffix == '.h' and directory not in ret:
                ret.append(directory)
        return ret

    def collect_headers(self, directory: Path) -> List[Path]:
        ret = []
        for item in directory.iterdir():
            if item.is_file() and item.suffix == '.h':
                ret.append(item.resolve())
        return ret

    def collect_source(self, directory: Path) -> List[Path]:
        ret = []
        for item in directory.iterdir():
            if item.is_dir() and item.name != 'arch':
                ret += self.collect_source(item)
            elif item.is_file():
                if item.suffix != ".inc":
                   ret.append(item)
        return ret

    def collect_tests(self, directory: Path) -> List[Path]:
        ret = []
        for item in directory.iterdir():
            if item.is_dir() and item.name != 'moc' and item.name != 'resources':
                ret += self.collect_tests(item)
            elif item.is_file():
                ret.append(item)
        return ret

    def generate_moc_pairs(self, headers: List[Path]) -> List[Tuple[Path, Path]]:
        ret = []
        header_common = commonprefix(headers)
        moc_base = self._project_path.resolve()/'moc'
        for header in headers:
            with open(header, 'r') as header_fp:
                for line in header_fp.readlines():
                    if line.strip() == 'Q_OBJECT':
                        header_relative = header.relative_to(header_common)
                        moc_file = moc_base/header_relative.parent/f'moc_{header_relative.stem}.cxx'
                        ret.append((header, moc_file.resolve()))
                        break
                header_fp.close()
        return ret

    def generate_obj_pairs(self, source: List[Path]) -> List[Tuple[Path, Path]]:
        ret = []
        source_common = commonprefix(source)
        obj_base = self._project_path.resolve()/'obj'
        for source_file in source:
            source_relative = source_file.relative_to(source_common)
            obj_file = obj_base/source_relative.parent/f'{source_relative.stem}.o'
            ret.append((source_file, obj_file.resolve())) 
        return ret

    def generate_test_moc_pairs(self, tests: List[Path]) -> List[Tuple[Path, Path]]:
        ret = []
        for test in tests:
            test_moc_dir = test.parent/'moc'
            test_moc_file = test_moc_dir/f'moc_{test.stem}.cxx'
            ret.append((test, test_moc_file.resolve()))
        return ret

    def generate_test_res_pairs(self, qrc_file: Path) -> Tuple[Path, Path]:
        return (qrc_file, qrc_file.with_suffix('.rcc'))

    def generate_test_bin_mappings(self, test_sets: List[Tuple[Path, Path]]) -> Dict:
        ret = {}
        for pair in test_sets:
            test_bin_path = self._project_path.resolve()/'bin'/'tests'/pair[0].stem
            ret[test_bin_path] = pair
        return ret
            
    def generate_bin_name(self, target: str) -> str:
        return f'jit-sandwich-{target}'    

    def clean(self):
        for dir in self._clean_dirs:
            self.clean_dir(dir)
        if self._test_rcc_pair[1].exists():
            self._test_rcc_pair[1].unlink()
        for test_set in self._test_moc_pairs:
            if test_set[1].parent.exists():
                self.clean_dir(test_set[1].parent)

    def clean_dir(self, directory: Path) -> None:
        if directory.exists():
            for item in directory.iterdir():
                if item.is_dir():
                    self.clean_dir(item)
                else:
                    item.unlink()
            directory.rmdir()

    def get_compiler_version(self) -> str:
        raise NotImplementedError('No compiler recognized')

    def get_moc_version(self) -> str:
        raise NotImplementedError('No moc recognized')

    def get_compiler(self) -> Path:
        raise NotImplementedError('No compiler recognized')

    def get_moc(self) -> Path:
        raise NotImplementedError('No moc recognized')

    def get_rcc(self) -> Path:
        raise NotImplementedError('No rcc recognized')

    def build(self) -> None:
        if not self._agenda:
            raise NotImplementedError('No build procedure defined')
        for cmd in self._agenda:
            if self._verbose:
                print(' '.join(cmd))
            result = subprocess.run(cmd, capture_output=True, text=True)
            if result.stderr:
                raise BuildError(' '.join(cmd), result.stdout, result.stderr)
            if result.stdout and self._verbose:
                print(result.stdout)
        if self._verbose:
            print(f'Build complete: {self._bin_path}')

    def execute_tests(self) -> None:
        print('Executing Unit Tests')
        if not self._agenda:
            raise NotImplementedError('No build procedure defined')
        for test in self._test_bins.keys():
            if not test.exists():
                raise FileNotFoundError(f'{str(test)} does not exist')
            result = subprocess.run([str(test)], capture_output=True, text=True)
            print(f'{result.stdout}')
        
class LinuxBuild(Build):

    def __init__(self, target: str, project_path: Path, debug: bool):
        super().__init__(target, project_path, debug)

        self._i_flags   = []
        self._l_flags   = []
        self._cxx_flags = []
        self._defines   = []
        self._test_defines = []
        self._test_i_flags = []
        self._test_l_flags = []
        for include_dir in self._include_dirs:
            self._i_flags.append(f'-I{include_dir}')
        for dependency in self._depenencies:
            self._i_flags += self.collect_dependency_includes(dependency)
            self._l_flags += self.collect_dependency_libraries(dependency)
        for dependency in self._test_depenencies:
            self._test_i_flags += self.collect_dependency_includes(dependency)
            self._test_l_flags += self.collect_dependency_libraries(dependency)

        self._test_defines.append(f'-DRESOURCE_PATH="{str(self._test_rcc_pair[1])}"')

        if debug:
            self._cxx_flags.append('-g3')

        for cmd in self.gen_moc_cmds():
            if len(cmd) > 0:
                self._agenda.append(cmd)
        for cmd in self.gen_object_cmds():
            if len(cmd) > 0:
                self._agenda.append(cmd)
        for cmd in self.gen_build_cmds():
            if len(cmd) > 0:
                self._agenda.append(cmd)
        for cmd in self.gen_test_moc_cmds():
            if len(cmd) > 0:
                self._agenda.append(cmd)
        for cmd in self.gen_test_rcc():
            if len(cmd) > 0:
                self._agenda.append(cmd)
        for cmd in self.gen_test_cmds():
            if len(cmd) > 0:
                self._agenda.append(cmd)
        
    def collect_dependency_includes(self, dependency: str) -> List[str]:
        result = subprocess.run(['pkg-config', '--cflags', dependency], capture_output=True, text=True)
        if result.stderr:
            raise DependencyError(dependency, result.stderr)
        elif result.stdout:
            return result.stdout.split()
        else:
            raise DependencyError(dependency, f'Error with locating includes for {dependency}')
    
    def collect_dependency_libraries(self, dependency: str) -> List[str]:
        result = subprocess.run(['pkg-config', '--libs', dependency], capture_output=True, text=True)
        if result.stderr:
            raise DependencyError(dependency, result.stderr)
        elif result.stdout:
            return result.stdout.split()
        else:
            raise DependencyError(dependency, f'Error with locating includes for {dependency}')

    def get_moc(self) -> Path:
        return Path('/usr/bin/moc-qt5')

    def get_rcc(self) -> Path:
        return Path('/usr/bin/rcc-qt5')

    def get_moc_version(self) -> str:
        result = subprocess.run([str(self.get_moc()), '-v'], capture_output=True, text=True)
        if result.stdout:
            return result.stdout
        raise DependencyError(str(self.get_moc()), result.stderr)

    def get_compiler(self) -> Path:
        return Path('/usr/bin/g++')
    
    def get_compiler_version(self) -> str:
        result = subprocess.run([str(self.get_compiler()), '--version'], capture_output=True, text=True)
        if result.stdout:
            return result.stdout
        raise DependencyError(str(self.get_compiler()), result.stderr)

    def gen_moc_cmds(self) -> List[List[str]]:
        cmds = []
        for moc_pair in self._moc_pairs:
            if file_check(moc_pair[0], moc_pair[1]):
                if not moc_pair[1].parent.exists():
                    moc_pair[1].parent.mkdir(parents=True)
                cmd = [str(self.get_moc())]
                cmd += self._defines
                cmd += self._i_flags
                cmd += ['-o', str(moc_pair[1]), str(moc_pair[0])]
                cmds.append(cmd)
        return cmds
    
    def gen_object_cmds(self) -> List[List[str]]:
        cmds = []
        for obj_pair in self._obj_pairs:
            if file_check(obj_pair[0], obj_pair[1]):
                if not obj_pair[1].parent.exists():
                    obj_pair[1].parent.mkdir(parents=True) 
                cmd = [str(self.get_compiler())]
                cmd += self._cxx_flags
                cmd += self._i_flags
                cmd += self._defines
                cmd += ['-o', str(obj_pair[1])]
                cmd += ['-c', str(obj_pair[0])]
                cmds.append(cmd)
        return cmds
    
    def gen_build_cmds(self) -> List[List[str]]:
        build_cmd = []
        obj_files = []
        for obj_pair in self._obj_pairs:
            obj_files.append(obj_pair[0])

        if file_check_many(obj_files, self._bin_path) or file_check(Path('main.cpp'), self._bin_path):
            build_cmd += [str(self.get_compiler())]
            build_cmd += self._cxx_flags
            build_cmd += self._i_flags
            build_cmd += self._defines
            build_cmd += [str(obj_file) for obj_file in obj_files]

            if not self._bin_path.parent.exists():
                self._bin_path.parent.mkdir(parents=True)
            build_cmd.append('main.cpp')
            build_cmd += ['-o', str(self._bin_path)]
            build_cmd += self._l_flags 
        return [build_cmd]

    def gen_test_moc_cmds(self) -> List[List[str]]:
        cmds = []
        for test in self._test_moc_pairs:
            if file_check(test[0], test[1]):
                if not test[1].parent.exists():
                    test[1].parent.mkdir(parents=True) 
                cmd = [str(self.get_moc())]
                cmd += self._defines
                cmd += self._i_flags
                cmd += ['-o', str(test[1]), str(test[0])]
                cmds.append(cmd)
        return cmds

    def gen_test_rcc(self) -> List[List[str]]:
        ret = []
        if file_check(self._test_rcc_pair[1], self._test_rcc_pair[0]):
            ret.append([str(self.get_rcc()), '-binary',  str(self._test_rcc_pair[0]), '-o', str(self._test_rcc_pair[1])])
        return ret

    def gen_test_cmds(self) -> List[List[str]]:
        cmds = []
        obj_files = []
        for obj_pair in self._obj_pairs:
            obj_files.append(obj_pair[0])

        build_cmd_base = [str(self.get_compiler())]
        build_cmd_base += self._cxx_flags
        build_cmd_base += self._i_flags
        build_cmd_base += self._test_i_flags
        build_cmd_base += self._defines
        build_cmd_base += self._test_defines
        build_cmd_base += [str(obj_file) for obj_file in obj_files]

        for test in self._test_bins.keys():
            if file_check_many(obj_files, test) or file_check(self._test_bins[test][0], test):
                if not test.parent.exists():
                    test.parent.mkdir(parents=True)
                test_buid_cmd = build_cmd_base.copy()
                test_buid_cmd.append(str(self._test_bins[test][0]))
                test_buid_cmd += ['-o', str(test)]
                test_buid_cmd += self._l_flags
                test_buid_cmd += self._test_l_flags
                cmds.append(test_buid_cmd)            
        return cmds

    def gen_test_build_cmds(self) -> List[List[str]]:
        pass

if __name__ == '__main__':
   
    parser = ArgumentParser(description='Build script for this awesome emulator')
    parser.add_argument('--verbose', '-v', action='store_true', help='Set the verbosity')
    parser.add_argument('--debug', '-d', action='store_true', help='Enables a debug build')
    parser.add_argument('--pretend', '-p', action='store_true', help='Displays setup and commands for the build')
    parser.add_argument('--clean', '-c', action='store_true', help='Cleans the build environment')
    parser.add_argument('--skip-test', '-s', action='store_true', help='Omits the unit test phase of the build (Requires a previous build)')
    parser.add_argument('--test', '-t', action='store_true', help='Runs only the unit test phase of the build')
    args = parser.parse_args()
       
    try:
        build = LinuxBuild('chip8', Path(__file__).resolve().parent, args.debug)
        if args.pretend:
            print(build)
        elif args.clean:
            build.clean()
        elif args.test:
            build.execute_tests()
        else:
            build.build()
            if not args.skip_test:
                build.execute_tests()
    except Exception as e:
        print(f'Error: {e}\n')
        print(traceback.format_exc())
    
   