#ifndef TEST_H
#define TEST_H

#include "resource.h"

#define TEST_STR_MAX         0x2000

extern int g_test;
extern char g_test_1[];
extern char g_test_2[];
extern int g_test_size_1;
extern int g_test_size_2;
extern int g_test_failed;
extern int g_test_total_case;

void test_expr(void);
void test_if_else(void);
void test_while(void);
void test_for(void);

#endif
