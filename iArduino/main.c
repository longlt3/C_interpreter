
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "resource.h"

extern void test1();

extern char*g_dgb_ret_val[];

int main()
{
    // char expr[] = "1 + print  (  print   (10) )";
    int rs;
    char expr[] = "1 + ()2";
    // char prog[] = "for(i=0; i<10; i++){if(i<2) continue; print(i);}";
    // char prog[] = "i=10,print(i),i=100,print(i); print(1^3)";
    // char prog[] = "if(0) if(0) print(1); else print(2); else if(1) print(3); else print(4); print(100); ";
    // char prog[] = "if(1)if(0)if(1)print(1);else print(2);else print(3);else if(1)print(4);else print(5);print(10);";
    // char prog[] = "print(100); if(1) if(1) print(1); else { if(0) {print(3);} else {print(4);print(5);} print(200); } print(300); ";
    // char prog[] = "if(1) if(1) { print(1); } print(0xFF);";
    // char prog[] = "if(1) { print(0xAA); } else { print(0xFF); }";
    char prog[] = "while((i=i+1)<10)if(1)print(i);print(100);";
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
    TRACESTR(g_dgb_ret_val[rs]);
    // TRACE();

    return 0;
}

