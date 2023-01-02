#include <QtTest>

#include "RegisterFile.h"

class RegisterFileTest: public QObject
{
    Q_OBJECT

private slots:

    void initTestCase() {}

    void testAddRegister() 
    {
        RegisterFile regs;
        QSharedPointer<Register> r1 = regs.addRegister("R1", sizeof(uint32_t));
        QSharedPointer<Register> r2 = regs.addRegister("R2", sizeof(uint32_t));
        QSharedPointer<Register> r3 = regs.addRegister("R3", sizeof(uint32_t));
        QSharedPointer<Register> l1 = regs.addRegister("L1", "R1", sizeof(uint8_t), 0);

        uint32_t write = 0x11111111;
        uint32_t read = 0;
        r1->write((uint8_t *) &write, sizeof(write));
        regs.getRegister("R1")->read((uint8_t *) &read, sizeof(read));
        QCOMPARE(write, read);
        
        write = 0x22222222;
        read = 0;
        r2->write((uint8_t *) &write, sizeof(write));
        regs.getRegister("R2")->read((uint8_t *) &read, sizeof(read));
        QCOMPARE(write, read);
        
        write = 0x33333333;
        read = 0;
        r3->write((uint8_t *) &write, sizeof(write));
        regs.getRegister("R3")->read((uint8_t *) &read, sizeof(read));
        QCOMPARE(write, read);

        uint8_t read_2 = 0;
        regs.getRegister("L1")->read(&read_2, sizeof(read_2));
        QCOMPARE(read_2, 0x11);
    }
    
    void testGetRegister() 
    {
        RegisterFile regs;
        regs.addRegister("R1", sizeof(uint32_t));
        regs.addRegister("R2", sizeof(uint32_t));
        regs.addRegister("R3", sizeof(uint32_t));

        uint32_t write = 0x11111111;
        uint32_t read = 0;
        regs.getRegister("R1")->write((uint8_t *) &write, sizeof(write));
        regs.getRegister("R1")->read((uint8_t *) &read, sizeof(read));
        QCOMPARE(write, read);
        
        write = 0x22222222;
        read = 0;
        regs.getRegister("R2")->write((uint8_t *) &write, sizeof(write));
        regs.getRegister("R2")->read((uint8_t *) &read, sizeof(read));
        QCOMPARE(write, read);
        
        write = 0x33333333;
        read = 0;
        regs.getRegister("R3")->write((uint8_t *) &write, sizeof(write));
        regs.getRegister("R3")->read((uint8_t *) &read, sizeof(read));
        QCOMPARE(write, read);
    }
    
    void testInit()
    {
        RegisterFile regs;
        QSharedPointer<Register> r1 = regs.addRegister("R1", sizeof(uint32_t));
        QSharedPointer<Register> r2 = regs.addRegister("R2", sizeof(uint32_t));
        QSharedPointer<Register> r3 = regs.addRegister("R3", sizeof(uint32_t));
        regs.init();

        regs.reset();
        uint32_t read = 0xffffffff;
        regs.getRegister("R1")->read((uint8_t *) &read, sizeof(read));
        QCOMPARE(read, 0);
        read = 0xffffffff;
        regs.getRegister("R2")->read((uint8_t *) &read, sizeof(read));
        QCOMPARE(read, 0);
        read = 0xffffffff;
        regs.getRegister("R3")->read((uint8_t *) &read, sizeof(read));
        QCOMPARE(read, 0);                
    }

    void testReset() 
    {
        RegisterFile regs;
        regs.addRegister("R1", sizeof(uint32_t));
        regs.addRegister("R2", sizeof(uint32_t));
        regs.addRegister("R3", sizeof(uint32_t));

        uint32_t write = 0x11111111;
        uint32_t read = 0;
        regs.getRegister("R1")->write((uint8_t *) &write, sizeof(write));
        regs.getRegister("R1")->read((uint8_t *) &read, sizeof(read));
        QCOMPARE(write, read);
        
        write = 0x22222222;
        read = 0;
        regs.getRegister("R2")->write((uint8_t *) &write, sizeof(write));
        regs.getRegister("R2")->read((uint8_t *) &read, sizeof(read));
        QCOMPARE(write, read);
        
        write = 0x33333333;
        read = 0;
        regs.getRegister("R3")->write((uint8_t *) &write, sizeof(write));
        regs.getRegister("R3")->read((uint8_t *) &read, sizeof(read));
        QCOMPARE(write, read);

        regs.reset();
        read = 0xffffffff;
        regs.getRegister("R1")->read((uint8_t *) &read, sizeof(read));
        QCOMPARE(read, 0);
        read = 0xffffffff;
        regs.getRegister("R2")->read((uint8_t *) &read, sizeof(read));
        QCOMPARE(read, 0);
        read = 0xffffffff;
        regs.getRegister("R3")->read((uint8_t *) &read, sizeof(read));
        QCOMPARE(read, 0);
    }
    
    void cleanupTestCase() {}
};

QTEST_MAIN(RegisterFileTest)
#include "moc/moc_RegisterFileTest.cxx"

