#include <QtTest>

#include "Register.h"

class RegisterTest: public QObject
{
    Q_OBJECT

private slots:

    void initTestCase() {}

    void testBuffer() 
    {
        uint32_t test = 0;
        uint32_t contents = 0x12345678;
        Register reg(sizeof(contents));
        ::memcpy(reg.buffer(), (void *) &contents, sizeof(contents));
        test = *((uint32_t *) reg.buffer());
        QCOMPARE(contents, test);
    }

    void testAlias()
    {
        uint32_t test1 = 0x12345678;
        uint32_t read1 = 0xffffffff;
        uint16_t read2 = 0xffff;
        uint8_t  read3 = 0xff;
        QSharedPointer<Register> reg1 = QSharedPointer<Register>(new Register(sizeof(read1)));
        reg1->init();
        QSharedPointer<Register> reg2 = reg1->alias(sizeof(read2), 0);
        QVERIFY(reg1->size() > reg2->size());

        *((uint32_t *) reg1->buffer()) = test1;
        read1 = *((uint32_t *) reg1->buffer());
        read2 = *((uint16_t *) reg2->buffer());
        QCOMPARE(read1 & 0xffff, read2);

        QVERIFY(read2 != 0);
        reg1->reset();
        read2 = *((uint16_t *) reg2->buffer());  
        QCOMPARE(read2, 0);

        read1 = *((uint32_t *) reg1->buffer());
        QCOMPARE(read1, 0);
        QSharedPointer<Register> reg3 = reg2->alias(sizeof(read3), sizeof(read3)); 
        *((uint8_t *) reg3->buffer()) = 0x12;
        read1 = *((uint32_t *) reg1->buffer());
        QCOMPARE(read1, 0x00001200);
    }

    void testDestruction()
    {
        QSharedPointer<Register> sub_0 = QSharedPointer<Register>(new Register(sizeof(uint32_t)));
        QSharedPointer<Register> sub_1 = sub_0->alias(sizeof(uint16_t), 0);
        QSharedPointer<Register> sub_2 = sub_0->alias(sizeof(uint16_t), sizeof(uint16_t));
        QSharedPointer<Register> sub_3 = sub_1->alias(sizeof(uint8_t), 0);
        QSharedPointer<Register> sub_4 = sub_2->alias(sizeof(uint8_t), 0);
        QSharedPointer<Register> sub_5 = sub_2->alias(sizeof(uint8_t), sizeof(uint8_t));

        uint32_t write_val = 0x01234567;
        sub_0->write((uint8_t *) &write_val, sizeof(write_val));
        sub_1.reset();
        write_val = 0xfedca987;
        sub_0->write((uint8_t *) &write_val, sizeof(write_val));
        
        uint8_t read_val = 0;
        sub_3->read(&read_val, sizeof(read_val));
        QCOMPARE(read_val, 0x87);

        sub_0.reset();
        read_val = 0;
        sub_3->read(&read_val, sizeof(read_val));
        QCOMPARE(read_val, 0x87);

        sub_2.reset();
        uint8_t write_2 = 0xff;
        sub_4->write(&write_2, sizeof(write_2));
        sub_5->write(&write_2, sizeof(write_2));
    }

    void testRead()
    {
        uint32_t test1 = 0xffffffff;
        Register reg(sizeof(test1));
        reg.init();
        reg.read((uint8_t *) &test1, sizeof(test1));
        QCOMPARE(test1, 0);

        uint64_t test2 = 0xffffffffffffffff;
        reg.read((uint8_t *) &test2, sizeof(test2));
        QCOMPARE(test2, 0xffffffff00000000);

        uint16_t test3 = 0xffff;
        uint32_t newValue = 0x89abcdef;
        ::memcpy(reg.buffer(), (void *) &newValue, sizeof(newValue));
        reg.read((uint8_t *) &test3, sizeof(test3));
        QCOMPARE(test3, 0xcdef);
    }

    void testReset() 
    {
        uint32_t test = 0;
        uint32_t contents = 0x12345678;
        Register reg(sizeof(contents));
        ::memcpy(reg.buffer(), (void *) &contents, sizeof(contents));
        test = *((uint32_t *) reg.buffer());
        QCOMPARE(contents, test);

        reg.reset();
        test = *((uint32_t *) reg.buffer());
        QCOMPARE(test, 0);
    }

    void testSize() 
    {
        Register regA(4);
        QCOMPARE(regA.size(), 4);
        Register regB(2);
        QCOMPARE(regB.size(), 2);
        Register regC(4);
        QCOMPARE(regC.size(), 4);
        QCOMPARE(regA.size(), regC.size());
    }
    
    void testWrite()
    {
        uint32_t read = 0;
        uint32_t test1 = 0xffffffff;
        Register reg(sizeof(test1));
        reg.init();
        reg.write((uint8_t *) &test1, sizeof(test1));
        read = *((uint32_t *) reg.buffer());
        QCOMPARE(test1, read);

        uint64_t test2 = 0x0123456789abcdef;
        reg.write((uint8_t *) &test2, sizeof(test2));
        read = *((uint32_t *) reg.buffer());
        QCOMPARE(test2 & 0x00000000ffffffff, read);

        uint16_t test3 = 0x1212;
        reg.write((uint8_t *) &test3, sizeof(test3));
        read = *((uint32_t *) reg.buffer());
        QCOMPARE(test3, read);
    }

    void cleanupTestCase() {}
};

QTEST_MAIN(RegisterTest)
#include "moc/moc_RegisterTest.cxx"