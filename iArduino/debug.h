#ifndef DEBUG_H
#define DEBUG_H

#include "resource.h"

#ifdef TCI_DEBUG

#define DBG_STR_MAX         0x2000

enum DBG_STR
{
    DBG_STATEMENT,
    DBG_RET_VAL,
    DBG_PUSH,
    DBG_OPERATOR,
    DBG_CONDITION,
    DBG_MAX,
};

extern char * g_dbg_str[DBG_MAX][DBG_STR_MAX];
extern int g_dbg;
extern int g_dbg_st_exe;
extern int g_dbg_st_exe_case;
extern int g_dbg_run;
extern int g_dbg_run_case;

#define TRACE(x)              if(g_dbg) printf("\n%s : %d\r\n",__FUNCTION__,__LINE__)
#define TRACEINT(x)           if(g_dbg) printf("\n%s : %d : %d\r\n",__FUNCTION__,__LINE__,x)
#define TRACESTR(x)           if(g_dbg) printf("\n%s : %d : %s\r\n",__FUNCTION__,__LINE__,x)
#define TRACESTRINT(x,y)      if(g_dbg) printf("\n%s : %d : %s : %d\r\n",__FUNCTION__,__LINE__,x,y)
#define TRACEADD(x)           if(g_dbg) printf("\n%s : %d : %p\r\n",__FUNCTION__,__LINE__,x)
#define ASSERT(expr)          if(!expr) while(1);

#else

#define TRACE(x)
#define TRACEINT(x)
#define TRACESTR(x)
#define TRACESTRINT(x,y)
#define TRACEADD(x)
#define ASSERT(expr)

#endif

#endif
