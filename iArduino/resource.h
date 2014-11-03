#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <stdio.h>
#include <string.h>

// #define DEBUG
#ifdef DEBUG
#define TRACE(x)              printf("\n%s : %d\r\n",__FUNCTION__,__LINE__)
#define TRACEINT(x)           printf("\n%s : %d : %d\r\n",__FUNCTION__,__LINE__,x)
#define TRACESTR(x)           printf("\n%s : %d : %s\r\n",__FUNCTION__,__LINE__,x)
#define TRACEADD(x)           printf("\n%s : %d : %p\r\n",__FUNCTION__,__LINE__,x)
#else
#define TRACE(x)
#define TRACEINT(x)
#define TRACESTR(x)
#define TRACEADD(x)
#define HR
#endif

#define HR                    printf("------------------------------------------\n")

enum RETVAL
{
    RETVAL_OK,
    RETVAL_SYNTAX,
    RETVAL_NULL,
    RETVAL_EMPTY,
    RETVAL_FULL,
    RETVAL_NOT_SUPPORTED,
};

enum RUN_STEP
{
    RUN_STEP_INIT,
    RUN_STEP_RUN,
    RUN_STEP_NOT_RUN,
    RUN_STEP_IF,
};

enum STATEMENT
{
    STATEMENT_INIT,
    STATEMENT_FOR,
    STATEMENT_WHILE,
    STATEMENT_IF,
    STATEMENT_ELSE,
    STATEMENT_CONTINUE,
    STATEMENT_BREAK,
    STATEMENT_END_STM,
    STATEMENT_OPEN_BRK,
    STATEMENT_CLOSE_BRK,
    STATEMENT_ERROR,
};

enum BRACKET
{
    BRACKET_INIT,
    BRACKET_OPEN,
    BRACKET_CLOSE,
};

enum RUN_STATEMENT
{
    RUN_STATEMENT_INIT,
    RUN_STATEMENT_CONDITION,
    RUN_STATEMENT_LAST,
};

enum CONDITION
{
    CONDITION_INIT,
    CONDITION_TRUE,
    CONDITION_FALSE,
};

enum PUSH
{
    PUSH_VARIABLE,
    PUSH_OPERATOR,
    PUSH_ERR,
};

enum SYMBOL_TYPE
{
    SYM_PRIMITIVE,
    SYM_VARIABLE,
    SYM_FUNCTION,
    SYM_ERROR,
};

enum OPERATOR
{
    OPE_DEFAULT,
    OPE_OPEN_PAREN,         // (
    OPE_CLOSE_PAREN,        // )
    OPE_D_EQU,              // ==
    OPE_DIF,                // !=
    OPE_D_OR,               // ||
    OPE_D_AND,              // &&
    OPE_LES_TH,             // <
    OPE_GRE_TH,             // >
    OPE_LES_EQU,            // <=
    OPE_GRE_EQU,            // >=
    OPE_LEF_SH,             // <<
    OPE_RIG_SH,             // >>
    OPE_XOR,                // ^
    OPE_AND,                // &
    OPE_OR,                 // |
    OPE_ADD,                // +
    OPE_SUB,                // -
    OPE_MUL,                // *
    OPE_DIV,                // /
    OPE_MOD,                // %
    OPE_DIF_Z,              // !
    OPE_EQU,             // =
    OPE_D_ADD,              // ++
    OPE_D_SUB,              // --

    OPE_ERR,
};

#define MAX 20

typedef struct _int_stack_t
{
    int elem[MAX];
    int top;
} int_stack_t;

struct Function1 {
     char *name;
     int   len;
     int (*func)(int);
};

struct Function2 {
     char *name;
     int   len;
     int (*func)(int, int);
};

struct Function3 {
     char *name;
     int   len;
     int (*func)(int, int, int);
};

#endif
