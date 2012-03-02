#include "stdafx.h"
#include "logging.h"

#ifdef __DBG__
#include <vector>
#include <string>
#include <windows.h>
using namespace std;

CRITICAL_SECTION gcs;
wstring			 gDbgBase;
FILE *			 gstdout = NULL;
//----------------------------------------------------------------//
class CDbgContext {
public:
	CDbgContext(const TCHAR* szCategory) {
		m_category = szCategory;
		wstring fullName = gDbgBase;
		fullName += _T("_");
		fullName += m_category;
		fullName += _T(".log");
		m_stddout = _tfopen(fullName.c_str(), _T("wt") );
	}

	virtual ~CDbgContext() {
	}

	wstring		m_category;
	FILE *		m_stddout;
};

vector<CDbgContext>	m_contexts;

void FreeContexts() {
	vector<CDbgContext>::iterator it = m_contexts.begin();
	while(it != m_contexts.end()) {
		const CDbgContext& cntxt = *it;
		if (cntxt.m_stddout) {
			fclose(cntxt.m_stddout);
		}
		++it;
	}
	m_contexts.clear();
}
//----------------------------------------------------------------//
FILE* GetCategoryLog(const TCHAR* szCategory) {
	EnterCriticalSection(&gcs);
	vector<CDbgContext>::iterator it = m_contexts.begin();
	while(it != m_contexts.end()) {
		const CDbgContext& cntxt = *it;
		if (_tcsicmp(cntxt.m_category.c_str(), szCategory) == 0) {
			LeaveCriticalSection(&gcs);
			return cntxt.m_stddout;
		}
		++it;
	}
	CDbgContext context(szCategory);
	m_contexts.push_back(context);
	LeaveCriticalSection(&gcs);
	return context.m_stddout;
}
//----------------------------------------------------------------//
void allocdebug(TCHAR *szFolder) {
	InitializeCriticalSection(&gcs);
	//+ HeX [11/28/2007]
	//для помещения всех логов в одну папку
	//CreateDirectory(_T("c:\\AV_LOGS"),NULL);
	//- HeX [11/28/2007]
	TCHAR szFullName[ MAX_PATH ];
	TCHAR szName[ MAX_PATH ];
	GetModuleFileName( NULL, szName, MAX_PATH );
	if(szFolder)
	{	
		CreateDirectory( szFolder, NULL );
		TCHAR *pName = (TCHAR*)_tcsrchr( szName, _T('\\') );
		if( pName == NULL ) 
			pName = _T("undefined");
		else 
			pName++;
		wsprintf( szFullName, _T("%s\\%s"), szFolder, pName );
		
		
	}else
	{
		wsprintf( szFullName, _T("%s"), szName );		
		TCHAR *pName = (TCHAR*)_tcsrchr( szFullName, _T('.') );
		*pName = _T('\0');
	}
	gDbgBase = szFullName;
	_tcscat(szFullName, _T(".log"));
	gstdout = _tfopen( szFullName, _T("wt") );
}
//----------------------------------------------------------------//
void freedebug( void )
{
	FreeContexts();

	if( gstdout == NULL ) return;
	fclose( gstdout );
	gstdout = NULL;

	DeleteCriticalSection(&gcs);
}
//----------------------------------------------------------------//
void outdebugwithprefix(TCHAR c, TCHAR* id, TCHAR* body) {
	TCHAR szTBuffer[128];
	GetTimeFormat(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT,
				  NULL, _T("hh:mm:ss"), szTBuffer, sizeof(szTBuffer) );
	//fprintf(gstdout, "[%s %s] [%d] [%c] [%s] ", szDBuffer, szTBuffer, GetTickCount(), c, id);
	if (gstdout) {
		_ftprintf(gstdout, _T("[%s] [%d] [%.4d] [%c] [%s] %s"), szTBuffer, GetTickCount(), GetCurrentThreadId(), c, id, body);
		fflush(gstdout);
	}

	FILE* cat = GetCategoryLog(id);
	if (cat) {
		_ftprintf(cat, _T("[%s] [%d] [%.4d] [%c] %s"), szTBuffer, GetTickCount(), GetCurrentThreadId(), c, body);
		fflush(cat);
	}

	delete body;
}
void  outdebugwithprefix(char c, char* id, char* body) {
	char szTBuffer[128];
	GetTimeFormatA(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT,
		NULL, "hh:mm:ss", szTBuffer, sizeof(szTBuffer) );
	//fprintf(gstdout, "[%s %s] [%d] [%c] [%s] ", szDBuffer, szTBuffer, GetTickCount(), c, id);
	//EnterCriticalSection(&gcs);
	if (gstdout) {
		fprintf(gstdout, "[%s] [%d] [%.4d] [%c] [%s] %s", szTBuffer, GetTickCount(), GetCurrentThreadId(), c, id, body);
		fflush(gstdout);
	}
	//LeaveCriticalSection(&gcs);
	
wchar_t szWid[MAX_PATH];
	MultiByteToWideChar(CP_ACP,0,id,strlen(id),szWid,MAX_PATH);
	szWid[strlen(id)]=_T('\0');
	FILE* cat = GetCategoryLog(szWid);
	if (cat) {
		fprintf(cat, "[%s] [%d] [%.4d] [%c] %s", szTBuffer, GetTickCount(), GetCurrentThreadId(), c, body);
		fflush(cat);
	}


	delete body;
}

//----------------------------------------------------------------//
TCHAR* outdebug(const TCHAR *lpFmt, ... ) {
    va_list vl;
    va_start( vl, lpFmt );

	int v0,v1,v2,v3,v4,v5,v6,v7,v8,v9;

    v0 = va_arg( vl, int);
    v1 = va_arg( vl, int);
    v2 = va_arg( vl, int);
    v3 = va_arg( vl, int);
    v4 = va_arg( vl, int);
    v5 = va_arg( vl, int);
    v6 = va_arg( vl, int);
    v7 = va_arg( vl, int);
    v8 = va_arg( vl, int);
    v9 = va_arg( vl, int);

	TCHAR* body = new TCHAR[1024];
	_stprintf(body, lpFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
	_tcscat(body, _T("\n"));


	va_end( vl );

	return body;
}

char* outdebug(const char *lpFmt, ... ) {
	va_list vl;
	va_start( vl, lpFmt );

	int v0,v1,v2,v3,v4,v5,v6,v7,v8,v9;

	v0 = va_arg( vl, int);
	v1 = va_arg( vl, int);
	v2 = va_arg( vl, int);
	v3 = va_arg( vl, int);
	v4 = va_arg( vl, int);
	v5 = va_arg( vl, int);
	v6 = va_arg( vl, int);
	v7 = va_arg( vl, int);
	v8 = va_arg( vl, int);
	v9 = va_arg( vl, int);

	char* body = new char[1024];
	sprintf(body, lpFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
	strcat(body, "\n");


	va_end( vl );

	return body;
}

//----------------------------------------------------------------//
#endif
