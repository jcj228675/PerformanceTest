#include "performencetest.h"

#include <windows.h>

#include <QFile>
#include <QTextStream>

static long long g_nFrequency = 0;

void getCpuFreqency(long long &v)
{
	DWORD BufSize = _MAX_PATH;
	DWORD dwMHz = _MAX_PATH;
	HKEY hKey;

	// open the key where the proc speed is hidden:

	long lError = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
		0,
		KEY_READ,
		&hKey);

	if (lError != ERROR_SUCCESS)
	{// if the key is not found, tell the user why:
		v = 0;
		return;
	}

	// query the key:

	RegQueryValueExA(hKey, "~MHz", NULL, NULL, (LPBYTE)&dwMHz, &BufSize);

	// convert the DWORD to a CString:
	v = dwMHz * 1000000;
}

PerformenceTest::PerformenceTest(QString sFuncName)
: m_sCurFunc(sFuncName), m_bEnd(false)
{
	m_nStartTime = __rdtsc();
}

PerformenceTest::~PerformenceTest()
{
	if (!m_bEnd)
	{
		long long nEndTime = __rdtsc();
		addTime(m_sCurFunc, nEndTime - m_nStartTime);
	}
}

void PerformenceTest::addTime(QString &sFuncName, long long nTime)
{
	QMutexLocker oLocker(&m_oMutex);
	m_mapResult[sFuncName].push_back(nTime);
}

void PerformenceTest::outputResult(const QString &sFileName, bool bClear)
{
	QFile oFile(sFileName);
	oFile.open(QIODevice::ReadWrite | QIODevice::Truncate);
	QTextStream oStream(&oFile);

	for (auto ite = m_mapResult.begin(); ite != m_mapResult.end(); ++ite)
	{
		auto &vecRecord = ite->second;
		std::size_t nCalledCount = vecRecord.size();
		double nTotalTime = 0;
		for (std::size_t i = 0; i < vecRecord.size(); ++i)
		{
			nTotalTime += vecRecord[i];
		}
		double nAverageTime = nTotalTime / nCalledCount;

		QString sResult = ite->first + QString("  called times: %1, average time: %2, total time: %3").
			arg(nCalledCount).arg(nAverageTime / g_nFrequency * 1000).arg(nTotalTime / g_nFrequency * 1000);
		oStream << sResult << "\r\n";
	}
	if (bClear)
	{
		m_mapResult.clear();
	}
}

void PerformenceTest::initFrequency()
{
	getCpuFreqency(g_nFrequency);
}

double PerformenceTest::elapsed(bool bEnd)
{
	if (!bEnd)
	{
		long long nCurTime = __rdtsc();
		return (double)(nCurTime - m_nStartTime) / g_nFrequency * 1000;
	}
	else
	{
		return (double)end() / g_nFrequency * 1000;
	}
}

long long PerformenceTest::end()
{
	m_bEnd = true;
	long long nEndTime = __rdtsc();
	addTime(m_sCurFunc, nEndTime - m_nStartTime);
	return nEndTime - m_nStartTime;
}

int PerformenceTest::m_nTimes = 0;

bool PerformenceTest::m_bEnabled = false;

PerformenceTest::Initor PerformenceTest::g_nInitor;

QMutex PerformenceTest::m_oMutex;

std::map<QString, std::vector<long long>> PerformenceTest::m_mapResult;
