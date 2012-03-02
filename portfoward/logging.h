#ifndef __DEBUG__DEF__H__
#define __DEBUG__DEF__H__


#ifdef __DBG__
#include <windows.h>
#include <stdio.h>

void  outdebugwithprefix(TCHAR c, TCHAR* id, TCHAR* body);
void  outdebugwithprefix(char c, char* id, char* body);
TCHAR* outdebug( const TCHAR *lpFmt, ... );
char* outdebug( const char *lpFmt, ... );
void allocdebug( TCHAR *szFolder );
void freedebug( );

#define tdbg_i( b, a )     outdebugwithprefix(_T('I'), b, outdebug a);
#define tdbg_w( b, a )     outdebugwithprefix(_T('W'), b, outdebug a);
#define tdbg_e( b, a )     outdebugwithprefix(_T('E'), b, outdebug a);
#define dbg_i( b, a )     outdebugwithprefix('I', b, outdebug a);
#define dbg_w( b, a )     outdebugwithprefix('W', b, outdebug a);
#define dbg_e( b, a )     outdebugwithprefix('E', b, outdebug a);
#define dbgalloc( a ) allocdebug( a );
#define dbgfree( )    freedebug( );
#endif

#ifndef __DBG__
#define dbg_i( b, a )
#define dbg_w( b, a )
#define dbg_e( b, a )
#define dbgalloc( a )
#define dbgfree( )
#endif
#endif


