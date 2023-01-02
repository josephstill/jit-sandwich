#include <QtTest>

#include "Ram.h"

#include <cstring>

class RamTest: public QObject 
{
    Q_OBJECT

private slots:

    void initTestCase() 
    {

    }
    
    void testExecuteFlag() 
    {
        Ram ram(1024);
        ram.enableExecute();
        QVERIFY(ram.isExecute());
        
        ram.disableExecute();
        QVERIFY(!ram.isExecute());
    }

    void testReadFlag() 
    {
        Ram ram(1024);
        ram.enableRead();
        QVERIFY(ram.isRead());
        
        ram.disableRead();
        QVERIFY(!ram.isRead());
    }

    void testWriteFlag() 
    {
        Ram ram(1024);
        ram.enableWrite();
        QVERIFY(ram.isWrite());
        
        ram.disableWrite();
        QVERIFY(!ram.isWrite());        
    }

    void testSize() 
    {
        Ram ram(1024);
        QCOMPARE(ram.size(), 1024);
    }

    void testInit() 
    {
        Ram ram(1024);
        ram.enableRead();
        ram.init();
        uint8_t readBuffer[1024];
        ::memset(readBuffer, 0xff, 1024);
        QVERIFY(ram.performRead(0, readBuffer, 1024));
        for (int index = 0; index < 1024; ++index)
        {
            QCOMPARE(readBuffer[index], 0);
        }        
    }
    
    void testReset() 
    {
        Ram ram(1024);
        ram.enableRead();
        ram.enableWrite();
        
        ram.init();
        uint8_t writeBuff[1024];
        ::memset(writeBuff, 0xab, 1024);
        ram.performWrite(0, writeBuff, 1024);

        uint8_t writeVerifyBuffer[1024];
        ::memset(writeVerifyBuffer, 0xff, 1024);
        QVERIFY(ram.performRead(0, writeVerifyBuffer, 1024));        
        for (int index = 0; index < 1024; ++index)
        {
            QCOMPARE(writeVerifyBuffer[index], 0xab);
        }  

        ram.reset();
        uint8_t readBuffer[1024];
        ::memset(readBuffer, 0xff, 1024);
        QVERIFY(ram.performRead(0, readBuffer, 1024));
        for (int index = 0; index < 1024; ++index)
        {
            QCOMPARE(readBuffer[index], 0);
        }   

        ::memset(readBuffer, 0xff, 1024);
        ram.performWrite(0, writeBuff, 1024);
        ram.disableWrite();
        ram.reset(); 
        QVERIFY(ram.performRead(0, readBuffer, 1024));
        for (int index = 0; index < 1024; ++index)
        {
            QCOMPARE(readBuffer[index], 0);
        }                   
    }

    void testPerformRead() 
    {
        Ram ram(1024);
        ram.enableRead();
        ram.enableWrite();
        
        ram.init();
        uint8_t readBuffer[2048];
        ::memset(readBuffer, 0xff, 2048);
        QVERIFY(ram.performRead(0, readBuffer, 1024));
        for (int index = 0; index < 2048; ++index)
        {
            QCOMPARE(readBuffer[index], index < 1024 ? 0 : 0xff);
        }  

        uint8_t writeBuff[1024];
        ::memset(writeBuff, 0xab, 1024);
        ram.performWrite(0, writeBuff, 1024);

        ram.disableRead();
        ::memset(readBuffer, 0xff, 2048);
        QVERIFY(!ram.performRead(0, readBuffer, 1024));
        for (int index = 0; index < 2048; ++index)
        {
            QCOMPARE(readBuffer[index], 0xff);
        }  

        ram.enableRead();
        QVERIFY(!ram.performRead(0, readBuffer, 2048));
        QVERIFY(!ram.performRead(512, readBuffer, 1024));
        QVERIFY(!ram.performRead(1022, readBuffer, 88));
        QVERIFY(!ram.performRead(1056, readBuffer, 12));
        for (int index = 0; index < 2048; ++index)
        {
            QCOMPARE(readBuffer[index], 0xff);
        }  

        QVERIFY(ram.performRead(256, readBuffer, 512));
        for (int index = 0; index < 2048; ++index)
        {
            QCOMPARE(readBuffer[index], index < 512 ? 0xab : 0xff);
        }     
    }
    
    void testPerformWrite() 
    {
        Ram ram(1024);
        ram.enableRead();
        ram.enableWrite();
        
        ram.init();
        uint8_t readBuffer[1024];
        ::memset(readBuffer, 0xff, 1024);
        QVERIFY(ram.performRead(0, readBuffer, 1024));
        for (int index = 0; index < 1024; ++index)
        {
            QCOMPARE(readBuffer[index], 0);
        }  

        uint8_t writeBuff[2048];
        ::memset(writeBuff, 0xab, 2048);
        QVERIFY(ram.performWrite(0, writeBuff, 1024));   

        ::memset(readBuffer, 0xff, 1024);
        QVERIFY(ram.performRead(0, readBuffer, 1024));
        for (int index = 0; index < 1024; ++index)
        {
            QCOMPARE(readBuffer[index], 0xab);
        }  
        ram.reset();
        ram.disableWrite();

        ::memset(readBuffer, 0xff, 1024);
        QVERIFY(!ram.performWrite(0, writeBuff, 1024));
        QVERIFY(ram.performRead(0, readBuffer, 1024));
        for (int index = 0; index < 1024; ++index)
        {
            QCOMPARE(readBuffer[index], 0x00);
        }          

        ::memset(readBuffer, 0xff, 1024);
        ram.enableWrite();
        QVERIFY(!ram.performWrite(0, writeBuff, 2048));
        QVERIFY(!ram.performWrite(32, writeBuff, 1024));
        QVERIFY(!ram.performWrite(1000, writeBuff, 256));
        QVERIFY(!ram.performWrite(1248, writeBuff, 16));
        QVERIFY(ram.performRead(0, readBuffer, 1024));
        for (int index = 0; index < 1024; ++index)
        {
            QCOMPARE(readBuffer[index], 0x00);
        }  
    }
    
    void cleanupTestCase() {}
};

QTEST_MAIN(RamTest)
#include "moc/moc_RamTest.cxx"