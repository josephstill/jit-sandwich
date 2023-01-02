#include <QtTest>
#include <QResource>

#include "Rom.h"

class RomTest: public QObject 
{
    Q_OBJECT

private slots:

    void initTestCase() 
    {
        QResource::registerResource(RESOURCE_PATH);
    }

    void testExecuteFlag() 
    {
        Rom rom(":/bins/test.bin");
        QVERIFY(!rom.isExecute());

        rom.enableExecute();
        QVERIFY(rom.isExecute());

        rom.disableExecute();
        QVERIFY(!rom.isExecute());
    }

    void testReadFlag() 
    {
        Rom rom(":/bins/test.bin");
        QVERIFY(rom.isRead());
        
    }

    void testWriteFlag() 
    {
        Rom rom(":/bins/test.bin");
        QVERIFY(!rom.isWrite());

    }

    void testSize() 
    {
        Rom rom(":/bins/test.bin");
        QCOMPARE(rom.size(), 1024);
    }

    void testInit() 
    {
        Rom rom(":/bins/test.bin");
        rom.init();   

        uint8_t readBuffer[1024];
        ::memset(readBuffer, 0xff, 1024);
        QVERIFY(rom.performRead(0, readBuffer, 1024));
        for (int x = 0; x < 1024; ++x)
        {
            QCOMPARE(readBuffer[x], 0xab);
        }
    }
    
    void testReset() 
    {
        Rom rom(":/bins/test.bin");
        rom.init();   

        uint8_t readBuffer[1024];
        ::memset(readBuffer, 0xff, 1024);
        QVERIFY(rom.performRead(0, readBuffer, 1024));
        for (int x = 0; x < 1024; ++x)
        {
            QCOMPARE(readBuffer[x], 0xab);
        }
        
        rom.reset();
        ::memset(readBuffer, 0xff, 1024);
        QVERIFY(rom.performRead(0, readBuffer, 1024));
        for (int x = 0; x < 1024; ++x)
        {
            QCOMPARE(readBuffer[x], 0xab);
        }
    }
    
    void testPerformRead() 
    {
        Rom rom(":/bins/test.bin");
        rom.init();      
 
        uint8_t readBuffer[2048];
        ::memset(readBuffer, 0xff, 2048);

        QVERIFY(!rom.performRead(0, readBuffer, 2048));
        QVERIFY(!rom.performRead(512, readBuffer, 1024));
        QVERIFY(!rom.performRead(1022, readBuffer, 100));
        for (int x = 0; x < 2048; ++x)
        {
            QCOMPARE(readBuffer[x], 0xff);
        }

        ::memset(readBuffer, 0xff, 2048);
        QVERIFY(rom.performRead(0, readBuffer, 256));
        for (int x; x < 2048; ++x)
        {
            QCOMPARE(readBuffer[x], x < 256 ? 0xab : 0xff);
        }
    }
    
    void testPerformWrite() 
    {
        Rom rom(":/bins/test.bin");
        rom.init(); 

        uint8_t writeBuffer[1024];
        ::memset(writeBuffer, 0x00, 1024); 
        uint8_t readBuffer[1024];
        ::memset(readBuffer, 0xff, 1024);

        QVERIFY(!rom.performWrite(0, writeBuffer, 1024));
        QVERIFY(rom.performRead(0, readBuffer, 1024));
        for (int x = 0; x < 1024; ++x)
        {
            QCOMPARE(readBuffer[x], 0xab);
        }        
    }
    
    void cleanupTestCase() {}
};

QTEST_MAIN(RomTest)
#include "moc/moc_RomTest.cxx"