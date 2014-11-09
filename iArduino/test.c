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
    TEST(statement_execution(cp_test_s, size, NULL), RETVAL_SYNTAX);
}

void test_statement_execution_ok(char *ch, int size, int val)
{
    char *cp_test_s, *cp_test_e;
    int value;
    printf("Case: %-30s : ",ch);
    cp_test_s = ch, cp_test_e = cp_test_s + size;
    ASSERT_RETURN(statement_execution(cp_test_s, size, &value));
    TEST(value, val);
}


void test1()
{
    char prog[2048];
    int rs=0, size=0;
    int i,j;

    HR;printf("Testing expression.\n");HR;
    printf("print(((1 + 2) * 3 + 4 * (5 + 6)) * 7);\n\n");
    size = sizeof("print(((1 + 2) * 3 + 4 * (5 + 6)) * 7);");
    memcpy(prog, "print(((1 + 2) * 3 + 4 * (5 + 6)) * 7);", size);
    g_test = 0;
    print(((1 + 2) * 3 + 4 * (5 + 6)) * 7);
    g_test_size_1 = g_test_size_2 = 0;
    g_test = 1;
    rs = run(prog, size);
    if(rs)
    {
        printf("__FAILED!\n");HR;
    }
    else
    {
        // printf("Output: %s\n", g_test_1);
        rs = strcmp(g_test_1, g_test_2);
        if(rs)
        {
            printf("Expected:\n%s\n", g_test_2);
            printf("__FAILED!\n");HR;
        }
        else
        {
            printf("Passed!\n");HR;
        }
    }
/*----------------------------------------------------------------------------*/

    HR;printf("Testing if else statement.\n");HR;

    printf("if(1)if(0)if(1)print(1);else print(2);else print(3);else if(1)print(4);else print(5);print(10);\n\n");
    size = sizeof("if(1)if(0)if(1)print(1);else print(2);else print(3);else if(1)print(4);else print(5);print(10);");
    memcpy(prog, "if(1)if(0)if(1)print(1);else print(2);else print(3);else if(1)print(4);else print(5);print(10);", size);
    g_test = 0;
    if(1)if(0)if(1)print(1);else print(2);else print(3);else if(1)print(4);else print(5);print(10);
    g_test_size_1 = g_test_size_2 = 0;
    g_test = 1;
    rs = run(prog, size);
    if(rs)
    {
        printf("__FAILED!\n");HR;
    }
    else
    {
        // printf("Output: %s\n", g_test_1);
        rs = strcmp(g_test_1, g_test_2);
        if(rs)
        {
            printf("Expected:\n%s\n", g_test_2);
            printf("__FAILED!\n");HR;
        }
        else
        {
            printf("Passed!\n");HR;
        }
    }

    printf("print(100); if(1) if(1) print(1); else { if(0) {print(3);} else {print(4);print(5);} print(200); } print(300);\n\n");
    size = sizeof("print(100); if(1) if(1) print(1); else { if(0) {print(3);} else {print(4);print(5);} print(200); } print(300);");
    memcpy(prog, "print(100); if(1) if(1) print(1); else { if(0) {print(3);} else {print(4);print(5);} print(200); } print(300);", size);
    g_test = 0;
    print(100); if(1) if(1) print(1); else { if(0) {print(3);} else {print(4);print(5);} print(200); } print(300);
    g_test_size_1 = g_test_size_2 = 0;
    g_test = 1;
    rs = run(prog, size);
    if(rs)
    {
        printf("__FAILED!\n");HR;
    }
    else
    {
        // printf("Output: %s\n", g_test_1);
        rs = strcmp(g_test_1, g_test_2);
        if(rs)
        {
            printf("Expected:\n%s\n", g_test_2);
            printf("__FAILED!\n");HR;
        }
        else
        {
            printf("Passed!\n");HR;
        }
    }
/*----------------------------------------------------------------------------*/

    HR;printf("Testing while - continue - break statement.\n");HR;

    printf("print(i); while((i=i+1)<10) { j=0; while((j=j+1)<30) { if(i>1) { print(i);} } } print(100);\n\n");
    size = sizeof("print(i); while((i=i+1)<10) { j=0; while((j=j+1)<30) { if(i>1) { print(i);} } } print(100);");
    memcpy(prog, "print(i); while((i=i+1)<10) { j=0; while((j=j+1)<30) { if(i>1) { print(i);} } } print(100);", size);
    g_test = 0;
    print(i); while((i=i+1)<10) { j=0; while((j=j+1)<30) { if(i>1) { print(i);} } } print(100);
    g_test_size_1 = g_test_size_2 = 0;
    g_test = 1;
    rs = run(prog, size);
    if(rs)
    {
        printf("__FAILED!\n");HR;
    }
    else
    {
        // printf("Output: %s\n", g_test_1);
        rs = strcmp(g_test_1, g_test_2);
        if(rs)
        {
            printf("Expected:\n%s\n", g_test_2);
            printf("__FAILED!\n");HR;
        }
        else
        {
            printf("Passed!\n");HR;
        }
    }

    printf("while((i=i+1)<50) { j=10; while((j=j-1)>1) { if(j-i>5) { continue; } else { print(i+j); print(j); print(i); } } } print(100);\n\n");
    size = sizeof("while((i=i+1)<50) { j=10; while((j=j-1)>1) { if(j-i>5) { continue; } else { print(i+j); print(j); print(i); } } } print(100);");
    memcpy(prog, "while((i=i+1)<50) { j=10; while((j=j-1)>1) { if(j-i>5) { continue; } else { print(i+j); print(j); print(i); } } } print(100);", size);
    g_test = 0;
    while((i=i+1)<50) { j=10; while((j=j-1)>1) { if(j-i>5) { continue; } else { print(i+j); print(j); print(i); } } } print(100);
    g_test_size_1 = g_test_size_2 = 0;
    g_test = 1;
    rs = run(prog, size);
    if(rs)
    {
        printf("__FAILED!\n");HR;
    }
    else
    {
        // printf("Output: %s\n", g_test_1);
        rs = strcmp(g_test_1, g_test_2);
        if(rs)
        {
            printf("Expected:\n%s\n", g_test_2);
            printf("__FAILED!\n");HR;
        }
        else
        {
            printf("Passed!\n");HR;
        }
    }

    printf("while((i=i+1)<30) { if(i>10) { print(0xFF); break; } print(i); } print(i);\n\n");
    size = sizeof("while((i=i+1)<30) { if(i>10) { print(0xFF); break; } print(i); } print(i);");
    memcpy(prog, "while((i=i+1)<30) { if(i>10) { print(0xFF); break; } print(i); } print(i);", size);
    g_test = 0;
    while((i=i+1)<30) { if(i>10) { print(0xFF); break; } print(i); } print(i);
    g_test_size_1 = g_test_size_2 = 0;
    g_test = 1;
    rs = run(prog, size);
    if(rs)
    {
        printf("__FAILED!\n");HR;
    }
    else
    {
        // printf("Output: %s\n", g_test_1);
        rs = strcmp(g_test_1, g_test_2);
        if(rs)
        {
            printf("Expected:\n%s\n", g_test_2);
            printf("__FAILED!\n");HR;
        }
        else
        {
            printf("Passed!\n");HR;
        }
    }
/*----------------------------------------------------------------------------*/

    HR;printf("Testing for - continue - break statement.\n");HR;

    printf("print(i); for(i=0;i<30;i=i+1) { for(j=0;j<13;j=j+1) { if(j>10) { print(j); break; } print(i); } } print(100);\n\n");
    size = sizeof("print(i); for(i=0;i<30;i=i+1) { for(j=0;j<13;j=j+1) { if(j>10) { print(j); break; } print(i); } } print(100);");
    memcpy(prog, "print(i); for(i=0;i<30;i=i+1) { for(j=0;j<13;j=j+1) { if(j>10) { print(j); break; } print(i); } } print(100);", size);
    g_test = 0;
    print(i); for(i=0;i<30;i=i+1) { for(j=0;j<13;j=j+1) { if(j>10) { print(j); break; } print(i); } } print(100);
    g_test_size_1 = g_test_size_2 = 0;
    g_test = 1;
    rs = run(prog, size);
    if(rs)
    {
        printf("__FAILED!\n");HR;
    }
    else
    {
        // printf("Output: %s\n", g_test_1);
        rs = strcmp(g_test_1, g_test_2);
        if(rs)
        {
            printf("Expected:\n%s\n", g_test_2);
            printf("__FAILED!\n");HR;
        }
        else
        {
            printf("Passed!\n");HR;
        }
    }

    printf("for(i=0;i<20;i=i+1) { for(j=0;j<20;j=j+1) { if(j-i>5) { continue; } else { print(i+j); print(j); print(i); } } } print(100);\n\n");
    size = sizeof("for(i=0;i<20;i=i+1) { for(j=0;j<20;j=j+1) { if(j-i>5) { continue; } else { print(i+j); print(j); print(i); } } } print(100);");
    memcpy(prog, "for(i=0;i<20;i=i+1) { for(j=0;j<20;j=j+1) { if(j-i>5) { continue; } else { print(i+j); print(j); print(i); } } } print(100);", size);
    g_test = 0;
    for(i=0;i<20;i=i+1) { for(j=0;j<20;j=j+1) { if(j-i>5) { continue; } else { print(i+j); print(j); print(i); } } } print(100);
    g_test_size_1 = g_test_size_2 = 0;
    g_test = 1;
    rs = run(prog, size);
    if(rs)
    {
        printf("__FAILED!\n");HR;
    }
    else
    {
        // printf("Output: %s\n", g_test_1);
        rs = strcmp(g_test_1, g_test_2);
        if(rs)
        {
            printf("Expected:\n%s\n", g_test_2);
            printf("__FAILED!\n");HR;
        }
        else
        {
            printf("Passed!\n");HR;
        }
    }
}
