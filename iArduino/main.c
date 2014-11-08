
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "resource.h"

int main()
{
    // char expr[] = "1 + print  (  print   (10) )";
    int rs;
    int val;
    int i=0, j=0;
    // char *expr_e = expr + sizeof(expr);
    // char expr[] = "1 + ()2";

    g_test = 0;
    g_dbg = 0;
    g_dbg_run = 0;
    g_dbg_run_case = 0;
    g_dbg_st_exe = 0;
    g_dbg_st_exe_case = 0;

    // char prog[] = "if(0) if(0) print(1); else print(2); else if(1) print(3); else print(4); print(100);";
    // if(0) if(0) print(1); else print(2); else if(1) print(3); else print(4); print(100);

    // char prog[] = "if(1)if(0)if(1)print(1);else print(2);else print(3);else if(1)print(4);else print(5);print(10);";
    // if(1)if(0)if(1)print(1);else print(2);else print(3);else if(1)print(4);else print(5);print(10);

    // char prog[] = "print(100); if(1) if(1) print(1); else { if(0) {print(3);} else {print(4);print(5);} print(200); } print(300); ";
    // print(100); if(1) if(1) print(1); else { if(0) {print(3);} else {print(4);print(5);} print(200); } print(300);

    // char prog[] = "if(1) if(1) { print(1); } print(0xFF);";
    // if(1) if(1) { print(1); } print(0xFF);

    // char prog[] = "while((i=i+1)<3) { print(i); } print(i);";
    // while((i=i+1)<3) { print(i); } print(i);

    // char prog[] = "while((i=i+1)<30) { if(i>10) { continue; }  print(i); } print(i);";
    // while((i=i+1)<30) { if(i>10) { continue; } print(i); } print(i);

    // char prog[] = "print(i); while((i=i+1)<10) { j=0; while((j=j+1)<15) print(i); } print(100);";
    // print(i); while((i=i+1)<10) { j=0; while((j=j+1)<15) print(i); } print(100);

    // char prog[] = "print(i); while((i=i+1)<30) { j=0;  while((j=j+1)<30) { print(j); } } print(100);";
    // print(i); while((i=i+1)<30) { j=0;  while((j=j+1)<30) { print(j); } } print(100);

    // char prog[] = "print(i); while((i=i+1)<10) { j=0; while((j=j+1)<30) { if(i>1) { print(i);} } } print(100);";
    // print(i); while((i=i+1)<10) { j=0; while((j=j+1)<30) { if(i>1) { print(i);} } } print(100);

    // char prog[] = "print(i); while((i=i+1)<30) { while((j=j+1)<3) { if(j<2) { print(j); continue; } print(i); } } print(100);";
    // print(i); while((i=i+1)<30) { while((j=j+1)<3) { if(j<2) { print(j); continue; } print(i); } } print(100);

    char prog[] = "while((i=i+1)<10000) { j=10; while((j=j-1)>1) { if(j-i>5) { continue; } else { print(i+j); print(j); print(i); } } } print(100);";
    while((i=i+1)<10000) { j=10; while((j=j-1)>1) { if(j-i>5) { continue; } else { print(i+j); print(j); print(i); } } } print(100);


    // test1();
    // print_symbol(expr);
    // rs = statement_executino(expr, expr_e, &val);
    // print_ret_val(rs);
    // TRACEINT(val);
    // TRACEINT(1+(2));
    // TRACEINT(ope_st.top);

    TRACESTR(prog);
    g_test = 1;
    rs = run(prog, sizeof(prog));
    TRACESTR(g_dbg_str[DBG_RET_VAL][rs]);

    HR;printf("Output:\n%s\n", g_test_1);
    HR;printf("Expected:\n%s\n", g_test_2);
    rs = strcmp(g_test_1, g_test_2);
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

