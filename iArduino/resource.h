#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <stdio.h>
#include <string.h>

#define TEST
#define DEBUG

#ifdef DEBUG
#define TRACE(x)              printf("\n%s : %d\r\n",__FUNCTION__,__LINE__)
#define TRACEINT(x)           printf("\n%s : %d : %d\r\n",__FUNCTION__,__LINE__,x)
#define TRACESTR(x)           printf("\n%s : %d : %s\r\n",__FUNCTION__,__LINE__,x)
#define TRACESTRINT(x,y)      printf("\n%s : %d : %s : %d\r\n",__FUNCTION__,__LINE__,x,y)
#define TRACEADD(x)           printf("\n%s : %d : %p\r\n",__FUNCTION__,__LINE__,x)
#else
#define TRACE(x)
#define TRACEINT(x)
#define TRACESTR(x)
#define TRACESTRINT(x,y)
#define TRACEADD(x)
#endif

enum DBG_STR
{
    DBG_STATEMENT,
    DBG_RET_VAL,
    DBG_PUSH,
    DBG_OPERATOR,
    DBG_CONDITION,
    DBG_MAX,
};

#define DBG_STR_MAX         256

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
    RUN_STEP_1ST,
    RUN_STEP_2ND,
    RUN_STEP_3RD,
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
    STATEMENT_SEMICOLON,
    STATEMENT_COMMA,
    STATEMENT_OPEN_BRK,
    STATEMENT_CLOSE_BRK,
    STATEMENT_EXECUTION,
    STATEMENT_ERROR,
};

enum BRACKET
{
    BRACKET_INIT,
    BRACKET_OPEN,
    BRACKET_CLOSE,
};

enum CONDITION
{
    CONDITION_INIT,
    CONDITION_TRUE,
    CONDITION_FALSE,
};

enum SYMBOL_TYPE
{
    SYM_PRIMITIVE,
    SYM_VARIABLE,
    SYM_FUNCTION,
    SYM_ERROR,
};

enum PUSH
{
    PUSH_VARIABLE,
    PUSH_OPERATOR,
    PUSH_ERR,
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
    OPE_EQU,                // =
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

typedef struct _ptr_stack_t
{
    char *elem[MAX];
    int top;
} ptr_stack_t;

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

int is_white_space(char ch);

int is_empty(int_stack_t *st);

int size_int(int_stack_t *st);

int size_ptr(ptr_stack_t *st);

int top_int(int_stack_t *st);

char *top_ptr(ptr_stack_t *st);

int pop_int(int_stack_t *st, int *elem);

int pop_ptr(ptr_stack_t *st, char **elem);

int push_int(int_stack_t *st, int elem);

int push_ptr(ptr_stack_t *st, char *elem);

int prio_operator(int ope);

int calculate_operator(int l_operant, int r_operant, int ope);

int walk_through_parenthesis(char **start, char *end);

int get_primitive_val(char ** sym_s, char *const sym_e, int *val);

int is_operator(char ** symbol);

int run(char *prog, int size);

int statement_execution(char infix_s[], char infix_e[], int *rs);

int check_statement(char *statement);

int check_symbol(char *sym_s);

void print_ret_val(int rs);

void print_symbol(char *symbol);

void print_ope(int ope);

void print_stack_int(int_stack_t *st);

void print_stack_ptr(ptr_stack_t *st);

void print_ope_stack(int_stack_t *st);



#endif
