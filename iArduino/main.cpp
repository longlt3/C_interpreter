
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
    OPE_OPEN_PAREN,
    OPE_CLOSE_PAREN,

    OPE_D_EQU,
    OPE_DIF,
    OPE_D_OR,
    OPE_D_AND,

    OPE_LES_TH,
    OPE_GRE_TH,
    OPE_LES_EQU,
    OPE_GRE_EQU,

    OPE_LEF_SH,
    OPE_RIG_SH,
    OPE_XOR,
    OPE_AND,
    OPE_OR,

    OPE_ADD,
    OPE_SUB,

    OPE_MUL,
    OPE_DIV,
    OPE_MOD,

    OPE_DIF_Z,

    OPE_ERR,
};

#define MAX 20

typedef struct _int_stack_t
{
    int elem[MAX];
    int top;
} int_stack_t;

int_stack_t ope_st;
int_stack_t val_st;

// int top=-1;

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

int is_white_space(char ch);

int get_top(int_stack_t *const st);

int pop(int_stack_t *const st, int *const elem);

int push(int_stack_t *const st, int elem);

int prcd(int ope);

int calculate_operator(int l_operant, int r_operant, int ope);

int isoperator(char ** symbol);

int convertip(char infix_s[], char infix_e[], int *const rs);

int check_symbol(char *const sym_s, char *const sym_e);

int main()
{

    char sym_pri[] = "1", *sym_pri_e,
        sym_func[] = "asd", *sym_func_e,
        sym_var[] = "a", *sym_var_e
        ;
    char expr[] = "0b1011 ^ 0b0100";
    char *expr_e = expr + sizeof(expr);
    int val;
    int rs = convertip(expr, expr_e, &val);
    print_ret_val(rs);
    TRACEINT(val);

    return 0;
}

int is_white_space(char ch)
{
    return (ch == '\0') || (ch == '\n') || (ch == '\r') || (ch == '\t') || (ch == ' ');
}

int get_top(int_stack_t *const st)
{
    if(!st)
    {
        return -1;
    }
    return st->elem[(st->top) - 1];
}

int pop(int_stack_t *const st, int *const elem)
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

int push(int_stack_t *const st, int elem)
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

int prcd(int ope)
{
    int ret = 0;
    switch(ope)
    {
        case OPE_D_EQU:
        case OPE_DIF:
        case OPE_D_OR:
        case OPE_D_AND:
            ret = 2;
        break;

        case OPE_LES_TH:
        case OPE_GRE_TH:
        case OPE_LES_EQU:
        case OPE_GRE_EQU:
            ret = 3;
        break;

        case OPE_LEF_SH:
        case OPE_RIG_SH:
        case OPE_XOR:
        case OPE_AND:
        case OPE_OR:
            ret = 4;
        break;

        case OPE_ADD:
        case OPE_SUB:
            ret = 5;
        break;

        case OPE_MUL:
        case OPE_DIV:
        case OPE_MOD:
            ret = 6;
        break;

        case OPE_DIF_Z:
            ret = 7;
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

// int isoperator(char *symbol, int size)
int isoperator(char ** symbol)
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

int check_symbol(char *const sym_s, char*const sym_e)
{
    char *cp_s, *cp_e;
    if(!sym_s || !sym_e)
    {
        return SYM_ERROR;
    }
    cp_s = sym_s;
    cp_e = sym_e;
    if(cp_e == '\0')
    {
        cp_e--;
    }
    if(*cp_s >= '0' && *cp_s <= '9')
    {
        return SYM_PRIMITIVE;
    }
    else if((*cp_s >= 'a' && *cp_s <= 'z') || (*cp_s >= 'A' && *cp_s <= 'Z'))
    {
        if(cp_s == cp_e)
        {
            return SYM_VARIABLE;
        }
        else
        {
            return SYM_FUNCTION;
        }
    }
    return SYM_ERROR;
}

int convertip(char infix_s[], char infix_e[], int *const rs)
{
    int i, j=0;
    char *symbol, *sym_s, *sym_e;
    // ope_st[++top]='#';
    memset(&ope_st, 0, sizeof(ope_st));
    memset(&val_st, 0, sizeof(val_st));
    push(&ope_st, OPE_DEFAULT);
    int ope = OPE_ERR;

    // int rs = 0;

    int el;

    int r_val, l_val, tmp_ope;
    char *cp_s,*cp_e;
    int val;

    sym_s = infix_s;
    sym_e = infix_e;
    for(symbol=sym_s; symbol<sym_e;symbol++)
    {
        if(is_white_space(*symbol))
        {
            continue;
        }
        ope = isoperator(&symbol);

        // TRACESTR(symbol);

        switch(ope)
        {
            case OPE_ERR:
                cp_s = symbol;
                cp_e = sym_e;
                val = 0;
                // is primitive value, variable, function calling. */
                if(*cp_s >= '0' && *cp_s <= '9')
                {
                    // primitive type
                    if(*cp_s == '0')
                    {
                        // binary or hexadecimal format.
                        if((*(cp_s+1) != 'x') && (*(cp_s+1) != 'b'))
                        {
                            // val is zero.
                            cp_s++;
                        }
                        else if(*(cp_s+1)=='x')
                        {
                            // hexadecimal format
                            // check syntax
                            if(!(cp_s+1) || !(cp_s+2) || (cp_s+2)>=cp_e)
                            {
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
                                return RETVAL_SYNTAX;
                            }
                            while((cp_s < cp_e) &&
                                (((*cp_s>='0') && (*cp_s<='9')) ||
                                ((*cp_s>='A') && (*cp_s<='F')) ||
                                ((*cp_s>='a') && (*cp_s<='f')))
                                )
                            {
                                val <<= 4;
                                if (*cp_s>='0' && *cp_s<='9')
                                {
                                    val += (*cp_s - '0');
                                }
                                else if (*cp_s>='a' && *cp_s<='f')
                                {
                                    val += (*cp_s - 'a' + 0xa);
                                }
                                else if (*cp_s>='A' && *cp_s<='F')
                                {
                                    val += (*cp_s - 'A' + 0xa);
                                }
                                cp_s++;
                            }
                        }
                        else
                        {
                            // binary format.
                            if(!((cp_s < cp_e) && (*cp_s>='0') && (*cp_s<='1')))
                            {
                                return RETVAL_SYNTAX;
                            }
                            // check syntax
                            if(!(cp_s+1) || !(cp_s+2) || (cp_s+2)>=cp_e)
                            {
                                return RETVAL_SYNTAX;
                            }
                            // walk through b
                            cp_s += 2;
                            while((cp_s < cp_e) && (*cp_s>='0') && (*cp_s<='1'))
                            {
                                val = (val<<1) + (*cp_s - '0');
                                cp_s++;
                            }
                        }
                    }
                    else
                    {
                        // decimal format.
                        while((cp_s < cp_e) && (*cp_s>='0') && (*cp_s<='9'))
                        {
                            val *= 10;
                            val += (*cp_s - '0');
                            cp_s++;
                        }
                    }
                }
                else if((*cp_s >= 'a' && *cp_s <= 'z') || (*cp_s >= 'A' && *cp_s <= 'Z'))
                {
                    // is variable or function.
                    if((cp_s+1) != cp_e)
                    {
                        // is variable
                    }
                }
                symbol = cp_s - 1;
                push(&val_st, val);
            break;

            case OPE_CLOSE_PAREN:
                // see ')'
                // while not match '('
                // pop right, left hand value, operator in stack then calculate them.
                while(get_top(&ope_st) != OPE_OPEN_PAREN)
                {
                    // if no more val or operator in stack, syntax error.
                    // pop right-hand value & operator.
                    if((pop(&val_st, &r_val) == RETVAL_EMPTY) || (pop(&ope_st, &tmp_ope) == RETVAL_EMPTY))
                    {
                        return RETVAL_SYNTAX;
                    }
                    // operator !
                    if(tmp_ope == OPE_DIF_Z)
                    {
                        val = !r_val;
                    }
                    else
                    {
                        // pop left-hand value.
                        if(pop(&val_st, &l_val) == RETVAL_EMPTY)
                        {
                            return RETVAL_SYNTAX;
                        }
                        // calculate them.
                        val = calculate_operator(l_val, r_val, tmp_ope);
                    }
                    // push value which was calculated to value stack.
                    push(&val_st, val);
                }
                // pop '('
                if(pop(&ope_st, NULL) == RETVAL_EMPTY)
                {
                    return RETVAL_SYNTAX;
                }
            break;

            default:
                // if see operator has greater priority than stack's top, just push them to stack.
                if(prcd(ope) > prcd(get_top(&ope_st)))
                {
                    push(&ope_st, ope);
                }
                else
                {
                    // see lesser priority, pop all greater prio operator in stack and process them.
                    while(prcd(ope) <= prcd(get_top(&ope_st)))
                    {
                        // if no more val or operator in stack, syntax error.
                        // pop right-hand value & operator.
                        if((pop(&val_st, &r_val) == RETVAL_EMPTY) || (pop(&ope_st, &tmp_ope) == RETVAL_EMPTY))
                        {
                            return RETVAL_SYNTAX;
                        }
                        // operator !
                        if(tmp_ope == OPE_DIF_Z)
                        {
                            val = !r_val;
                        }
                        else
                        {
                            // pop left-hand value.
                            if(pop(&val_st, &l_val) == RETVAL_EMPTY)
                            {
                                return RETVAL_SYNTAX;
                            }
                            // calculate them.
                            val = calculate_operator(l_val, r_val, tmp_ope);
                        }
                        // push value which was calculated to value stack.
                        push(&val_st, val);
                    }
                }
            break;

            case OPE_DIF_Z:
            case OPE_OPEN_PAREN:
                push(&ope_st, ope);
            break;
        }
    }//end of for.

    while(get_top(&ope_st) != OPE_DEFAULT)
    {
        if((pop(&val_st, &r_val) == RETVAL_EMPTY) || (pop(&ope_st, &tmp_ope) == RETVAL_EMPTY))
        {
            return RETVAL_SYNTAX;
        }
        if(tmp_ope == OPE_DIF_Z)
        {
            val = !r_val;
        }
        else
        {
            if(pop(&val_st, &l_val) == RETVAL_EMPTY)
            {
                return RETVAL_SYNTAX;
            }
            val = calculate_operator(l_val, r_val, tmp_ope);
        }
        push(&val_st, val);
    }

    if(pop(&val_st, rs) == RETVAL_EMPTY)
    {
        return RETVAL_SYNTAX;
    }
    return RETVAL_OK;
}
