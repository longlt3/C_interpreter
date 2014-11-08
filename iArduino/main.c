
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "resource.h"

extern void test1();

extern char * g_dbg_str[DBG_MAX][DBG_STR_MAX];
extern char g_test[];
extern char g_test_2[];

int main()
{
    // char expr[] = "1 + print  (  print   (10) )";
    int rs;
    char expr[] = "1 + ()2";
    // char prog[] = "if(0) if(0) print(1); else print(2); else if(1) print(3); else print(4); print(100);";
    // char prog[] = "if(1)if(0)if(1)print(1);else print(2);else print(3);else if(1)print(4);else print(5);print(10);";
    // char prog[] = "print(100); if(1) if(1) print(1); else { if(0) {print(3);} else {print(4);print(5);} print(200); } print(300); ";
    // char prog[] = "if(1) if(1) { print(1); } print(0xFF);";
    // char prog[] = "while((i=i+1)<10) { if(i>5) { continue; }  print(i); } print(i);";
    // char prog[] = "while((i=i+1)<3) { j=10; while((j=j-1)>3) { if(j-i>5) { continue; } } } print(100);";
    char prog[] = "while((i=i+1)<3) { j=10; while((j=j-1)>3) { if(j-i>5) { continue; } else { print(i+j); print(j); print(i); } } } print(100);";

    char *expr_e = expr + sizeof(expr);
    int val;
    // test1();
    // print_symbol(expr);
    // rs = statement_executino(expr, expr_e, &val);
    // print_ret_val(rs);
    // TRACEINT(val);
    // TRACEINT(1+(2));
    // TRACEINT(ope_st.top);
    TRACESTR(prog);
    rs = run(prog, sizeof(prog));
    TRACESTR(g_dbg_str[DBG_RET_VAL][rs]);
    // TRACE();
    int i=0, j=0;
    // if(0) if(0) print_2(1); else print_2(2); else if(1) print_2(3); else print_2(4); print_2(100);
    // if(1)if(0)if(1)print_2(1);else print_2(2);else print_2(3);else if(1)print_2(4);else print_2(5);print_2(10);
    // print_2(100); if(1) if(1) print_2(1); else { if(0) {print_2(3);} else {print_2(4);print_2(5);} print_2(200); } print_2(300);
    // if(1) if(1) { print_2(1); } print_2(0xFF);
    // while((i=i+1)<10) { if(i>5) { continue; } print_2(i); } print_2(i);
    // while((i=i+1)<3) { j=10; while((j=j-1)>3) { if(j-i>5) { continue; } } } print_2(100);
    while((i=i+1)<3) { j=10; while((j=j-1)>3) { if(j-i>5) { continue; } else { print_2(i+j); print_2(j); print_2(i); } } } print_2(100);

    HR;printf("Output:\n%s\n", g_test);
    HR;printf("Expected:\n%s\n", g_test_2);
    rs = strcmp(g_test, g_test_2);
    if(rs)
    {
        HR;printf("__FAILED!\n");HR;
    }
    else
    {
        HR;printf("Passed!\n");HR;
    }
    return 0;
}

