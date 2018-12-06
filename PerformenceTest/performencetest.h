#ifndef PERFORMENCETEST_H
#define PERFORMENCETEST_H

#include "performencetest_global.h"

#include <map>
#include <vector>

class PERFORMENCETEST_EXPORT PerformenceTest
{
public:
	PerformenceTest(const std::wstring &sFunc);
	~PerformenceTest();
	static void outputResult(const std::wstring &sFileName, bool bClear = false);
	double elapsed(bool bEnd = false);
	static bool m_bEnabled;
	static int m_nTimes;

private:
	static void addTime(std::wstring &sFuncName, long long nTime);
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

	static std::map<std::wstring, std::vector<long long>> m_mapResult;

	long long end();
	bool m_bEnd;
	long long m_nStartTime;
	std::wstring m_sCurFunc;
};

#endif // PERFORMENCETEST_H
