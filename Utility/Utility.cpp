
#include <Utility/Utility.h>

CUtility::CUtility()
{
	//用于测试时间
	QueryPerformanceFrequency(&m_PerfFreq);  //获得CPU的主频
}

CUtility::~CUtility()
{
}

void CUtility::StartTime()
{
	QueryPerformanceCounter(&m_PerfStart);
}

void CUtility::EndTime()
{
	QueryPerformanceCounter(&m_PerfEnd);

	dTime = (float)((m_PerfEnd.QuadPart - m_PerfStart.QuadPart)*1000)/m_PerfFreq.QuadPart;
}

void CUtility::Utility_getApplicationPath(char* dstPath)
{
	GetModuleFileName(NULL, dstPath, MAX_FILE);
}

void CUtility::Utility_getApplicationDir(char* dstPath)
{
	GetModuleFileName(NULL, dstPath, MAX_FILE);
	char* fileDir = strrchr(dstPath,'\\');
	if (fileDir)
	{
		*fileDir = '\0';
	}
}

void CUtility::Utility_GetCurTimeStr(char* dstPath)
{
	CTime time = CTime::GetCurrentTime();
	CString strCurDate;
	strCurDate.Format(
		"%04d-%02d-%02d-%02d-%02d-%02d",
		time.GetYear(),
		time.GetMonth(),
		time.GetDay(),
		time.GetHour(),
		time.GetMinute(),
		time.GetSecond());

	strcpy(dstPath, strCurDate.GetBuffer(strCurDate.GetLength()));
}

void CUtility::Utility_GetCurTimeStrFormat(char* dstPath)
{
	CTime time = CTime::GetCurrentTime();
	CString strCurDate;
	strCurDate.Format(
		"%04d年 %02d月%02d日 %02d-%02d-%02d",
		time.GetYear(),
		time.GetMonth(),
		time.GetDay(),
		time.GetHour(),
		time.GetMinute(),
		time.GetSecond());

	strcpy(dstPath, strCurDate.GetBuffer(strCurDate.GetLength()));
}

void CUtility::Utility_getMonthStr(char* dstPath)
{
	CTime time = CTime::GetCurrentTime();
	CString strCurDate;
	strCurDate.Format(
		"%04d-%02d",
		time.GetYear(),
		time.GetMonth());

	strcpy(dstPath, strCurDate.GetBuffer(strCurDate.GetLength()));
}

void CUtility::Utility_getDayStr(char* dstPath)
{
	CTime time = CTime::GetCurrentTime();
	CString strCurDate;
	strCurDate.Format(
		"%04d-%02d-%02d",
		time.GetYear(),
		time.GetMonth(),
		time.GetDay());

	strcpy(dstPath, strCurDate.GetBuffer(strCurDate.GetLength()));
}

BOOL CUtility::Utility_CreateDirByPath (const char * srcPath)
{
	char szbuf[MAX_FILE] = {0};
	int pos = 0;

	int PathLen = strlen(srcPath);
	for (int i = 0; i < PathLen; i++)
	{
		if (srcPath[i] == '\\' || srcPath[i] == '/')
		{
			// 一级目录
			pos = i;
			strncpy(szbuf, srcPath, pos);
			szbuf[pos] = '\0';

			if (_access(szbuf, 0) == -1)  // 是否有此目录
			{
				if (_mkdir(szbuf) == -1)  // 创建目录
					return FALSE;
			}
		}
	}

	if (pos == 0)
		return FALSE;

	return TRUE;
}

void CUtility::Utility_getDirByPath (const char* srcPath, char* dstPath)
{
	strcpy(dstPath, srcPath);
	char* fileDir = strrchr(dstPath,'\\');
	if (fileDir)
	{
		*fileDir = '\0';
	}
}

void CUtility::Utility_getFileNameByPath(const char* FilePath, char* FileName)
{
	strcpy(FileName, FilePath);

	FileName = strrchr(FileName, '\\') + 1;
	if (FileName == NULL || *FileName == 0 || *FileName == '\0')
		FileName = "";
}

using namespace std;   

CCheckMem::CCheckMem()
{
}

CCheckMem::~CCheckMem()
{
}

void CCheckMem::MemCheckStartPoint()
{
	_CrtMemCheckpoint( &s1 );
}

void CCheckMem::MemCheckEndPoint()
{
	_CrtMemCheckpoint( &s2 );
}

void CCheckMem::MemDumpStatistics()
{
	if ( _CrtMemDifference( &s3, &s1, &s2) ) 
		_CrtMemDumpStatistics( &s3 );

	_CrtDumpMemoryLeaks();
}