#include "debug.h"

int g_dbg;
int g_dbg_st_exe;
int g_dbg_st_exe_case;
int g_dbg_run;
int g_dbg_run_case;

#ifdef DEBUG
char * g_dbg_str[DBG_MAX][DBG_STR_MAX] =
{
    {
        "STATEMENT_INIT",
        "STATEMENT_FOR",
        "STATEMENT_WHILE",
        "STATEMENT_IF",
        "STATEMENT_ELSE",
        "STATEMENT_CONTINUE",
        "STATEMENT_BREAK",
        "STATEMENT_SEMICOLON",
        "STATEMENT_COMMA",
        "STATEMENT_OPEN_BRK",
        "STATEMENT_CLOSE_BRK",
        "STATEMENT_EXECUTION",
        "STATEMENT_ERROR",
    },
    {
        "RETVAL_OK",
        "RETVAL_SYNTAX",
        "RETVAL_NULL",
        "RETVAL_EMPTY",
        "RETVAL_FULL",
        "RETVAL_NOT_SUPPORTED",
    },
    {
        "PUSH_VARIABLE",
        "PUSH_OPERATOR",
        "PUSH_ERR",
    },
    {
        "OPE_DEFAULT",
        "OPE_OPEN_PAREN",         // (
        "OPE_CLOSE_PAREN",        // )
        "OPE_D_EQU",              // ==
        "OPE_DIF",                // !=
        "OPE_D_OR",               // ||
        "OPE_D_AND",              // &&
        "OPE_LES_TH",             // <
        "OPE_GRE_TH",             // >
        "OPE_LES_EQU",            // <=
        "OPE_GRE_EQU",            // >=
        "OPE_LEF_SH",             // <<
        "OPE_RIG_SH",             // >>
        "OPE_XOR",                // ^
        "OPE_AND",                // &
        "OPE_OR",                 // |
        "OPE_ADD",                // +
        "OPE_SUB",                // -
        "OPE_MUL",                // *
        "OPE_DIV",                // /
        "OPE_MOD",                // %
        "OPE_DIF_Z",              // !
        "OPE_EQU",                // =
        "OPE_D_ADD",              // ++
        "OPE_D_SUB",              // --
        "OPE_ERR",
    },
    {
        "CONDITION_INIT",
        "CONDITION_TRUE",
        "CONDITION_FALSE",
    },
};
#else
char * g_dbg_str[DBG_MAX][DBG_STR_MAX] = {0};
#endif
