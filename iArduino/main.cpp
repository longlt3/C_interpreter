
#include <stdio.h>

// #include<conio.h>

#include <string.h>

#define TRACE(x)              printf("\n%s : %d\r\n",__FUNCTION__,__LINE__)
#define TRACEINT(x)           printf("\n%s : %d : %d\r\n",__FUNCTION__,__LINE__,x)
#define TRACESTR(x)           printf("\n%s : %d : %s\r\n",__FUNCTION__,__LINE__,x)
#define TRACEADD(x)           printf("\n%s : %d : %p\r\n",__FUNCTION__,__LINE__,x)
#define HR                    printf("------------------------------------------\n");
enum RETVAL
{
    RETVAL_OK,
    RETVAL_SYNTAX,
    RETVAL_NULL,
    RETVAL_EMPTY,
    RETVAL_FULL,
    RETVAL_NOT_SUPPORTED,
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

int print(int val)
{
    printf("%d\n", val);
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

int is_white_space(char ch);

int is_empty(int_stack_t *st);

int get_size(int_stack_t *st);

int get_top(int_stack_t *st);

int pop(int_stack_t *st, int *elem);

int push(int_stack_t *st, int elem);

int prio_operator(int ope);

int calculate_operator(int l_operant, int r_operant, int ope);

// int assignment_operator(int l_operant, int r_operant, int ope);

int get_primitive_val(char ** sym_s, char const*const sym_e, int *val);

int is_operator(char ** symbol);

int statement_executable(char infix_s[], char infix_e[], int *rs);

int check_symbol(char *sym_s);

void print_ret_val(int rs)
{
    switch(rs)
    {
        case RETVAL_OK:
            printf("RETVAL_OK\n");
        break;
        case RETVAL_SYNTAX:
            printf("RETVAL_SYNTAX\n");
        break;
        case RETVAL_NULL:
            printf("RETVAL_NULL\n");
        break;
        case RETVAL_EMPTY:
            printf("RETVAL_EMPTY\n");
        break;
        case RETVAL_FULL:
            printf("RETVAL_FULL\n");
        case RETVAL_NOT_SUPPORTED:
            printf("RETVAL_NOT_SUPPORTED\n");
        break;
    }
}

void print_symbol(char *symbol)
{
    int rs = check_symbol(symbol);
    switch(rs)
    {
        case SYM_PRIMITIVE:
            printf("SYM_PRIMITIVE\n");
        break;
        case SYM_VARIABLE:
            printf("SYM_VARIABLE\n");
        break;
        case SYM_FUNCTION:
            printf("SYM_FUNCTION\n");
        break;
        case SYM_ERROR:
            printf("SYM_ERROR\n");
        break;
    }
}

void print_ope(int ope)
{
    switch(ope)
    {
        case OPE_DEFAULT:
            printf("OPE_DEFAULT\n");
        break;
        case OPE_OPEN_PAREN:
            printf("OPE_OPEN_PAREN\n");
        break;
        case OPE_CLOSE_PAREN:
            printf("OPE_CLOSE_PAREN\n");
        break;
        case OPE_D_EQU:
            printf("OPE_D_EQU\n");
        break;
        case OPE_DIF:
            printf("OPE_DIF\n");
        break;
        case OPE_D_OR:
            printf("OPE_D_OR\n");
        break;
        case OPE_D_AND:
            printf("OPE_D_AND\n");
        break;
        case OPE_LES_TH:
            printf("OPE_LES_TH\n");
        break;
        case OPE_GRE_TH:
            printf("OPE_GRE_TH\n");
        break;
        case OPE_LES_EQU:
            printf("OPE_LES_EQU\n");
        break;
        case OPE_GRE_EQU:
            printf("OPE_GRE_EQU\n");
        break;
        case OPE_LEF_SH:
            printf("OPE_LEF_SH\n");
        break;
        case OPE_RIG_SH:
            printf("OPE_RIG_SH\n");
        break;
        case OPE_XOR:
            printf("OPE_XOR\n");
        break;
        case OPE_AND:
            printf("OPE_AND\n");
        break;
        case OPE_OR:
            printf("OPE_OR\n");
        break;
        case OPE_ADD:
            printf("OPE_ADD\n");
        break;
        case OPE_SUB:
            printf("OPE_SUB\n");
        break;
        case OPE_MUL:
            printf("OPE_MUL\n");
        break;
        case OPE_DIV:
            printf("OPE_DIV\n");
        break;
        case OPE_MOD:
            printf("OPE_MOD\n");
        break;
        case OPE_DIF_Z:
            printf("OPE_DIF_Z\n");
        break;
        case OPE_ERR:
            printf("OPE_ERR\n");
        break;

    }
}

void print_stack(int_stack_t *st)
{
    int i;
    for(i=st->top-1; i>=0; i--)
    {
        printf("%d ", st->elem[i]);
    }
    printf("\n");
}

void print_ope_stack(int_stack_t *st)
{
    int i;
    for(i=st->top-1; i>=0; i--)
    {
        print_ope(st->elem[i]);
    }
    printf("\n");
}

int main()
{
    // char sym_pri[] = "1", *sym_pri_e,
    //     sym_func[] = "asd", *sym_func_e,
    //     sym_var[] = "a", *sym_var_e
    //     ;
    // char expr[] = "((1+2) * 3 + 4 * (5+6)) * 7";
    char expr[] = "1 + print  (  print   (10) )";
    char *expr_e = expr + sizeof(expr);
    int val;
    // print_symbol(expr);
    int rs = statement_executable(expr, expr_e, &val);
    print_ret_val(rs);
    TRACEINT(val);
    // TRACEINT(1+(2));
    // TRACEINT(ope_st.top);
    return 0;
}

int is_white_space(char ch)
{
    return (ch == '\0') || (ch == '\n') || (ch == '\r') || (ch == '\t') || (ch == ' ');
}

int is_empty(int_stack_t *st)
{
    if(st)
    {
        return !st->top;
    }
    return -1;
}

int get_size(int_stack_t *st)
{
    return st->top;
}

int get_top(int_stack_t *st)
{
    if(!st)
    {
        return -1;
    }
    return st->elem[(st->top) - 1];
}

int pop(int_stack_t *st, int *elem)
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

int push(int_stack_t *st, int elem)
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

int get_primitive_val(char ** sym_s, char *const sym_e, int *val)
{
    char *cp_s;
    if(!sym_s || !(*sym_s) || !sym_e || !val)
    {
        return RETVAL_NULL;
    }
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

int statement_executable(char infix_s[], char infix_e[], int *rs)
{
    char *symbol, *sym_s, *sym_e;
    int ope;
    int prev_push;
    int open;
    int r_val, l_val, tmp_ope;
    char *cp_s,*cp_e;
    int val, ret_val;
    int_stack_t var_st, val_st, ope_st;

    memset(&ope_st, 0, sizeof(int_stack_t));
    memset(&val_st, 0, sizeof(int_stack_t));
    memset(&var_st, 0, sizeof(int_stack_t));
    push(&ope_st, OPE_DEFAULT);
    prev_push = PUSH_OPERATOR;

    open = 0;
    sym_s = infix_s;
    sym_e = infix_e;
    for(symbol=sym_s; symbol<sym_e;symbol++)
    {
        if(is_white_space(*symbol))
        {
            continue;
        }

        ope = is_operator(&symbol);

        TRACESTR(symbol);
        // print_stack(&val_st);
        // print_ope_stack(&ope_st);

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
                        if(get_primitive_val(&cp_s, cp_e, &val) != RETVAL_OK)
                        {
                            TRACE();
                            return RETVAL_SYNTAX;
                        }
                    break;

                    case SYM_VARIABLE:
                        if(*cp_s>='a' && *cp_s<='z')
                        {
                            val = variables[*cp_s-'a'];
                        }
                        else
                        {
                            val = variables[*cp_s-'A'];
                        }
                        push(&var_st, (*cp_s-'A'));
                    break;

                    case SYM_FUNCTION:
                        int i, j, called, arg;
                        const struct Function1 *f;
                        called = 0;
                        for (i=0; i<FUNC1_NUM; i++)
                        {
                            f = func1+i;
                            for(j=0; !is_white_space(*(cp_s+j)) && (*(cp_s+j) == *(f->name+j)); j++);
                            if(j == f->len)
                            {
                                char *cp_tmp;
                                called = 1;
                                while(*cp_s++ != '(');
                                cp_tmp = cp_s;
                                while(*cp_s++ != ')');
                                statement_executable(cp_tmp, cp_s - 1, &arg);
                                val = (*f->func)(arg);
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
                push(&val_st, val);
                prev_push = PUSH_VARIABLE;
            break;

            case OPE_CLOSE_PAREN:
                int count;
                count=0;
                // see ')'
                // while not match '('
                // pop right, left hand value, operator in stack then calculate them.
                while(get_top(&ope_st) != OPE_OPEN_PAREN)
                {
                    // if no more val or operator in stack, syntax error.
                    // pop right-hand value & operator.
                    if((pop(&val_st, &r_val) != RETVAL_OK) || (pop(&ope_st, &tmp_ope) != RETVAL_OK))
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
                        if(pop(&var_st, &l_val) != RETVAL_OK)
                        {
                            TRACE();
                            return RETVAL_SYNTAX;
                        }
                        variables[l_val] = r_val;
                        pop(&val_st, NULL);
                    }
                    else
                    {
                        // pop left-hand value.
                        if(pop(&val_st, &l_val) != RETVAL_OK)
                        {
                            TRACE();
                            return RETVAL_SYNTAX;
                        }
                        // calculate them.
                        val = calculate_operator(l_val, r_val, tmp_ope);
                    }
                    // push value which was calculated to value stack.
                    push(&val_st, val);
                    prev_push = PUSH_VARIABLE;
                }
                open--;
                // pop '('
                // if close parenthesis are more than open, return error syntax.
                if((pop(&ope_st, NULL) != RETVAL_OK) || (open < 0))
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
                push(&ope_st, ope);
                prev_push = PUSH_OPERATOR;
            break;

            // operator !
            case OPE_DIF_Z:
                push(&ope_st, ope);
                prev_push = PUSH_OPERATOR;
            break;

            // operator =
            case OPE_EQU:
                TRACE();
                push(&ope_st, ope);
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
                if(prio_operator(ope) > prio_operator(get_top(&ope_st)))
                {
                    push(&ope_st, ope);
                    prev_push = PUSH_OPERATOR;
                }
                else
                {
                    // see lesser priority, pop all greater prio operator in stack and process them.
                    while(prio_operator(ope) <= prio_operator(get_top(&ope_st)))
                    {
                        // if no more val or operator in stack, syntax error.
                        // pop right-hand value & operator.
                        if((pop(&val_st, &r_val) == RETVAL_EMPTY) || (pop(&ope_st, &tmp_ope) == RETVAL_EMPTY))
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
                            if(pop(&var_st, &l_val) != RETVAL_OK)
                            {
                                TRACE();
                                return RETVAL_SYNTAX;
                            }
                            variables[l_val] = r_val;
                            pop(&val_st, NULL);
                        }
                        else
                        {
                            // pop left-hand value.
                            if(pop(&val_st, &l_val) == RETVAL_EMPTY)
                            {
                                TRACE();
                                return RETVAL_SYNTAX;
                            }
                            // calculate them.
                            val = calculate_operator(l_val, r_val, tmp_ope);
                        }
                        // push value which was calculated to value stack.
                        push(&val_st, val);
                        prev_push = PUSH_VARIABLE;
                    }
                    push(&ope_st, ope);
                    prev_push = PUSH_OPERATOR;
                }
            break;
        }
    }//end of for.

    if(open != 0)
    {
        TRACE();
        return RETVAL_SYNTAX;
    }

    while(get_top(&ope_st) != OPE_DEFAULT)
    {
        if((pop(&val_st, &r_val) == RETVAL_EMPTY) || (pop(&ope_st, &tmp_ope) == RETVAL_EMPTY))
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
            if(pop(&var_st, &l_val) != RETVAL_OK)
            {
                TRACE();
                return RETVAL_SYNTAX;
            }
            variables[l_val] = r_val;
            pop(&val_st, NULL);
        }
        else
        {
            if(pop(&val_st, &l_val) == RETVAL_EMPTY)
            {
                TRACE();
                return RETVAL_SYNTAX;
            }
            val = calculate_operator(l_val, r_val, tmp_ope);
        }
        push(&val_st, val);
        prev_push = PUSH_VARIABLE;
    }

    // pop out result and default operator.
    if((pop(&val_st, rs) != RETVAL_OK) || (pop(&ope_st, NULL) != RETVAL_OK))
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
