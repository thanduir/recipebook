#include <QtTest/QtTest>
#include "RBUnitTests.h"

int main(int argc, char *argv[])
{
    TESTLIB_SELFCOVERAGE_START(TestObject)
    RBUnitTests tc;
    QTest::setMainSourcePath(__FILE__);
    return QTest::qExec(&tc, argc, argv);
}
