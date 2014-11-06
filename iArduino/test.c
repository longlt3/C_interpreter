#include "resource.h"

// #define TEST1(arg)          if(arg==RETVAL_OK) printf("PASSED\n"); else printf("FAILED\n");
#define TEST(arg1, arg2)   if(arg1==arg2) printf("Passed\n"); else printf("FAILED\n");

#define ASSERT_RETURN(retval)   if(retval!=RETVAL_OK) { printf("FAILED\n"); return; }

// extern int statement_execution(char infix_s[], char infix_e[], int *rs);

void test_statement_execution_error_syntax(char *ch, int size)
{
    char *cp_test_s, *cp_test_e;
    printf("Case: %-30s : ",ch);
    cp_test_s = ch, cp_test_e = cp_test_s + size;
    TEST(statement_execution(cp_test_s, cp_test_e, NULL), RETVAL_SYNTAX);
}

void test_statement_execution_ok(char *ch, int size, int val)
{
    char *cp_test_s, *cp_test_e;
    int value;
    printf("Case: %-30s : ",ch);
    cp_test_s = ch, cp_test_e = cp_test_s + size;
    ASSERT_RETURN(statement_execution(cp_test_s, cp_test_e, &value));
    TEST(value, val);
}


void test1()
{
    char *ch;
    int size;
    HR;printf("Testing syntax error.\n");HR;
    ch = "1()";
    size = sizeof("1()");
    test_statement_execution_error_syntax(ch, size);
    ch = "1(0)";
    size = sizeof("1(0)");
    test_statement_execution_error_syntax(ch, size);
    ch = "+ (0)";
    size = sizeof("+ (0)");
    test_statement_execution_error_syntax(ch, size);
    ch = "1 + (0";
    size = sizeof("1 + (0");
    test_statement_execution_error_syntax(ch, size);
    ch = "1 (+ 0)";
    size = sizeof("1 (+ 0)");
    test_statement_execution_error_syntax(ch, size);
    ch = "1 + ()2";
    size = sizeof("1 + ()2");
    test_statement_execution_error_syntax(ch, size);

    HR;printf("Testing operators.\n");HR;
    ch = "2 ^ 3";
    size = sizeof("2 ^ 3");
    test_statement_execution_ok(ch, size, 2 ^ 3);
    ch = "2 & 3";
    size = sizeof("2 & 3");
    test_statement_execution_ok(ch, size, 2 & 3);
    ch = "2 | 3";
    size = sizeof("2 | 3");
    test_statement_execution_ok(ch, size, 2 | 3);
    ch = "!1!=0";
    size = sizeof("!1!=0");
    test_statement_execution_ok(ch, size, !1!=0);
    ch = "0b101 << 3 == 0xF0 >> 2";
    size = sizeof("0b101 << 3 == 0xF0 >> 2");
    test_statement_execution_ok(ch, size, 0b101 << 3 == 0xF0 >> 2);
    ch = "1 < 2 || 2 > 3";
    size = sizeof("1 < 2 || 2 > 3");
    test_statement_execution_ok(ch, size, 1 < 2 || 2 > 3);
    ch = "2 != 3 && 3 - 3";
    size = sizeof("2 != 3 && 3 - 3");
    test_statement_execution_ok(ch, size, 2 != 3 && 3 - 3);

    HR;printf("Testing complex expression.\n");HR;
    ch = "((1 + 2) * 3 + 4 * (5 + 6)) * 7";
    size = sizeof("((1 + 2) * 3 + 4 * (5 + 6)) * 7");
    test_statement_execution_ok(ch, size, ((1 + 2) * 3 + 4 * (5 + 6)) * 7);
}
