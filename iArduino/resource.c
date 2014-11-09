#include "resource.h"

int g_test;

char g_test_1[DBG_STR_MAX];
int g_test_size_1 = 0;

char g_test_2[DBG_STR_MAX];
int g_test_size_2 = 0;

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
int print(int val)
{
#ifdef TEST
    if(g_test)
    {
        g_test_size_1 += sprintf(g_test_1 + g_test_size_1, "%d ", val);
    }
    else
    {
        g_test_size_2 += sprintf(g_test_2 + g_test_size_2, "%d ", val);
    }
#endif;
    if(g_test)
    {
        printf("%d\n", val);
    }
    return val;
}

const struct Function1 func1[] = {
     {"print", 5, print},
};
#define FUNC1_NUM 1

const struct Function2 func2[] = {
    0
};
#define FUNC2_NUM 0

const struct Function3 func3[] = {
    0
};
#define FUNC3_NUM 0

int variables[26] = {0};

void print_stack_int(int_stack_t *st)
{
    int i;
    for(i=0; i<st->top; i++)
    {
        printf("%d ", st->elem[i]);
    }
    printf("\n");
}

void print_stack_ptr(ptr_stack_t *st)
{
    int i;
    for(i=0; i<st->top; i++)
    {
        printf("%d ", (uint32_t)st->elem[i]);
    }
    printf("\n");
}

int is_white_space(char ch)
{
    return (ch == '\n') || (ch == '\r') || (ch == '\t') || (ch == ' ');
}

int is_empty(int_stack_t *st)
{
    if(st)
    {
        return !st->top;
    }
    return -1;
}

int size_int(int_stack_t *st)
{
    return st->top;
}

int size_ptr(ptr_stack_t *st)
{
    return st->top;
}

int top_int(int_stack_t *st)
{
    if(!st)
    {
        return -1;
    }
    return st->top ? st->elem[(st->top) - 1] : st->elem[0];
}

char * top_ptr(ptr_stack_t *st)
{
    if(!st)
    {
        return NULL;
    }
    return st->top ? st->elem[(st->top) - 1] : st->elem[0];
}

int pop_ptr(ptr_stack_t *st, char **elem)
{
    if(!st)
    {
        return RETVAL_NULL;
    }
    if(st->top == 0)
    {
        return RETVAL_EMPTY;
    }
    if(elem)
    {
        *elem = st->elem[(st->top)-1];
    }
    st->top--;
    return RETVAL_OK;
}

int pop_int(int_stack_t *st, int *elem)
{
    if(!st)
    {
        return RETVAL_NULL;
    }
    if(st->top == 0)
    {
        return RETVAL_EMPTY;
    }
    if(elem)
    {
        *elem = st->elem[(st->top)-1];
    }
    st->top--;
    return RETVAL_OK;
}

int push_ptr(ptr_stack_t *st, char *elem)
{
    if(!st)
    {
        return RETVAL_NULL;
    }
    if(st->top == MAX)
    {
        return RETVAL_FULL;
    }
    st->elem[st->top] = elem;
    st->top++;
    return RETVAL_OK;
}

int push_int(int_stack_t *st, int elem)
{
    if(!st)
    {
        return RETVAL_NULL;
    }
    if(st->top == MAX)
    {
        return RETVAL_FULL;
    }
    st->elem[st->top] = elem;
    st->top++;
    return RETVAL_OK;
}

int prio_operator(int ope)
{
    int ret = 0;
    switch(ope)
    {
        case OPE_D_EQU:
        case OPE_DIF:
        case OPE_D_OR:
        case OPE_D_AND:
            ret = 3;
        break;

        case OPE_LES_TH:
        case OPE_GRE_TH:
        case OPE_LES_EQU:
        case OPE_GRE_EQU:
            ret = 5;
        break;

        case OPE_LEF_SH:
        case OPE_RIG_SH:
        case OPE_XOR:
        case OPE_AND:
        case OPE_OR:
            ret = 7;
        break;

        case OPE_ADD:
        case OPE_SUB:
            ret = 9;
        break;

        case OPE_MUL:
        case OPE_DIV:
        case OPE_MOD:
            ret = 11;
        break;

        case OPE_DIF_Z:
            ret = 13;
        break;

        default:
            ret = 1;
        break;
    }

    return ret;
}

int calculate_operator(int l_operant, int r_operant, int ope)
{
    switch(ope)
    {
        case OPE_D_EQU:
            return (l_operant == r_operant);
        case OPE_DIF:
            return (l_operant != r_operant);
        case OPE_D_OR:
            return (l_operant || r_operant);
        case OPE_D_AND:
            return (l_operant && r_operant);
        case OPE_LES_TH:
            return (l_operant < r_operant);
        case OPE_GRE_TH:
            return (l_operant > r_operant);
        case OPE_LES_EQU:
            return (l_operant <= r_operant);
        case OPE_GRE_EQU:
            return (l_operant >= r_operant);
        case OPE_LEF_SH:
            return (l_operant << r_operant);
        case OPE_RIG_SH:
            return (l_operant >> r_operant);
        case OPE_XOR:
            return (l_operant ^ r_operant);
        case OPE_AND:
            return (l_operant & r_operant);
        case OPE_OR:
            return (l_operant | r_operant);
        case OPE_ADD:
            return (l_operant + r_operant);
        case OPE_SUB:
            return (l_operant - r_operant);
        case OPE_MUL:
            return (l_operant * r_operant);
        case OPE_DIV:
            return (l_operant / r_operant);
        case OPE_MOD:
            return (l_operant % r_operant);
    }
    return 0;
}

int walk_through_parenthesis(char **start, char *end)
{
    int in;
    char *cp_s;

    if(!start || !*start || !end || *start>=end)
    {
        return RETVAL_NULL;
    }
    cp_s = *start;
    while(cp_s<end && *cp_s!='(')
    {
        cp_s+=1;
    }

    for(in=1, cp_s++; (cp_s<end) && (in>0); cp_s++)
    {
        if(*cp_s == '(')
        {
            in++;
        }
        else if(*cp_s ==')')
        {
            in--;
        }
    }

    if(in)
    {
        return RETVAL_SYNTAX;
    }
    *start = cp_s;
    return RETVAL_OK;
}

int get_primitive_val(char ** sym_s, int size, int *val)
{
    char *sym_e;
    char *cp_s;
    if(!sym_s || !(*sym_s) || !size || !val)
    {
        return RETVAL_NULL;
    }
    sym_e = *sym_s+size;
    char *const cp_e = sym_e;
    cp_s = *sym_s;
    if(*cp_s == '0') // binary or hexadecimal format.
    {
        if((*(cp_s+1) != 'x') && (*(cp_s+1) != 'b')) // val is zero.
        {
            cp_s++;
        }
        else if(*(cp_s+1)=='x') // hexadecimal format
        {
            // check syntax
            if(!(cp_s+1) || !(cp_s+2) || (cp_s+2)>=cp_e)
            {
                TRACE();
                return RETVAL_SYNTAX;
            }
            // walk through 'x'
            cp_s+=2;
            // if nothing hex's character left after 'x', return error syntax.
            if(!((cp_s < cp_e) &&
                (((*cp_s>='0') && (*cp_s<='9')) ||
                ((*cp_s>='A') && (*cp_s<='F')) ||
                ((*cp_s>='a') && (*cp_s<='f')))
                ))
            {
                TRACE();
                return RETVAL_SYNTAX;
            }
            while((cp_s < cp_e) &&
                (((*cp_s>='0') && (*cp_s<='9')) ||
                ((*cp_s>='A') && (*cp_s<='F')) ||
                ((*cp_s>='a') && (*cp_s<='f')))
                )
            {
                *val <<= 4;
                if (*cp_s>='0' && *cp_s<='9')
                {
                    *val += (*cp_s - '0');
                }
                else if (*cp_s>='a' && *cp_s<='f')
                {
                    *val += (*cp_s - 'a' + 0xa);
                }
                else if (*cp_s>='A' && *cp_s<='F')
                {
                    *val += (*cp_s - 'A' + 0xa);
                }
                cp_s++;
            }
        }
        else
        {
            // binary format.
            if(!((cp_s < cp_e) && (*cp_s>='0') && (*cp_s<='1')))
            {
                TRACE();
                return RETVAL_SYNTAX;
            }
            // check syntax
            if(!(cp_s+1) || !(cp_s+2) || (cp_s+2)>=cp_e)
            {
                TRACE();
                return RETVAL_SYNTAX;
            }
            // walk through b
            cp_s += 2;
            while((cp_s < cp_e) && (*cp_s>='0') && (*cp_s<='1'))
            {
                *val = (*val<<1) + (*cp_s - '0');
                cp_s++;
            }
        }
    }
    else if(*cp_s>='0' && *cp_s<='9') // decimal format.
    {
        while((cp_s < cp_e) && (*cp_s>='0') && (*cp_s<='9'))
        {
            *val *= 10;
            *val += (*cp_s - '0');
            cp_s++;
        }
    }
    else // bool
    {
        TRACE();
        return RETVAL_SYNTAX;
    }
    cp_s--;
    *sym_s = cp_s;
    return RETVAL_OK;
}

// int is_operator(char *symbol, int size)
int is_operator(char ** symbol)
{
    int ret = OPE_ERR;
    if((symbol == 0) || (*symbol+1) == 0)
    {
        return ret;
    }

    switch(**symbol)
    {
        case '=':
            if(*(*symbol+1) == '=')
            {
                *symbol+=1;
                ret = OPE_D_EQU;
            }
            else
            {
                ret = OPE_EQU;
            }
        break;

        case '!':
            if(*(*symbol+1) == '=')
            {
                *symbol+=1;
                ret = OPE_DIF;
            }
            else
            {
                ret = OPE_DIF_Z;
            }
        break;

        case '|':
            if(*(*symbol+1) == '|')
            {
                *symbol+=1;
                ret = OPE_D_OR;
            }
            else
            {
                ret = OPE_OR;
            }
        break;

        case '&':
            if(*(*symbol+1) == '&')
            {
                *symbol+=1;
                ret = OPE_D_AND;
            }
            else
            {
                ret = OPE_AND;
            }
        break;

        case '<':
            if(*(*symbol+1) == '=')
            {
                *symbol+=1;
                ret = OPE_LES_EQU;
            }
            else if(*(*symbol+1) == '<')
            {
                *symbol+=1;
                ret = OPE_LEF_SH;
            }
            else
            {
                ret = OPE_LES_TH;
            }
        break;

        case '>':
            if(*(*symbol+1) == '=')
            {
                *symbol+=1;
                ret = OPE_GRE_EQU;
            }
            else if(*(*symbol+1) == '>')
            {
                *symbol+=1;
                ret = OPE_RIG_SH;
            }
            else
            {
                ret = OPE_GRE_TH;
            }
        break;

        case '^':
            ret = OPE_XOR;
        break;

        case '+':
            ret = OPE_ADD;
        break;

        case '-':
            ret = OPE_SUB;
        break;

        case '*':
            ret = OPE_MUL;
        break;

        case '/':
            ret = OPE_DIV;
        break;

        case '%':
            ret = OPE_MOD;
        break;

        case '(':
            ret = OPE_OPEN_PAREN;
        break;

        case ')':
            ret = OPE_CLOSE_PAREN;
        break;

        default:
        break;
    }
    return ret;
}

int check_statement(char *statement)
{
    if(!statement)
    {
        return STATEMENT_EXECUTION;
    }
    if(!strncmp(statement, "for", sizeof("for") - 1))
    {
        return STATEMENT_FOR;
    }

    if(!strncmp(statement, "while", sizeof("while") - 1))
    {
        return STATEMENT_WHILE;
    }

    if(!strncmp(statement, "if", sizeof("if") - 1))
    {
        return STATEMENT_IF;
    }

    if(!strncmp(statement, "else", sizeof("else") - 1))
    {
        return STATEMENT_ELSE;
    }

    if(!strncmp(statement, "continue", sizeof("continue") - 1))
    {
        return STATEMENT_CONTINUE;
    }

    if(!strncmp(statement, "break", sizeof("break") - 1))
    {
        return STATEMENT_BREAK;
    }

    if(*statement == '{')
    {
        return STATEMENT_OPEN_BRK;
    }

    if(*statement == '}')
    {
        return STATEMENT_CLOSE_BRK;
    }

    if(*statement == ';')
    {
        return STATEMENT_SEMICOLON;
    }

    if(*statement == ',')
    {
        return STATEMENT_COMMA;
    }

    return STATEMENT_EXECUTION;
}

int check_symbol(char *symbol)
{
    char * sym_s;
    if(!symbol)
    {
        return SYM_ERROR;
    }
    sym_s = symbol;
    if(*symbol >= '0' && *symbol <= '9')
    {
        return SYM_PRIMITIVE;
    }
    else if((*symbol >= 'a' && *symbol <= 'z') || (*symbol >= 'A' && *symbol <= 'Z'))
    {
        symbol++;
        while(symbol &&
            ((*symbol >= '0' && *symbol <= '9') ||
                (*symbol >= 'a' && *symbol <= 'z') ||
                (*symbol >= 'A' && *symbol <= 'Z'))
            )
        {
            symbol++;
        }
        if(symbol == (sym_s + 1))
        {
            return SYM_VARIABLE;
        }
        while(is_white_space(*symbol))
        {
            symbol++;
        }
        if(*symbol == '(')
        {
            return SYM_FUNCTION;
        }
    }
    return SYM_ERROR;
}

int run(char *prog, int size)
{
    char *prog_s, *prog_e, *run1_s_ptr, *run1_e_ptr, *run2_s_ptr, *run3_s_ptr, *run3_e_ptr, *sub_prog_s_ptr, *sub_prog_e_ptr;
    char *cp_s, *cp_e;
    int state, run_step, val, prev_state, syntax_state, condition, level_by_pass, level_crr, level_next;
    int_stack_t cond_st, brk_st, state_st, loop_level_st;
    ptr_stack_t run1_s_ptr_st, run1_e_ptr_st, run3_s_ptr_st, run3_e_ptr_st, run2_s_ptr_st, sub_prog_s_ptr_st;

    int run1_size, run2_size, run3_size;
    int_stack_t run1_size_st, run2_size_st, run3_size_st;


    state = STATEMENT_INIT;
    prev_state = state;
    syntax_state = state;
    run_step = RUN_STEP_INIT;
    prog_s = prog;
    prog_e = prog_s + size;
    run1_s_ptr = 0;
    run1_size = 0;
    run2_s_ptr = 0;
    run2_size = 0;
    run3_s_ptr = 0;
    run3_size = 0;
    sub_prog_s_ptr = 0;
    sub_prog_e_ptr = 0;
    condition = CONDITION_INIT;
    level_next = 0;
    level_crr = 0;
    level_by_pass = MAX;
    memset(&cond_st, 0, sizeof(int_stack_t));
    memset(&brk_st, 0, sizeof(int_stack_t));
    memset(&state_st, 0, sizeof(int_stack_t));
    memset(&loop_level_st, 0, sizeof(int_stack_t));

    memset(&run1_s_ptr_st, 0, sizeof(ptr_stack_t));
    memset(&run1_e_ptr_st, 0, sizeof(ptr_stack_t));

    memset(&run2_s_ptr_st, 0, sizeof(ptr_stack_t));
    memset(&run2_size_st, 0, sizeof(int_stack_t));

    memset(&run3_s_ptr_st, 0, sizeof(ptr_stack_t));
    memset(&run3_e_ptr_st, 0, sizeof(ptr_stack_t));

    memset(&sub_prog_s_ptr_st, 0, sizeof(ptr_stack_t));

    for(; prog_s < prog_e;)
    {
        if(is_white_space(*prog_s))
        {
            prog_s++;
            continue;
        }

        if(g_dbg_run)
        {
            TRACESTR(prog_s);
        }

        if(state == STATEMENT_INIT)
        {
            state = check_statement(prog_s);
        }

        switch(state)
        {
                // syntax_state = STATEMENT_FOR;
                // if(g_dbg_run_case) TRACESTR(g_dbg_str[DBG_STATEMENT][syntax_state]);
                // switch(run_step)
                // {
                //     case RUN_STEP_INIT:
                //         run_step = RUN_STEP_2ND;
                //         if(!run2_s_ptr)
                //         {
                //             prog_s = prog_s + sizeof("while") - 1;
                //             cp_s = prog_s;
                //             if(walk_through_parenthesis(&prog_s, prog_e) != RETVAL_OK)
                //             {
                //                 TRACE();
                //                 return RETVAL_SYNTAX;
                //             }
                //             run2_s_ptr = cp_s;
                //             run2_e_ptr = prog_s;
                //             sub_prog_s_ptr = prog_s;
                //         }
                //     break;

                //     case RUN_STEP_2ND:
                //         if (size_int(&brk_st))
                //         {
                //             level_crr = level_next;
                //         }
                //         level_next++;
                //         if(level_by_pass > (level_next - 1))
                //         {
                //             if(statement_execution(run2_s_ptr, run2_e_ptr, &val) != RETVAL_OK)
                //             {
                //                 TRACE();
                //                 return RETVAL_SYNTAX;
                //             }
                //             if(!val)
                //             {
                //                 condition = CONDITION_FALSE;
                //                 level_by_pass = level_next;
                //             }
                //             else
                //             {
                //                 condition = CONDITION_TRUE;
                //                 level_by_pass = MAX;
                //                 push_int(&state_st, state);
                //                 push_int(&loop_level_st, level_crr);
                //                 push_ptr(&run2_s_ptr_st, run2_s_ptr);
                //                 push_ptr(&run2_e_ptr_st, run2_e_ptr);
                //                 push_ptr(&sub_prog_s_ptr_st, sub_prog_s_ptr);
                //             }
                //             TRACESTR(g_dbg_str[DBG_CONDITION][condition]);
                //         }
                //         run_step = RUN_STEP_INIT;
                //         state = STATEMENT_INIT;
                //     break;
                // }
                // prev_state = STATEMENT_FOR;
            // break;
            case STATEMENT_FOR:
            case STATEMENT_WHILE:
                syntax_state = state;
                if(g_dbg_run_case) TRACESTR(g_dbg_str[DBG_STATEMENT][syntax_state]);
                switch(run_step)
                {
                    case RUN_STEP_INIT:
                        if(state == STATEMENT_WHILE)
                        {
                            run_step = RUN_STEP_2ND;
                            if(!run2_s_ptr)
                            {
                                prog_s = prog_s + sizeof("while") - 1;
                                cp_s = prog_s;
                                if(walk_through_parenthesis(&prog_s, prog_e) != RETVAL_OK)
                                {
                                    TRACE();
                                    return RETVAL_SYNTAX;
                                }
                                run2_s_ptr = cp_s;
                                run2_size = prog_s - cp_s;
                                sub_prog_s_ptr = prog_s;
                            }
                        }
                        else
                        {
                            if(!run2_s_ptr)
                            {
                                prog_s = prog_s + sizeof("for") - 1;
                                while(*prog_s != '(')
                                {
                                    prog_s++;
                                }
                                cp_s = prog_s;
                                if(walk_through_parenthesis(&prog_s, prog_e) != RETVAL_OK)
                                {
                                    TRACE();
                                    return RETVAL_SYNTAX;
                                }
                                cp_s++;
                                run1_s_ptr = cp_s;
                                while(*cp_s != ';')
                                {
                                    cp_s++;
                                    run1_size++;
                                }
                                cp_s++;
                                run2_s_ptr = cp_s;
                                while(*cp_s != ';')
                                {
                                    cp_s++;
                                    run2_size++;
                                }
                                cp_s++;
                                run3_s_ptr = cp_s;
                                run3_size = prog_s - cp_s - 1;
                                sub_prog_s_ptr = prog_s;
                                run_step = RUN_STEP_1ST;
                                TRACESTR(run1_s_ptr+run1_size);HR;
                                TRACESTR(run2_s_ptr+run2_size);HR;
                                TRACESTR(run3_s_ptr+run3_size);HR;
                                // return 0;
                                TRACESTR(prog_s);
                            }
                            else
                            {
                                run_step = RUN_STEP_3RD;
                            }
                        }
                    break;

                    case RUN_STEP_1ST:
                        if(statement_execution(run1_s_ptr, run1_size, NULL) != RETVAL_OK)
                        {
                            TRACE();
                            return RETVAL_SYNTAX;
                        }
                        run_step = RUN_STEP_2ND;
                    break;

                    case RUN_STEP_2ND:
                        if (size_int(&brk_st))
                        {
                            // TRACESTRINT("level_crr",level_crr); TRACESTRINT("level_next",level_next); TRACESTRINT("level_by_pass",level_by_pass);
                            level_crr = level_next;
                        }
                        level_next++;
                        if(level_by_pass > (level_next - 1))
                        {
                            if(statement_execution(run2_s_ptr, run2_size, &val) != RETVAL_OK)
                            {
                                TRACE();
                                return RETVAL_SYNTAX;
                            }
                            if(!val)
                            {
                                condition = CONDITION_FALSE;
                                level_by_pass = level_next;
                            }
                            else
                            {
                                condition = CONDITION_TRUE;
                                level_by_pass = MAX;
                                push_int(&state_st, state);
                                push_int(&loop_level_st, level_crr);
                                push_ptr(&run2_s_ptr_st, run2_s_ptr);
                                push_int(&run2_size_st, run2_size);
                                push_ptr(&run3_s_ptr_st, run3_s_ptr);
                                push_int(&run3_size_st, run3_size);
                                push_ptr(&sub_prog_s_ptr_st, sub_prog_s_ptr);
                            }
                        }
                        run1_s_ptr = 0;
                        run1_size = 0;
                        run2_s_ptr = 0;
                        run2_size = 0;
                        run3_s_ptr = 0;
                        run3_size = 0;
                        sub_prog_s_ptr = 0;
                        run_step = RUN_STEP_INIT;
                        state = STATEMENT_INIT;
                        TRACESTR(g_dbg_str[DBG_CONDITION][condition]);
                    break;

                    case RUN_STEP_3RD:
                        run_step = RUN_STEP_2ND;
                        if(statement_execution(run3_s_ptr, run3_size, NULL) != RETVAL_OK)
                        {
                            TRACE();
                            return RETVAL_SYNTAX;
                        }
                    break;
                }
                prev_state = syntax_state;
            break;

            case STATEMENT_IF:
                syntax_state = STATEMENT_IF;
                if(g_dbg_run_case) TRACESTR(g_dbg_str[DBG_STATEMENT][syntax_state]);
                if (size_int(&brk_st))
                {
                    level_crr = level_next;
                }
                level_next++;
                prog_s = prog_s + sizeof("if") - 1;
                if(prog_s >= prog_e)
                {
                    TRACE();
                    return RETVAL_SYNTAX;
                }
                cp_s = prog_s;
                if(walk_through_parenthesis(&prog_s, prog_e) != RETVAL_OK)
                {
                    TRACE();
                    return RETVAL_SYNTAX;
                }
                if(level_by_pass > (level_next - 1))
                {
                    if(statement_execution(cp_s, prog_s - cp_s, &val) != RETVAL_OK)
                    {
                        TRACE();
                        return RETVAL_SYNTAX;
                    }
                    if(!val)
                    {
                        condition = CONDITION_FALSE;
                        level_by_pass = level_next;
                    }
                    else
                    {
                        condition = CONDITION_TRUE;
                        level_by_pass = MAX;
                    }
                }
                TRACESTR(g_dbg_str[DBG_CONDITION][condition]);
                // TRACESTRINT("level_crr",level_crr); TRACESTRINT("level_next",level_next); TRACESTRINT("level_by_pass",level_by_pass);
                prev_state = STATEMENT_IF;
                state = STATEMENT_INIT;
            break;

            case STATEMENT_ELSE: ;
                int level_tmp = level_next;
                syntax_state = STATEMENT_ELSE;
                if(g_dbg_run_case) TRACESTR(g_dbg_str[DBG_STATEMENT][syntax_state]);
                if(condition == CONDITION_INIT)
                {
                    TRACE();
                    return RETVAL_SYNTAX;
                }
                prog_s = prog_s + sizeof("else") - 1;
                if(prog_s >= prog_e)
                {
                    TRACE();
                    return RETVAL_SYNTAX;
                }
                level_next = level_crr+1;
                if (level_by_pass > level_crr)
                {
                    if(level_next == level_by_pass)
                    {
                        level_by_pass = MAX;
                    }
                    else
                    {
                        level_by_pass = level_next;
                    }
                }
                level_crr = level_tmp;
                prev_state = STATEMENT_ELSE;
                state = STATEMENT_INIT;
            break;

            case STATEMENT_CONTINUE:
                syntax_state = STATEMENT_CONTINUE;
                if(g_dbg_run_case) TRACESTR(g_dbg_str[DBG_STATEMENT][syntax_state]);
                if(level_by_pass <= level_next)
                {
                    prog_s = prog_s + sizeof("continue") - 1;
                    state = STATEMENT_INIT;
                    break;
                }
                if(pop_ptr(&sub_prog_s_ptr_st, &sub_prog_s_ptr) != RETVAL_OK)
                {
                    TRACE();
                    return RETVAL_SYNTAX;
                }
                prog_s = sub_prog_s_ptr;
                pop_ptr(&run2_s_ptr_st, &run2_s_ptr);
                pop_int(&run2_size_st, &run2_size);
                pop_ptr(&run3_s_ptr_st, &run3_s_ptr);
                pop_int(&run3_size_st, &run3_size);
                pop_int(&state_st, &state);
                pop_int(&loop_level_st, &level_next);
                level_crr = level_next;
                while(top_int(&brk_st) >= level_crr && size_int(&brk_st))
                {
                    pop_int(&brk_st, NULL);
                }
                // if(top_int(&brk_st) == level_crr)
                // {
                //     pop_int(&brk_st, NULL);
                // }

            break;

            case STATEMENT_BREAK:
                syntax_state = STATEMENT_BREAK;
                if(g_dbg_run_case) TRACESTR(g_dbg_str[DBG_STATEMENT][syntax_state]);
                if(level_by_pass > level_next)
                {
                    if(pop_int(&loop_level_st, &level_by_pass) != RETVAL_OK)
                    {
                        TRACE();
                        return RETVAL_SYNTAX;
                    }
                    level_by_pass++;
                    pop_ptr(&run2_s_ptr_st, NULL);
                    pop_int(&run2_size_st, NULL);
                    pop_ptr(&run3_s_ptr_st, NULL);
                    pop_int(&run3_size_st, NULL);
                    pop_ptr(&sub_prog_s_ptr_st, NULL);
                    pop_int(&state_st, NULL);
                }
                // TRACESTRINT("level_crr",level_crr); TRACESTRINT("level_next",level_next); TRACESTRINT("level_by_pass",level_by_pass);

                // level_crr = level_next;
                // while(top_int(&brk_st) >= level_crr && size_int(&brk_st))
                // {
                //     pop_int(&brk_st, NULL);
                // }
                prog_s++;
                state = STATEMENT_INIT;
            break;

            case STATEMENT_SEMICOLON:
                state = STATEMENT_INIT;
                syntax_state = STATEMENT_SEMICOLON;
                if(g_dbg_run_case) TRACESTR(g_dbg_str[DBG_STATEMENT][syntax_state]);
                // if(syntax_state != STATEMENT_EXECUTION && syntax_state != STATEMENT_COMMA)
                // {
                //     TRACE();
                //     return RETVAL_SYNTAX;
                // }
                if(prev_state == STATEMENT_IF
                    || prev_state == STATEMENT_ELSE
                    || prev_state == STATEMENT_WHILE || prev_state == STATEMENT_FOR)
                {
                    if(size_int(&brk_st))
                    {
                        level_next = top_int(&brk_st) + 1;
                    }
                    else
                    {
                        level_next = 0;
                    }
                    level_crr--;
                    if(prev_state == STATEMENT_ELSE)
                    {
                        level_crr--;
                    }
                    else if(prev_state == STATEMENT_WHILE || prev_state == STATEMENT_FOR)
                    {
                        pop_ptr(&run2_s_ptr_st, &run2_s_ptr);
                        pop_int(&run2_size_st, &run2_size);
                        pop_ptr(&run3_s_ptr_st, &run3_s_ptr);
                        pop_int(&run3_size_st, &run3_size);
                        pop_ptr(&sub_prog_s_ptr_st, &sub_prog_s_ptr);
                        pop_int(&state_st, &state);
                        pop_int(&loop_level_st, &level_next);
                        prog_s = sub_prog_s_ptr ? (sub_prog_s_ptr - 1) : prog_s;
                    }
                }
                prev_state = STATEMENT_SEMICOLON;
                prog_s++;
            break;

            case STATEMENT_COMMA:
                syntax_state = STATEMENT_COMMA;
                if(g_dbg_run_case) TRACESTR(g_dbg_str[DBG_STATEMENT][syntax_state]);
                state = STATEMENT_INIT;
                prog_s++;
            break;

            case STATEMENT_OPEN_BRK:
                syntax_state = STATEMENT_OPEN_BRK;
                if(g_dbg_run_case) TRACESTR(g_dbg_str[DBG_STATEMENT][syntax_state]);
                prev_state = STATEMENT_OPEN_BRK;
                state = STATEMENT_INIT;
                push_int(&brk_st, level_crr);
                prog_s++;
            break;

            case STATEMENT_CLOSE_BRK:
                syntax_state = STATEMENT_CLOSE_BRK;
                if(g_dbg_run_case) TRACESTR(g_dbg_str[DBG_STATEMENT][syntax_state]);
                if(pop_int(&brk_st, &level_next) != RETVAL_OK)
                {
                    TRACE();
                    RETVAL_SYNTAX;
                }
                level_crr = level_next;
                prev_state = STATEMENT_CLOSE_BRK;
                state = STATEMENT_INIT;
                if(size_int(&loop_level_st) && (level_next == top_int(&loop_level_st)))
                {
                    pop_ptr(&run2_s_ptr_st, &run2_s_ptr);
                    pop_int(&run2_size_st, &run2_size);
                    pop_ptr(&run3_s_ptr_st, &run3_s_ptr);
                    pop_int(&run3_size_st, &run3_size);
                    pop_ptr(&sub_prog_s_ptr_st, &sub_prog_s_ptr);
                    pop_int(&loop_level_st, NULL);
                    pop_int(&state_st, &state);
                    prog_s = sub_prog_s_ptr ? (sub_prog_s_ptr - 1) : prog_s;
                }
                prog_s++;
            break;

            case STATEMENT_EXECUTION:
                switch(syntax_state)
                {
                    case STATEMENT_EXECUTION:
                        TRACE();
                        return RETVAL_SYNTAX;
                    break;

                    default:
                    break;
                }
                syntax_state = STATEMENT_EXECUTION;
                if(g_dbg_run_case) TRACESTR(g_dbg_str[DBG_STATEMENT][syntax_state]);
                // TRACESTRINT("level_crr",level_crr); TRACESTRINT("level_next",level_next); TRACESTRINT("level_by_pass",level_by_pass);
                if(prog_s == prog_e - 1)
                {
                    TRACE();
                    goto finished;
                }
                level_crr = level_next;
                if(level_by_pass > level_crr)
                {
                    level_by_pass = MAX;
                }
                cp_s = prog_s;
                while((*prog_s != ';') && (prog_s < prog_e))
                {
                    if(level_by_pass > level_crr)
                    {
                        while((*prog_s != ',') && (*prog_s != ';') && (prog_s < prog_e))
                        {
                            prog_s++;
                        }
                        if(statement_execution(cp_s, prog_s - cp_s, NULL) != RETVAL_OK)
                        {
                            TRACE();
                            return RETVAL_SYNTAX;
                        }
                        if(prog_s < prog_e)
                        {
                            cp_s = prog_s+1;
                        }
                    }
                    else
                    {
                        prog_s++;
                    }
                }
                state = STATEMENT_INIT;
            break;

            default:
                prog_s++;
            break;
        }
    }
finished:
    if(size_int(&state_st) || size_int(&brk_st) || size_int(&loop_level_st) || size_ptr(&run2_s_ptr_st) || size_int(&run2_size_st) || size_ptr(&run3_s_ptr_st) || size_int(&run3_size_st) || size_ptr(&sub_prog_s_ptr_st))
    {
        TRACESTRINT("state_st", size_int(&state_st));
        TRACESTRINT("brk_st", size_int(&brk_st));
        TRACESTRINT("loop_level_st", size_int(&loop_level_st));
        TRACESTRINT("run2_s_ptr_st", size_ptr(&run2_s_ptr_st));
        TRACESTRINT("run2_size_st", size_int(&run2_size_st));
        TRACESTRINT("run3_s_ptr_st", size_ptr(&run3_s_ptr_st));
        TRACESTRINT("run3_size_st", size_int(&run3_size_st));
        TRACESTRINT("sub_prog_s_ptr_st", size_ptr(&sub_prog_s_ptr_st));
        return RETVAL_SYNTAX;
    }

    return RETVAL_OK;
}

int statement_execution(char * const expr, int size, int *rs)
{
    char *symbol;
    char *sym_s;
    char *sym_e;
    char * infix_e = expr + size;
    int ope;
    int prev_push;
    int open;
    int r_val, l_val, tmp_ope;
    char *cp_s,*cp_e;
    int val, ret_val;
    int_stack_t var_st, val_st, ope_st;
    char input[size+1];
    memcpy(input, expr, size);
    input[size] = 0;

    if(g_dbg_st_exe)
    {
        TRACESTR(input);
    }

    if(!size)
    {
        TRACE();
        return RETVAL_SYNTAX;
    }
    memset(&ope_st, 0, sizeof(int_stack_t));
    memset(&val_st, 0, sizeof(int_stack_t));
    memset(&var_st, 0, sizeof(int_stack_t));
    push_int(&ope_st, OPE_DEFAULT);
    prev_push = PUSH_OPERATOR;
    open = 0;
    sym_s = expr;
    sym_e = infix_e;

    for(symbol=sym_s; symbol<sym_e;symbol++)
    {
        if(is_white_space(*symbol))
        {
            continue;
        }

        ope = is_operator(&symbol);
        if(g_dbg && g_dbg_st_exe_case)
        {
            memcpy(input, symbol, sym_e - symbol);
            input[sym_e - symbol] = 0;
            TRACESTR(input);
        }

        switch(ope)
        {
            case OPE_ERR:
                if(prev_push != PUSH_OPERATOR)
                {
                    TRACE();
                    return RETVAL_SYNTAX;
                }
                cp_s = symbol;
                cp_e = sym_e;
                val = 0;
                // is primitive value, variable, function calling. */
                switch(check_symbol(symbol))
                {
                    case SYM_PRIMITIVE:
                        if(get_primitive_val(&cp_s, cp_e - cp_s, &val) != RETVAL_OK)
                        {
                            TRACE();
                            return RETVAL_SYNTAX;
                        }
                    break;

                    case SYM_VARIABLE:
                        ;
                        int index;
                        if(*cp_s>='a' && *cp_s<='z')
                        {
                            index = *cp_s-'a';
                            val = variables[index];
                        }
                        else
                        {
                            index = *cp_s-'A';
                            val = variables[index];
                        }
                        push_int(&var_st, (index));
                    break;

                    case SYM_FUNCTION: ;
                        int i, j, called, arg;
                        called = 0;
                        for (i=0; i<FUNC1_NUM; i++)
                        {
                            const struct Function1 *f;
                            f = func1+i;
                            for(j=0; !is_white_space(*(cp_s+j)) && (*(cp_s+j) == *(f->name+j)); j++);
                            if(j == f->len)
                            {
                                char *cp_tmp;
                                called = 1;
                                while(*cp_s != '(')
                                {
                                    cp_s++;
                                }
                                cp_tmp = cp_s;
                                walk_through_parenthesis(&cp_s, sym_e);
                                TRACESTR(cp_s);HR;
                                statement_execution(cp_tmp, cp_s - cp_tmp, &arg);
                                val = (*f->func)(arg);
                                cp_s++;
                            }
                        }
                        if(!called)
                        {
                            TRACE();
                            return RETVAL_NOT_SUPPORTED;
                        }
                    break;

                    case SYM_ERROR:
                        TRACE();
                        return RETVAL_SYNTAX;
                    break;

                    default:
                    break;
                }
                symbol = cp_s;
                push_int(&val_st, val);
                prev_push = PUSH_VARIABLE;
            break;

            case OPE_CLOSE_PAREN:
                if(prev_push == PUSH_OPERATOR)
                {
                    TRACE();
                    return RETVAL_SYNTAX;
                }
                // see ')'
                // while not match '('
                // pop right, left hand value, operator in stack then calculate them.
                while(top_int(&ope_st) != OPE_OPEN_PAREN)
                {
                    // if no more val or operator in stack, syntax error.
                    // pop right-hand value & operator.
                    if((pop_int(&val_st, &r_val) != RETVAL_OK) || (pop_int(&ope_st, &tmp_ope) != RETVAL_OK))
                    {
                        TRACE();
                        return RETVAL_SYNTAX;
                    }
                    // operator !
                    if(tmp_ope == OPE_DIF_Z)
                    {
                        val = !r_val;
                    }
                    else if(tmp_ope == OPE_EQU)
                    {
                        if(pop_int(&var_st, &l_val) != RETVAL_OK)
                        {
                            TRACE();
                            return RETVAL_SYNTAX;
                        }
                        variables[l_val] = r_val;
                        pop_int(&val_st, NULL);
                    }
                    else
                    {
                        // pop left-hand value.
                        if(pop_int(&val_st, &l_val) != RETVAL_OK)
                        {
                            TRACE();
                            return RETVAL_SYNTAX;
                        }
                        // calculate them.
                        val = calculate_operator(l_val, r_val, tmp_ope);
                    }
                    // push value which was calculated to value stack.
                    push_int(&val_st, val);
                    prev_push = PUSH_VARIABLE;
                }
                open--;
                // pop '('
                // if close parenthesis are more than open, return error syntax.
                if((pop_int(&ope_st, NULL) != RETVAL_OK) || (open < 0))
                {
                    TRACE();
                    return RETVAL_SYNTAX;
                }
            break;

            case OPE_OPEN_PAREN:
                // check syntax
                if(prev_push == PUSH_VARIABLE)
                {
                    TRACE();
                    return RETVAL_SYNTAX;
                }
                open++;
                push_int(&ope_st, ope);
                prev_push = PUSH_OPERATOR;
            break;

            // operator !
            case OPE_DIF_Z:
                push_int(&ope_st, ope);
                prev_push = PUSH_OPERATOR;
            break;

            // operator =
            case OPE_EQU:
                push_int(&ope_st, ope);
                prev_push = PUSH_OPERATOR;
            break;

            // calculate operator.
            case OPE_D_EQU:
            case OPE_DIF:
            case OPE_D_OR:
            case OPE_D_AND:
            case OPE_LES_TH:
            case OPE_GRE_TH:
            case OPE_LES_EQU:
            case OPE_GRE_EQU:
            case OPE_LEF_SH:
            case OPE_RIG_SH:
            case OPE_XOR:
            case OPE_AND:
            case OPE_OR:
            case OPE_ADD:
            case OPE_SUB:
            case OPE_MUL:
            case OPE_DIV:
            case OPE_MOD:
                if(prev_push != PUSH_VARIABLE)
                {
                    TRACE();
                    return RETVAL_SYNTAX;
                }
                // if see operator has greater priority than stack's top, just push them to stack.
                if(prio_operator(ope) > prio_operator(top_int(&ope_st)))
                {
                    push_int(&ope_st, ope);
                    prev_push = PUSH_OPERATOR;
                }
                else
                {
                    // see lesser priority, pop all greater prio operator in stack and process them.
                    while(prio_operator(ope) <= prio_operator(top_int(&ope_st)))
                    {
                        // if no more val or operator in stack, syntax error.
                        // pop right-hand value & operator.
                        if((pop_int(&val_st, &r_val) == RETVAL_EMPTY) || (pop_int(&ope_st, &tmp_ope) == RETVAL_EMPTY))
                        {
                            TRACE();
                            return RETVAL_SYNTAX;
                        }
                        // operator !
                        if(tmp_ope == OPE_DIF_Z)
                        {
                            val = !r_val;
                        }
                        else if(tmp_ope == OPE_EQU)
                        {
                            if(pop_int(&var_st, &l_val) != RETVAL_OK)
                            {
                                TRACE();
                                return RETVAL_SYNTAX;
                            }
                            variables[l_val] = r_val;
                            pop_int(&val_st, NULL);
                        }
                        else
                        {
                            // pop left-hand value.
                            if(pop_int(&val_st, &l_val) == RETVAL_EMPTY)
                            {
                                TRACE();
                                return RETVAL_SYNTAX;
                            }
                            // calculate them.
                            val = calculate_operator(l_val, r_val, tmp_ope);
                        }
                        // push value which was calculated to value stack.
                        push_int(&val_st, val);
                        prev_push = PUSH_VARIABLE;
                    }
                    push_int(&ope_st, ope);
                    prev_push = PUSH_OPERATOR;
                }
            break;
        }
    }//end of for.

    if(open != 0)
    {
        TRACEINT(open);
        return RETVAL_SYNTAX;
    }

    while(top_int(&ope_st) != OPE_DEFAULT)
    {
        if((pop_int(&val_st, &r_val) == RETVAL_EMPTY) || (pop_int(&ope_st, &tmp_ope) == RETVAL_EMPTY))
        {
            TRACE();
            return RETVAL_SYNTAX;
        }
        if(tmp_ope == OPE_DIF_Z)
        {
            val = !r_val;
        }
        else if(tmp_ope == OPE_EQU)
        {
            if(pop_int(&var_st, &l_val) != RETVAL_OK)
            {
                TRACE();
                return RETVAL_SYNTAX;
            }
            variables[l_val] = r_val;
            pop_int(&val_st, NULL);
        }
        else
        {
            if(pop_int(&val_st, &l_val) == RETVAL_EMPTY)
            {
                TRACE();
                return RETVAL_SYNTAX;
            }
            val = calculate_operator(l_val, r_val, tmp_ope);
        }
        push_int(&val_st, val);
        prev_push = PUSH_VARIABLE;
    }

    // pop out result and default operator.
    if((pop_int(&val_st, rs) != RETVAL_OK) || (pop_int(&ope_st, NULL) != RETVAL_OK))
    {
        TRACE();
        return RETVAL_SYNTAX;
    }

    // if there're any elements in stack, return error syntax.
    if(!is_empty(&val_st) || !is_empty(&ope_st))
    {
        TRACE();
        return RETVAL_SYNTAX;
    }

    return RETVAL_OK;
}
