#include <QtTest/QtTest>

class RBUnitTests : public QObject
{
	Q_OBJECT

private slots:
	void initTestCase();

	void serializeTest();
	void serializeTestApp();
	void serializeTestRBConfigs();

private:
	QSharedPointer<QTemporaryDir> m_spTestDir;
};
