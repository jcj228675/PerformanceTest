#ifndef PERFORMENCETEST_H
#define PERFORMENCETEST_H

#include "performencetest_global.h"

#include <map>
#include <vector>

#include <QString>
#include <QMutex>
#include <QTime>

class PERFORMENCETEST_EXPORT PerformenceTest
{
public:
	PerformenceTest(QString sFunc);
	~PerformenceTest();
	static void outputResult(const QString &sFileName, bool bClear = false);
	double elapsed(bool bEnd = false);
	static bool m_bEnabled;
	static int m_nTimes;

private:
	static void addTime(QString &sFuncName, long long nTime);
	static void initFrequency();
	class Initor
	{
	public:
		Initor() 
		{
			initFrequency();
		}
	};
	static Initor g_nInitor;

	static std::map<QString, std::vector<long long>> m_mapResult;
	static QMutex m_oMutex;

	long long end();
	bool m_bEnd;
	long long m_nStartTime;
	QString m_sCurFunc;
};

#endif // PERFORMENCETEST_H
