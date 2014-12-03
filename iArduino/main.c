
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "resource.h"
#include "debug.h"
#include "test.h"

#define BUFFER_SIZE             0x800

void setup()
{
    char in_buf[BUFFER_SIZE+1];
    char prog_buf[BUFFER_SIZE+1];
    int result = -1;
    int prog_buf_size;
    char *des;
    memset(in_buf, 0, sizeof(in_buf));
    memset(prog_buf, 0, sizeof(prog_buf));
    getsS(in_buf, BUFFER_SIZE);
    TRACE();
    des = in_buf;
    while(is_white_space(*des) && des != 0)
    {
        des++;
    }

    if(des == 0 || *des == 0)
    {

    }
    else if (strncmp(des, "prog", 4) == 0)
    {
        des = prog_buf;
        while((des - prog_buf) < BUFFER_SIZE)
        {
            getsS(in_buf, BUFFER_SIZE);
            if(strncmp(in_buf, "end", 3) == 0)
            {
                break;
            }
            strcpy(des, in_buf);
            des = prog_buf + strlen(prog_buf);
            if((des - prog_buf) >= BUFFER_SIZE)
            {
                printf("program is too large.\n");
                result = RETVAL_SYNTAX;
                des = prog_buf;
                break;
            }
        }
        printf("Output:\n");
        result = run(prog_buf, strlen(prog_buf));
    }
    else
    {
        TRACESTR(des);
        if(*des == '\0')
        {
            printf("des: %s",des);
            printf("%d", '\0');
        }
        result = statement_execution(des, strlen(des), NULL);
        // printf("\nUnrecognized!\n\n");
    }

    if(result == -1)
    {

    }
    else if(result == RETVAL_OK)
    {
        printf("\nSuccessful!\n\n");
    }
    else
    {
        printf("\nError!\n\n");
    }
    // printf("%s.\n",g_dbg_str[DBG_RET_VAL][result]);
}

int main()
{
    // char expr[] = "1 + print  (  print   (10) )";
    int rs;
    int val;
    int i=0, j=0;
    // char *expr_e = expr + sizeof(expr);
    // char expr[] = "1 + ()2";
#ifdef TCI_TEST
    g_test_size_1 = g_test_size_2 = 0;
    g_test = 0;
#endif

#ifdef TCI_DEBUG
    g_dbg = 1;
    g_dbg_run = 0;
    g_dbg_run_case = 0;
    g_dbg_st_exe = 0;
    g_dbg_st_exe_case = 0;
#endif
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

    // char prog[] = "while((i=i+1)<10000) { j=10; while((j=j-1)>1) { if(j-i>5) { continue; } else { print(i+j); print(j); print(i); } } } print(100);";
    // while((i=i+1)<10000) { j=10; while((j=j-1)>1) { while((i=i+1)<10000) { if(i%2)if(i%3)if(i%5) print(i);else print(i+100);else print(i+1000);else if(i%7)print(i+200);else print(i+2000); print(9999); } } } print(100);


    // char prog[] = "while((i=i+1)<30) { if(i>10) { print(0xFF); break; }  print(i); } print(i);";
    // while((i=i+1)<30) { if(i>10) { print(0xFF); break; } print(i); } print(i);

    // char prog[] = "for(i=0;i<13;i=i+1) print(i);";
    // for(i=0;i<13;i=i+1) print(i);

    // char prog[] = "print(i); for(i=0;i<30;i=i+1) { for(j=0;j<13;j=j+1) { if(j>10) { print(j); break; } print(i); } } print(100);";
    // print(i); for(i=0;i<30;i=i+1) { for(j=0;j<13;j=j+1) { if(j>10) { print(j); break; } print(i); } } print(100);

    // char prog[] = "i=10, print(i);";
    // i=10, print(i);

    // char prog[] = "for(i=0;i<2;i=i+1){ if(i%2)if(i%3)if(i%5) print(i);else print(i+100);else print(i+1000);else if(i%7)print(i+200);else print(i+2000); } print(9999);";
    // for(i=0;i<2;i=i+1){ if(i%2)if(i%3)if(i%5) print(i);else print(i+100);else print(i+1000);else if(i%7)print(i+200);else print(i+2000); } print(9999);
#ifdef TCI_TEST
    // test_expr();
    // test_if_else();
    // test_while();
    // test_for();
    // printf("Failed: %d on %d Test case.\n", g_test_failed, g_test_total_case);
#endif
    // print_symbol(expr);
    // rs = statement_executino(expr, expr_e, &val);
    // print_ret_val(rs);
    // TRACEINT(val);
    // TRACEINT(1+(2));
    // TRACEINT(ope_st.top);

    // TRACESTR(prog);
    // g_test = 1;
    // rs = run(prog, sizeof(prog));
    // TRACESTR(g_dbg_str[DBG_RET_VAL][rs]);

    // HR;printf("Output:\n%s\n", g_test_1);
    // HR;printf("Expected:\n%s\n", g_test_2);
    // rs = strcmp(g_test_1, g_test_2);
    // if(rs)
    // {
    //     HR;printf("__FAILED!\n");HR;
    // }
    // else
    // {
    //     HR;printf("Passed!\n");HR;
    // }
    g_prog_run = 1;
    HR;printf("Tiny C language interpreter\n");HR;
    while(1)
    {
        setup();
    }
    return 0;
}

