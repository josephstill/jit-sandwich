# JIT-Sandwich  
It is my hope that this project will some day be a full system emulator. I am currently in the early stages of development; so, I chose a scholastic architecture in order to prove out the JIT compiler. That architecture is CHIP-8 due to its reduced instruction set, simplicity, and my familiarity with it.  
  
## Translation Instructions
Translation instructions are part of a kind of intermediate language that is constructed programmatically then encoded with host equivalent behavior. The following table explains the syntax and pseudocode in the instruction table. 
  
| Character | Meaning            |
|:---------:|:------------------:|
|`$`        | Register Contents  |
|`#`        | Immediate Value    |
|`*`        | Dereference        |

The following table lists the instructions with syntax and pseudocode.
  
| Instruction | Name                               | Syntax              | Pseudocode          | Notes                  |
|-------------|:----------------------------------:|:-------------------:|:-------------------:|:----------------------:|
|NOOP         | No Operation                       | `NOOP`              | ` `                 | Does Nothing           |
|MOV          | Move                               | `MOV $RD $RS`       | `$RD = $RS`         |                        |
|MOVI         | Move Immediate                     | `MOV $RD #IMM`      | `$RD = #IMM`        |                        |
|LD           | Load                               | `LD $RD $RS`        | `$RD = *$RS`        |                        |
|ST           | Store                              | `ST $RD $RS`        | `*$RD = $RS`        |                        |
|STI          | Store Immediate                    | `STI $RD #IMM`      | `*$RD = #IMM`       |                        |
|AND          | And                                | | | | 
|ANDI         | And Immediate                      | | | |
|OR           | Or                                 | | | |
|ORI          | Or Immediate                       | | | |
|XOR          | Exclusive Or                       | | | |
|XORI         | Exclusive Or Immediate             | | | |
|SHL          | Shift Left                         | | | |
|SHLI         | Shift Left Immediate               | | | |
|SHR          | Shift Right                        | | | |
|SHRI         | Shift Right Immediate              | | | |
|NOT          | Not                                | | | |
|ADD          | Add                                | `ADD $RD $RS1 $RS2` | `$RD = $RS1 + $RS2` | Maintains a carry flag |
|ADDI         | Add Immediate                      | `ADDI $RD $RS #IMM` | `$RD = $RS + #IMM`  | Maintains a carry flag |
|SUB          | Subtract                           | | | |  
|SUBI         | Subtract Immediate                 | | | |
|RSUBI        | Reverse Subtract Immediate         | | | |
|MULT         | Multiply                           | | | |
|MULTI        | Multiply Immediate                 | | | |
|DIV          | Divide                             | | | |
|DIVI         | Divide Immediate                   | | | |
|RDIVI        | Reverse Divide Immediate           | | | |
|LT           | Less Than                          | | | |
|LTI          | Less Than Immediate                | | | |
|GT           | Greater Than                       | | | |
|GTI          | Greater Than Immediate             | | | |
|LTE          | Less Than or Equal To              | | | |
|LTEI         | Less Than or Equal To Immediate    | | | |
|GTE          | Greater Than or Equal To           | | | |
|GTEI         | Greater Than or Equal To Immediate | | | |
|EQ           | Equals                             | | | |
|EQI          | Equals Immediate                   | | | |
|NE           | Not Equals                         | | | |
|NEI          | Not Equals Immediate               | | | |
|BLT          | Branch Less Than                   | | | |
|BLTI         | Branch Less Than Immediate         | | | |
|BGT          | Branch Greater Than                | | | |
|BGTI         | Branch Greater Than Immediate      | | | |
|BEQ          | Branch Equal                       | | | |
|BEQI         | Branch Equal Immediate             | | | |
|BNE          | Branch Not Equal                   | | | |
|BNEI         | Branch Not Equal Immediate         | | | |
|CF           | Carry Flag                         | | | |
|OF           | Overflow Flag                      | | | |
 

