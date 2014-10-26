// A C interpreter for Arduino
// Copyright(c) 2012 Noriaki Mitsunaga
// $Id: iArduino.ino,v 1.4 2012/02/13 11:15:23 kurobox Exp kurobox $
// See http://n.mtng.org/ele/arduino/iarduino-j.html (in Japanese)

#define INPUT_LEN 80   // Size of input buffer

int runAnimate = 0;
int runDelay = 0;
int runStep = 0;

//char progbuf[1000] = "a=1;b=1;\r\nc=1;\r\na=a+b+c;\r\nprint(a);\r\nprint(b);\r\n";
//char progbuf[1000] = "a=0;while(a<5)a=a+1;print(a);\r\na=5;while(a>0) {print(a);a=a-1;}\r\n";
//char progbuf[1000] = "if(a>b)print(10);else print(-10);";
//char progbuf[1000] = "if(a>b)print(10); print(-10);";
//char progbuf[1000] = "print(i);for(i=0;i<5;i=i+1)print(i);";
char progbuf[1000] = "print(i);for(i=0;i<5;i=i+1){print(i);if(i>2)break; print(0);} print(i);\r\n";
//char progbuf[800] = "for(i=0;;i=i+1){print(i);}";
// 999 ????(????1022??, EEPROM????2?????????????????)
char *prog;

#ifdef ARDUINO
#include <EEPROM.h>
#include <Servo.h>

Servo servo[12];
const int SERVO_NUM = 12;

void setup()
{
     Serial.begin(38400);
     Serial.println(F("Tiny C language interpreter for Arduino"));
     Serial.println(F("(c) 2012 N.M."));

     prog = progbuf;

     if (EEPROM.read(0x3ff) == 'P') {
          ReadProg();
          if (EEPROM.read(0x3fe) == 'R') {
               int err;

               Serial.println(F("Starting program"));
               runStep = 0;
               runAnimate = 0;
               runDelay = 0;
               if ((err = run(prog, strlen(prog))) != 0)
               printError(err);
          }
     }
     Serial.println(F("OK"));
}
#else
#include <stdio.h>
// #include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// #include <conio.h>
void printSln(const char *s);
void printS(const char *s);
#define getcharS() getchar()

#define TRACE(x)              printf("%s : %d\r\n",__FUNCTION__,__LINE__)
#define TRACEINT(x)           printf("%s : %d : %d\r\n",__FUNCTION__,__LINE__,x)
#define TRACESTR(x)           printf("%s : %d : %s\r\n",__FUNCTION__,__LINE__,x)
#endif

void edit(char *prog, int lNum, char *line);
int  eval2(char *s, int len, int *val);
void getsS(char *buf, size_t len);
void printError(int err);
int  run(char *prog, int len);
char *skipSpace(char *s, char *e);
void list(char *prog);

void loop()
{
     int err, v;
     char buf[INPUT_LEN+1];
     char *p;

     getsS(buf, INPUT_LEN);
     p = buf + strlen(buf) -1;
     *p = 0; /* remove \r or \n */
     p = skipSpace(buf, p);
     if (*p == 0)
     return;
     if (strncmp(buf, "animate", 7) == 0) {
          runStep = 0;
          runAnimate = 1;
          runDelay = 500;
          if ((err = run(prog, strlen(prog))) != 0)
          printError(err);
     } else if (strncmp(buf, "autorun", 7) == 0) {
#ifdef ARDUINO
          EEPROM.write(0x3fe, 'R');
#endif
     } else if (strncmp(buf, "debug", 5) == 0) {
          runStep = 0;
          runAnimate = 1;
          runDelay = 0;
          if ((err = run(prog, strlen(prog))) != 0)
          printError(err);
     } else if (strncmp(buf, "edit", 4) == 0) {
          int l = atoi(buf+4);
          getsS(buf, INPUT_LEN);
          edit(prog, l, buf);
     } else if (strncmp(buf, "list", 4) == 0) {
          list(prog);
     } else if (strncmp(buf, "noauto", 6) == 0) {
#ifdef ARDUINO
          EEPROM.write(0x3fe, 0);
#endif
     }  else if (strncmp(buf, "prog", 4) == 0) {
          int i;
          char *p;
          prog = NULL;
          p = progbuf;
          for (i=0; i<sizeof(progbuf); i = p - progbuf) {
               //fprintf(stderr, "%d: %d\n", i, sizeof(progbuf));
               getsS(buf, INPUT_LEN);
               if (strncmp(buf, "end", 3) == 0) {
                    *p = 0;
                    prog = progbuf;
                    break;
               }
               if (p-progbuf + strlen(buf) > sizeof(progbuf)-1) {
                    break;
               }
               strcpy(p, buf);
               p = progbuf + strlen(progbuf);
          }
          if (prog == NULL) {
               printSln("Program is too large");
               progbuf[0] = 0;
               prog = progbuf;
          }
     } else if (strncmp(buf, "run", 3) == 0) {
          runStep = 0;
          runAnimate = 0;
          runDelay = 0;
          if ((err = run(prog, strlen(prog))) != 0)
          printError(err);
     } else if (strncmp(buf, "step", 4) == 0) {
          runStep = 1;
          if ((err = run(prog, strlen(prog))) != 0)
          printError(err);
     } else if (strncmp(buf, "save", 4) == 0) {
#ifdef ARDUINO
          SaveProg();
#endif
     } else {
          if ((err = eval2(buf, strlen(buf), &v)) != 0)
          {
               printError(err);
          }
          else
          {
               TRACESTR(buf);
               printf("%d",v);
          }
     }
     printSln("\nOK");
}

//////////////////////////////////////////////////////////////////////////////
// Definitions of enums & structs
//////////////////////////////////////////////////////////////////////////////
enum ERRORS {
     STOPPED = 10,
     FOUND_CONTINUE = 1,
     FOUND_BREAK = 2,
     ERROR_DIV0 = -1,
     ERROR_SYNTAX = -2,
     ERROR_INTERNAL = -3
};

struct ArduinoConstValues {
     char *name;
     int   len;
     int   val;
};

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

//////////////////////////////////////////////////////////////////////////////
// Test and wrapper functions
//////////////////////////////////////////////////////////////////////////////
#ifndef ARDUINO
// Test functions
int rand_(int a)
{
     return rand();
}

int plus(int a, int b)
{
     return a+b;
}

int print(int a)
{
     printf("%d\n", a);
     return a;
}
#else // ARDUINO //
// Wrapper functions
int rand(int)
{
     return random(0, 32767);
}

int analogRead_(int pin)
{
     return analogRead(pin);
}

int delay_(int ms)
{
     delay(ms);
     return 0;
}

int digitalRead_(int pin)
{
     return digitalRead(pin);
}

int plus(int a, int b)
{
     return a+b;
}

int analogWrite_(int pin, int val)
{
     analogWrite(pin, val);
     return 0;
}

int digitalWrite_(int pin, int val)
{
     digitalWrite(pin, val);
     return 0;
}

int millis_(int dummy)
{
     return millis();
}

int min_(int a, int b)
{
     return min(a, b);
}

int max_(int a, int b)
{
     return max(a, b);
}

int noTone_(int pin)
{
     noTone(pin);
     return 0;
}

int pinMode_(int pin, int mode)
{
     pinMode(pin, mode);
     return 0;
}

int print(int a)
{
     Serial.println(a);
     return a;
}

int tone2(int pin, int freq)
{
     tone(pin, freq);
     return 0;
}

int tone3(int pin, int freq, int duration)
{
     tone(pin, freq, duration);
     return 0;
}
#endif

//////////////////////////////////////////////////////////////////////////////
// Global variables
//////////////////////////////////////////////////////////////////////////////
int variables[26] = {0};

const struct ArduinoConstValues constants[] = {
     {"LOW", 3, 0},
     {"HIGH", 4, 1},
     {"false", 5, 0},
     {"true", 4, 1},
     {"INPUT", 5, 0},
     {"OUTPUT", 6, 1},
};
#define CONST_NUM 6

// ----- Function tables -----
#ifndef ARDUINO
const struct Function1 func1[] = {
     {"rand(", 5, rand_},
     {"print(", 6, print},
};
#define FUNC1_NUM 2

const struct Function2 func2[] = {
     {"plus(", 5, plus},
};
#define FUNC2_NUM 1
#else /* ARDUINO */
const struct Function1 func1[] = {
     {"abs(", 4, abs},
     {"analogRead(", 11, analogRead_},
     {"delay(", 6, delay_},
     {"digitalRead(", 12, digitalRead_},
     {"millis(", 7, millis_},
     {"noTone(", 7, noTone_},
     {"rand(", 5, rand},
     {"print(", 6, print},
     //  {"sq(", 3, sq},
     //  {"sqrt(", 5, sqrt},
};
#define FUNC1_NUM 7

const struct Function2 func2[] = {
     {"analogWrite(", 12, analogWrite_},
     {"digitalWrite(", 13, digitalWrite_},
     {"min(", 4, min_},
     {"max(", 4, max_},
     {"pinMode(", 8, pinMode_},
     {"tone(", 5, tone2},
     //  {"plus(", 5, plus},
};
#define FUNC2_NUM 6

const struct Function3 func3[] = {
     {"tone3(", 5, tone3}
};
#define FUNC3_NUM 1
#endif

///////////////////////////////////////////////////////////////////////////
// Fucntion declarations
///////////////////////////////////////////////////////////////////////////
#ifdef ARDUINO
int kbhit()
{
     return Serial.available();
}
int getch()
{
     return Serial.read();
}
void Sleep(int s)
{
     delay(s);
}
#endif
int  eval2(char *s, int len, int *val);
void printError(int err);
int stepRun();

///////////////////////////////////////////////////////////////////////////
// Fucntions
///////////////////////////////////////////////////////////////////////////
#ifndef ARDUINO
void getsS(char *buf, size_t len)
{
     fgets(buf, len, stdin);
     buf[len+1] = 0;
}
void printSln(const char *s)
{
     // fprintf(stderr, "%s\n", s);
     printf("%s\n",s);
}
void printS(const char *s)
{
     fprintf(stderr, "%s", s);
}
void putcS(char c)
{
     fputc(c, stderr);
}
void writeS(const char *s, int len)
{
     fwrite(s, 1, len, stderr);
}
#else
byte getcharS()
{
     byte c;
     while (Serial.available() == 0)
     ;
     c = Serial.read();
     Serial.write(c);
     return c;
}
void getsS(char *buf, size_t len)
{
     static char prev = 0;
     char c, *p;

     p = buf;
     do {
          if (Serial.available() > 0) {
               c = Serial.read();
               if (p == buf && c == '\n' && prev == '\r') {
                    prev = c = 0;
                    continue;
               }
               if (c == 8 /* backspace*/) {
                    p --;
                    Serial.write(8);
                    Serial.write(' ');
                    Serial.write(8);
                    if (p<buf)
                    p = buf;
               } else {
                    Serial.write(c);
                    *p = c;
                    p ++;
               }
          }
     } while (c != '\n' && c!= '\r' && c != 3 && (p-buf)<len);
     prev = c;
     if (c == '\r') {
          *(p-1) = '\n';
          Serial.write('\n');
     } else if (c == '\n') {
          Serial.write('\r');
     }
     *p = 0;
}
void printS(const char *s)
{
     Serial.print(s);
}
void printS(const uint8_t *s)
{
     Serial.print((const char *)s);
}
void printSln(const char *s)
{
     Serial.println(s);
}
void printSln(const uint8_t *s)
{
     Serial.println((const char *)s);
}
void putcS(char c)
{
     Serial.write(c);
}
void writeS(const char *s, int len)
{
     Serial.write((uint8_t *)s, len);
}
void ReadProg()
{
     int i;
     for (i=0; i<(sizeof(progbuf)-1); i++) {
          progbuf[i] = EEPROM.read(i);
          if (progbuf[i] == 0)
          break;
     }
     if (progbuf[i] != 0)
     progbuf[i] = 0;
}
void SaveProg()
{
     int len = strlen(prog);

     for (int i=0; i<=len; i++) {
          EEPROM.write(i, prog[i]);
     }
     EEPROM.write(0x3ff, 'P'); // Magic number
}
#endif

int stepRun()
{
     TRACE();
     char buf[INPUT_LEN+1], *p;
     int err, v;

     if (runStep == 0) {
          char c;

          if (runDelay>0)
          usleep(runDelay);
          // if (kbhit() == 0)
          // return 0;
          c = getchar();
          if (c != 3) /* Control-c */
          printSln("Paused");
          else
          return STOPPED;
          runStep = 1;
     }
     for (;;) {
          putcS('>');
          getsS(buf, INPUT_LEN);
          if (buf[0] == 3)
          return STOPPED;
          p = buf + strlen(buf) -1;
          *p = 0; /* remove \r or \n */

          if (buf[0] == 0)
          break;
          if (strcmp(buf, "a") == 0 || strcmp(buf, "animate") == 0) {
               runStep = 0;
               runAnimate = 1;
               runDelay = 500;
               break;
          } else if (strcmp(buf, "c") == 0 || strcmp(buf, "continue") == 0) {
               runStep = 0;
               break;
          } else if (strcmp(buf, "d") == 0 || strcmp(buf, "debug") == 0) {
               runStep = 0;
               runAnimate = 1;
               runDelay = 0;
               break;
          } else if (strcmp(buf, "S") == 0) {
               return STOPPED;
          } else if (strcmp(buf, "r") == 0 || strcmp(buf, "run") == 0) {
               runStep = 0;
               runAnimate = 0;
               runDelay = 0;
               break;
          } else {
               p = skipSpace(buf, p);
               if ((err = eval2(p, strlen(p), &v)) != 0)
               printError(err);
               else
               print(v);
          }
     }
     return 0;
}

void printError(int err)
{
     switch (err)
     {
     case ERROR_DIV0:
#ifdef ARDUINO
          Serial.println(F("Divided by 0"));
#else
          printSln("Divided by 0");
#endif
          break;
     case ERROR_SYNTAX:
#ifdef ARDUINO
          Serial.println(F("Syntax error"));
#else
          printSln("Syntax error");
#endif
          break;
     case STOPPED:
#ifdef ARDUINO
          Serial.println(F("Stopped"));
#else
          printSln("Stopped");
#endif
          break;
     case ERROR_INTERNAL:
     default:
#ifdef ARDUINO
          Serial.println(F("Syntax error"));
#else
          printSln("Syntax error");
#endif
          //printSln("Intenal error");
          break;
     }
}

int
eval2(char *s, int len, int *val)
{
     int i, l;
     char *e;
     int v, varn = -1, err;

     //fwrite(s, 1, len, stderr);
     //fprintf(stderr, "\n");

     e = s + len;
     // Skip space
     while (*s != 0 && s<e && (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n'))
     s ++;

     if (*s == 0 || s == e) {
          *val = 1;
          return 0;
     }

     if (*s == '(') {
          // ( ??????
          char *p;
          int in = 1;

          s ++;
          while (*s != 0 && s<e && (*s == ' ' || *s == '\t'))
          s ++;
          if (*s == 0 || *s == ')')
          return ERROR_SYNTAX;

          p = s;
          while (*s != 0 && s<e) {
               if (*s == '(') {
                    in ++;
               } else if (*s == ')') {
                    in --;
                    if (in == 0) {
                         s ++;
                         break;
                    }
               }
               s ++;
          }
          if (in > 0)
          return ERROR_SYNTAX;
          if ((err = eval2(p, s-p-1, &v)) != 0)
          return err;
     } else if ((*s == '!' && *(s+1) != '=') || *s == '~' || *s == '-' || *s == '+') {
          // ?????1???????
          char *p, op;
          int in = 0;
          op = *s;
          s ++;
          while (*s != 0 && s<e && (*s == ' ' || *s == '\t'))
          s ++;
          if (*s == 0 || s == e)
          return ERROR_SYNTAX;
          p = s;
          while (*s != 0 && s<e && (in>0 || (*s != '+' && *s != '-' && *s != '>' && *s != '<' && *s != '='))) {
               if (*s == '(') {
                    in ++;
               } else if (*s == ')') {
                    in --;
               }
               s ++;
          }

          if (eval2(p, s-p, &v) != 0)
          return ERROR_SYNTAX;
          if (op == '!')
          v = !v;
          else if (op == '~')
          v = ~v;
          else if (op == '-')
          v = -v;
     } else if (*s>='0' && *s<='9') {
          // ??????????
          if (*s == '0' && *(s+1) == 'x') {
               v = 0;
               s += 2;
               while (*s != 0 && s<e && ((*s>='0' && *s<='9') ||
               (*s>='a' && *s<='f') || (*s>='A' && *s<='F'))) {
                    if (*s>='0' && *s<='9')
                    v = (v<<4) + (*s - '0');
                    else if (*s>='a' && *s<='f')
                    v = (v<<4) + (*s - 'a' + 0xa);
                    else if (*s>='A' && *s<='F')
                    v = (v<<4) + (*s - 'A' + 0xa);
                    s ++;
               }
          } else if (*s == '0' && *(s+1) == 'b') {
               v = 0;
               s += 2;
               while (*s != 0 && s<e && (*s>='0' && *s<='1')) {
                    v = (v<<1) + (*s - '0');
                    s ++;
               }
          } else {
               v = 0;
               while (*s != 0 && s<e && *s>='0' && *s<='9') {
                    v = v*10 + (*s - '0');
                    s ++;
               }
          }
     } else if (((*s>='a' && *s<='z') || (*s>='A' && *s<='Z')) &&
               (!(*(s+1)>='a' && *(s+1)<='z') && !(*(s+1)>='A' && *(s+1)<='Z')))  {
          // The token is a varibale
          if (*s>='a' && *s<='z')
          varn = *s - 'a';
          else
          varn = *s - 'A';
          v = variables[varn];
          s ++;
     } else {
          // ?????????????
          for (i=0; i<CONST_NUM; i++) {
               const struct ArduinoConstValues *c = constants+i;
               if (e-s>=c->len && strncmp(c->name, s, 3) == 0) {
                    v = c->val;
                    s += c->len;
                    break;
               }
          }
          // Check if the token is a supported function
          // ???1????
          for (i=0; i<FUNC1_NUM; i++) {
               const struct Function1 *f = func1+i;
               if (e-s>=f->len && strncmp(f->name, s, f->len) == 0) {
                    char *p;
                    int in = 1, v1;
                    s += f->len;
                    p = s;
                    while (*s != 0 && s<e && in>0) {
                         if (*s == '(') {
                              in ++;
                         } else if (*s == ')') {
                              in --;
                         }
                         s ++;
                    }
                    if (in>0)
                    return ERROR_SYNTAX;
                    if (eval2(p, s-p-1, &v1) != 0)
                    return ERROR_SYNTAX;

                    v = (*f->func)(v1);
               }
          }

          // servo?.attach(), servo?.write()
//          for (i=0; i<SERVO_NUM; i++) {
//               char f[20];
//               int attach_write = 0, len;
//               sprintf(f, "servo%d.attach(", i);
//               len = strlen(f);
//               if (e-s>=len && strncmp(f, s, len) == 0) {
//                    attach_write = 1;
//               } else {
//                    sprintf(f, "servo%d.write(", i);
//                    len = strlen(f);
//                    if (e-s>=len && strncmp(f, s, len) == 0) {
//                         attach_write = 2;
//                    }
//               }
//               if (attach_write > 0) {
//                    char *p;
//                    int in = 1, v1;
//                    s += len;
//                    p = s;
//                    while (*s != 0 && s<e && in>0) {
//                         if (*s == '(') {
//                              in ++;
//                         } else if (*s == ')') {
//                              in --;
//                         }
//                         s ++;
//                    }
//                    if (in>0)
//                    return ERROR_SYNTAX;
//                    if (eval2(p, s-p-1, &v1) != 0)
//                    return ERROR_SYNTAX;
//
//                    if (attach_write == 1)
//                    servo[i].attach(v1);
//                    else
//                    servo[i].write(v1);
//                    v = 0;
//                    break;
//               }
//          }

          // ???2????
          for (i=0; i<FUNC2_NUM; i++) {
               const struct Function2 *f = func2+i;
               if (e-s>=f->len && strncmp(f->name, s, f->len) == 0) {
                    char *p;
                    int in = 0, v1, v2;
                    s += f->len;
                    p = s;
                    while (*s != 0 && s<e && !(in == 0 && *s == ',')) {
                         if (*s == '(')
                         in ++;
                         else if (*s == ')')
                         in --;
                         s ++;
                    }
                    if (*s != ',')
                    return ERROR_SYNTAX;
                    if (eval2(p, s-p, &v1) != 0)
                    return ERROR_SYNTAX;

                    s ++;
                    p = s;
                    in = 1;
                    while (*s != 0 && s<e && in>0) {
                         if (*s == '(')
                         in ++;
                         else if (*s == ')')
                         in --;
                         s ++;
                    }
                    if (in>0)
                    return ERROR_SYNTAX;
                    if (eval2(p, s-p-1, &v2) != 0)
                    return ERROR_SYNTAX;

                    v = (*f->func)(v1, v2);
                    break;
               }
          }

          // ???3????
          //for (i=0; i<FUNC3_NUM; i++) {
//               const struct Function3 *f = func3+i;
//               if (e-s>=f->len && strncmp(f->name, s, f->len) == 0) {
//                    char *p;
//                    int in = 0, v1, v2, v3;
//                    s += f->len;
//                    p = s;
//                    while (*s != 0 && s<e && !(in == 0 && *s == ',')) {
//                         if (*s == '(')
//                         in ++;
//                         else if (*s == ')')
//                         in --;
//                         s ++;
//                    }
//                    if (*s != ',')
//                    return ERROR_SYNTAX;
//                    if (eval2(p, s-p, &v1) != 0)
//                    return ERROR_SYNTAX;
//
//                    s ++;
//                    p = s;
//                    in = 0;
//                    while (*s != 0 && s<e && !(in == 0 && *s == ',')) {
//                         if (*s == '(')
//                         in ++;
//                         else if (*s == ')')
//                         in --;
//                         s ++;
//                    }
//                    if (*s != ',')
//                    return ERROR_SYNTAX;
//                    if (eval2(p, s-p-1, &v2) != 0)
//                    return ERROR_SYNTAX;
//
//                    s ++;
//                    p = s;
//                    in = 1;
//                    while (*s != 0 && s<e && in>0) {
//                         if (*s == '(')
//                         in ++;
//                         else if (*s == ')')
//                         in --;
//                         s ++;
//                    }
//                    if (in>0)
//                    return ERROR_SYNTAX;
//                    if (eval2(p, s-p-1, &v3) != 0)
//                    return ERROR_SYNTAX;
//
//                    v = (*f->func)(v1, v2, v3);
//                    break;
//               }
//          }
     }

     // ???????????
     {
          int v2;

          // Skip space
          while (*s != 0 && s<e && (*s == ' ' || *s == '\t'))
          s ++;

          if (*s == 0 || s == e) { // ???????????
               *val = v;
               return 0;
          }

          if (*s == '=' && *(s+1) != '=') {
               char *p;
               int op = *s, in = 0;

               s ++;
               while (*s != 0 && s<e && (*s == ' ' || *s == '\t'))
               s ++;
               if (*s == 0 || s == e)
               return ERROR_SYNTAX;

               p = s;
               while (*s != 0 && s<e && (in>0 ||
               !(*s == '=' && *(s+1) == '='))) {
                    if (*s == '(') {
                         in ++;
                    } else if (*s == ')') {
                         in --;
                    }
                    s ++;
               }

               if ((err = eval2(p, s-p, &v2)) != 0)
               return err;

               if (varn < 0 || varn>=sizeof(variables))
               return ERROR_INTERNAL;
               v = variables[varn] = v2;

               if (*s == 0 || s == e) {
                    *val = v;
                    return 0;
               }
          }


          while (*s == '*' || *s == '/' || *s == '%') {
               char *p;
               int op = *s, in = 0;

               s ++;
               while (*s != 0 && s<e && (*s == ' ' || *s == '\t'))
               s ++;
               if (*s == 0 || s == e)
               return ERROR_SYNTAX;

               p = s;
               while (*s != 0 && s<e && (in>0 ||
               (*s != '*' && *s != '/' && *s != '%' &&
               *s != '+' && *s != '-' && *s != '>' && *s != '<' &&
               !(*s == '=' && *(s+1) == '=')  && *s != '!'))) {
                    if (*s == '(') {
                         in ++;
                    } else if (*s == ')') {
                         in --;
                    }
                    s ++;
               }

               if ((err = eval2(p, s-p, &v2)) != 0)
               return err;
               switch (op) {
               case '*':
                    v = v*v2;
                    break;
               case '/':
                    v = v/v2;
                    break;
               case '%':
                    v = v%v2;
                    break;
               }
               if (*s == 0 || s == e) {
                    *val = v;
                    return 0;
               }
          }

          if (*s == '+' || *s == '-') {
               char *p;
               int op = *s, in = 0;

               p = s;
               while (*s != 0 && s<e && (in>0 || (*s != '>' && *s != '<' && *s != '=' && *s != '!'))) {
                    if (*s == '(') {
                         in ++;
                    } else if (*s == ')') {
                         in --;
                    }
                    s ++;
               }
               if (in != 0)
               return ERROR_SYNTAX;

               // ???????????
               if ((err = eval2(p, s-p, &v2)) != 0)
               return err;
               // ??????
               v = v + v2;
               if (*s == 0 || s==e) {
                    *val = v;
                    return 0;
               }
          }

          if (*s == '>' || *s == '<' || (*s == '=' && *(s+1) == '=')
                    || (*s == '!' && *(s+1) == '=')) {
               char *p;
               int op = *s, op2 = *(s+1), in = 0;

               s ++;
               if (op2 == '=' || op2 == '<' || op2 == '>')
               s ++;

               p = s;
               while (*s != 0 && s<e && (in>0 || (*s != '&' && *s != '^' && *s != '|'))) {
                    if (*s == '(') {
                         in ++;
                    } else if (*s == ')') {
                         in --;
                    }
                    s ++;
               }
               if (in != 0)
               return ERROR_SYNTAX;

               // ???????????
               if ((err = eval2(p, s-p, &v2)) != 0)
               return err;

               if (op == '>') {
                    if (op2 == '=') {
                         v = v>=v2;
                    } else if (op2 == '>') {
                         v = v>>v2;
                    } else {
                         v = v>v2;
                    }
               } else if (op == '<') {
                    if (op2 == '=') {
                         v = v<=v2;
                    } else if (op2 == '<') {
                         v = v<<v2;
                    } else {
                         v = v<v2;
                    }
               } else if (op == '=' && op2 == '=') {
                    v = (v == v2);
               } else if (op == '!' && op2 == '=') {
                    v = (v != v2);
               }
               if (*s == 0 || s==e) {
                    *val = v;
                    return 0;
               }
          }

          if ((*s == '&' && *(s+1) != '&') || (*s == '|' && *(s+1) != '|') ||
                    *s == '^')  {
               char *p;
               int op = *s, in = 0;

               s ++;
               p = s;
               while (*s != 0 && s<e && (in>0 || !((*s == '&' && *(s+1) == '&') ||
               (*s == '|' && *(s+1) == '|')))) {
                    if (*s == '(') {
                         in ++;
                    } else if (*s == ')') {
                         in --;
                    }
                    s ++;
               }
               if (in != 0)
               return ERROR_SYNTAX;

               // ???????????
               if ((err = eval2(p, s-p, &v2)) != 0)
               return err;
               if (op == '&')
               v = v & v2;
               else if (op == '|')
               v = v | v2;
               else if (op == '^')
               v = v ^ v2;
               if (*s == 0 || s==e) {
                    *val = v;
                    return 0;
               }
          }

          if ((*s == '&' && *(s+1) == '&') || (*s == '|' && *(s+1) == '|')) {
               int op = *s;

               s += 2;
               // ???????????
               if ((err = eval2(s, e-s, &v2)) != 0)
               return err;
               if (op == '&')
               v = v && v2;
               else if (op == '|')
               v = v || v2;

               *val = v;
               return 0;
          }

     }
error:
     return ERROR_INTERNAL;
}

char *skipSpace(char *s, char *e)
{
     while (*s != 0 && s<e && (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n'))
     s ++;
     return s;
}

int
run(char *prog, int len)
{
     char *cur, *s, *e;
     int err, in, v, skip;

     s = prog;
     e = prog + len;

     for (;;) {
          char *p;
          s = skipSpace(s, e);
          if (*s == 0 || s>=e)
          break;

          skip = 0;
          // fprintf(stderr, s);

          if ((strncmp(s, "continue", 8) == 0
                         && (*(s+8) == ';' ||
                              *(s+8) == ' ' || *(s+8) == '\t' ||
                              *(s+8) == '\r' || *(s+8) == '\n')) ||
                    (strncmp(s, "break", 5) == 0
                         && (*(s+5) == ';' ||
                              *(s+5) == ' ' || *(s+5) == '\t' ||
                              *(s+5) == '\r' || *(s+5) == '\n'))) {
               char *cond, *cond_e, *p1, *statement, *statement_e;
               char *statement2 = NULL, *statement2_e;
               int continue_break = 0;

               p1 = s;
               if (strncmp(s, "continue",8) == 0) {
                    continue_break = 1;
                    s += 8;
               } else {
                    s += 5;
               }
               s = skipSpace(s, e);
               if (*s != ';') {
                    writeS(p1, 20);
                    return ERROR_SYNTAX;
               }
               s ++;

               if (continue_break == 0)
               return FOUND_BREAK;
               return FOUND_CONTINUE;
          } else if ((strncmp(s, "while", 5) == 0
                         && (*(s+5) == '(' ||
                              *(s+5) == ' ' || *(s+5) == '\t' ||
                              *(s+5) == '\r' || *(s+5) == '\n')) ||
                    (strncmp(s, "if", 2) == 0
                         && (*(s+2) == '(' ||
                              *(s+2) == ' ' || *(s+2) == '\t' ||
                              *(s+2) == '\r' || *(s+2) == '\n'))) {
               char *cond, *cond_e, *p1, *statement, *statement_e;
               char *statement2 = NULL, *statement2_e;
               int while_if = 0;
               if (strncmp(s, "while", 5) == 0) {
                    while_if = 1;
                    s += 5;
               } else
               s += 2;
               p1 = s;
               s = skipSpace(s, e);
               if (*s != '(') {
                    writeS(p1, 20);
                    return ERROR_SYNTAX;
               }
               s ++;
               cond = s;
               in = 1;
               while (*s !=0 && s<e && (in>0 || *s != ')')) {
                    if (*s == '(')
                    in ++;
                    if (*s == ')') {
                         in --;
                         if (in == 0)
                         break;
                    }
                    s ++;
               }
               if (*s != ')') {
                    writeS(p1, s-p1);
                    return ERROR_SYNTAX;
               }
               cond_e = s;
               s ++;
               s = skipSpace(s, e);
               if (*s == 0 || s>=e) {
                    writeS(p1, s-p1);
                    return ERROR_SYNTAX;
               }
               statement = s;
               in = 0;
               while (*s !=0 && s<e && (in>0 || (*s != '}' && *s != ';'))) {
                    if (*s == '{')
                    in ++;
                    else if (*s == '}') {
                         in --;
                         if (in == 0)
                         break;
                    }
                    s ++;
               }
               if (*s == 0 || s>=e || (*s != ';' && *s != '}') || in > 0) {
                    writeS(p1, s-p1);
                    return ERROR_SYNTAX;
               }
               s ++;
               statement_e = s;
               // ???? else ???????
               s = skipSpace(s, e);
               // fprintf(stderr, "*1 %s\n", s);
               if (while_if == 0 && strncmp(s, "else", 4) == 0
                         && (*(s+4) == '{' ||
                              *(s+4) == ' ' || *(s+4) == '\t' ||
                              *(s+4) == '\r' || *(s+4) == '\n')) {
                    s += 4;
                    s = skipSpace(s, e);
                    if (*s == 0 || s>=e) {
                         writeS(p1, s-p1);
                         return ERROR_SYNTAX;
                    }
                    //fprintf(stderr, "*2 %s\n", s);
                    statement2 = s;
                    in = 0;
                    while (*s !=0 && s<e && (in>0 || (*s != '}' && *s != ';'))) {
                         if (*s == '{')
                         in ++;
                         else if (*s == '}') {
                              in --;
                              if (in == 0)
                              break;
                         }
                         s ++;
                    }
                    if (*s == 0 || s>=e || (*s != ';' && *s != '}') || in > 0) {
                         writeS(p1, s-p1);
                         return ERROR_SYNTAX;
                    }
                    statement2_e = s;
               }

               if (while_if>0) {
                    // while ????
                    if (runAnimate || runStep) {
                         printS("while (");
                         writeS(cond, cond_e-cond);
                         printS(")\n");
                    }
                    if ((err = stepRun()) != 0)
                    return err;
                    for (;;) {
                         err = eval2(cond, cond_e-cond, &v);
                         if (err)
                         return err;
                         if (runAnimate || runStep) {
                              writeS(cond, cond_e-cond);
                              printS(": ");
                              printS(v ? "true" : "false");
                              printSln("");
                         }
                         //fwrite(cond, 1, cond_e-cond, stderr);
                         //fprintf(stderr, ": %d\n", v);
                         if (v == 0)
                         break;
                         if (runAnimate || runStep) {
                              if ((err = stepRun()) != 0)
                              return err;
                         }
                         err = run(statement, statement_e-statement);
                         if (err == FOUND_CONTINUE)
                         continue;
                         else if (err == FOUND_BREAK)
                         break;
                         else if (err)
                         return err;
                    }
               } else {
                    // if ????
                    if (runAnimate || runStep) {
                         printS("if (");
                         writeS(cond, cond_e-cond);
                         printS(")");
                    }
                    err = eval2(cond, cond_e-cond, &v);
                    if (err) {
                         if (runAnimate || runStep)
                         printSln("");
                         return err;
                    }
                    if (runAnimate || runStep) {
                         printS(": ");
                         printS(v ? "true" : "false");
                         printSln("");
                    }
                    //fwrite(cond, 1, cond_e-cond, stderr);
                    //fprintf(stderr, ": %d\n", v);
                    if (v != 0) {
                         err = run(statement, statement_e-statement);
                         if (err)
                         return err;
                    } else {
                         if (statement2 != NULL) {
                              err = run(statement2, statement2_e-statement2);
                              if (err)
                              return err;
                         }
                    }
               }
          } else if ((strncmp(s, "for", 3) == 0
                         && (*(s+3) == '(' ||
                              *(s+3) == ' ' || *(s+3) == '\t' ||
                              *(s+3) == '\r' || *(s+3) == '\n'))) {
               char *init, *init_e;
               char *cond, *cond_e, *inc, *inc_e;
               char *statement, *statement_e, *p1;
               p1 = s;
               s += 3;
               s = skipSpace(s, e);
               if (*s != '(') {
                    writeS(p1, 20);
                    return ERROR_SYNTAX;
               }
               s ++;
               s = skipSpace(s, e);
               init = s;
               while (*s !=0 && s<e && *s != ';')
               s ++;
               if (*s == 0 || s>=e) {
                    writeS(p1, 20);
                    return ERROR_SYNTAX;
               }
               init_e = s;
               s ++;
               s = skipSpace(s, e);
               cond = s;
               while (*s !=0 && s<e && *s != ';')
               s ++;
               if (*s == 0 || s>=e) {
                    writeS(p1, s-p1);
                    return ERROR_SYNTAX;
               }
               cond_e = s;
               s ++;
               inc = s = skipSpace(s, e);
               in = 1;
               while (*s !=0 && s<e && (in>0 || *s != ')')) {
                    if (*s == '(')
                    in ++;
                    else if (*s == ')') {
                         in --;
                         if (in == 0)
                         break;
                    }
                    s ++;
               }
               if (*s == 0 || s>=e) {
                    writeS(p1, s-p1);
                    return ERROR_SYNTAX;
               }
               inc_e = s;
               s ++;
               s = skipSpace(s, e);
               statement = s;
               in = 0;
               while (*s !=0 && s<e && (in>0 || (*s != '}' && *s != ';'))) {
                    if (*s == '{')
                    in ++;
                    else if (*s == '}') {
                         in --;
                         if (in == 0)
                         break;
                    }
                    s ++;
               }
               if (*s == 0 || s>=e || (*s != ';' && *s != '}') || in > 0) {
                    writeS(p1, s-p1);
                    return ERROR_SYNTAX;
               }
               s ++;
               statement_e = s;

               // for ????
               if (runAnimate || runStep) {
                    printS("for (");
                    writeS(init, init_e-init);
                    printS("; ");
                    writeS(cond, cond_e-cond);
                    printS("; ");
                    writeS(inc, inc_e-inc);
                    printS(")\n");
               }
               if ((err = stepRun()) != 0)
               return err;
               if (runAnimate || runStep) {
                    writeS(init, init_e-init);
                    printSln("");
               }
               if ((err = eval2(init, init_e-init, &v)) != 0) {
                    return err;
               }
               if ((err = stepRun()) != 0)
               return err;
               for (;;) {
                    if ((err = eval2(cond, cond_e-cond, &v)) != 0)
                    return err;
                    if (runAnimate || runStep) {
                         writeS(cond, cond_e-cond);
                         printS(": ");
                         printS(v ? "true" : "false");
                         printSln("");
                    }
                    if (v == 0)
                    break;
                    if (runAnimate || runStep) {
                         if ((err = stepRun()) != 0)
                         return err;
                    }
                    //fwrite(statement, 1, statement_e-statement, stderr);
                    if ((err = run(statement, statement_e-statement)) != 0) {
                         if (err == FOUND_CONTINUE)
                         ; /* DO_NOTHING */
                         else if (err == FOUND_BREAK)
                         break;
                         else return err;
                    }
                    if ((err = eval2(inc, inc_e-inc, &v)) != 0)
                    return err;
                    if (runAnimate || runStep) {
                         writeS(inc, inc_e-inc);
                         printSln("");
                    }
               }
          } else if (*s == '{' || *s == '}') {
               s ++;
               skip = 1;
          } else {
               // ?????
               p = s;
               in = 0;
               while (*s != 0 && s<e && (in>0 || *s != ';'))
               s ++;
               if (*s == 0 && in>0) {
                    writeS(p, s-p);
                    return ERROR_SYNTAX;
               }
               if (runAnimate != 0) {
                    writeS(p, s-p);
                    printSln("");
               }
               if ((err = eval2(p, s-p, &v)) != 0) {
                    writeS(p, s-p);
                    printSln("");
                    return err;
               }
               s ++;
#if 0
               printf("%d\n", v);
#endif
          }
          if (skip == 0) {
               if ((err = stepRun()) != 0)
               return err;
          }
     }
     return 0;
}

void edit(char *prog, int lNum, char *nLine)
{
     char *p, *q;
     int editLine;
     int l;

#if 0
     // Get line number to edit
     lNum = 0;
     p = nLine;
     while (*p != ' ') {
          if (*p>='0' && *p<='9')
          lNum = lNum*10 + (*p - '0');
          else {
               printSln("Invalid line number");
               return;
          }
          p ++;
     }
     nLine = p + 1;
#endif

     p = q = prog;
     l = 1;
     while (*q && l<=lNum)
     {
          if (*q == '\r' || *q =='\n') {
               if (*q == '\r' && *(q+1) == '\n')
               q ++;
               q ++;
               if (l == lNum || *q == 0)
               break;
               l ++;
               p = q;
               continue;
          }
          q ++;
     }
     // Append given line to the program if the line number is larger than current list.
     if (lNum>l && *q == 0) {
          strcat(prog, nLine);
          return;
     }
     // Replace the line with given one
     if ((q - p)<strlen(nLine)) {
          int sz = strlen(nLine) - (q-p);
          char *pp = prog + strlen(prog) + 1;
          char *qq = pp + sz;
          for (int i=strlen(q)+1; i>=0; i--) {
               *qq = *pp;
               qq --; pp --;
          }
          q = p + strlen(nLine);
     }
     for (int i=strlen(nLine); i>0; i--) {
          *p = *nLine;
          p ++; nLine ++;
     }
     if (p == q)
     return;
     for (int i=strlen(q); i>0; i--) {
          *p = *q;
          p ++; q ++;
     }
     *p = 0;
}

void list(char *prog)
{
     char *p, *q;
     int l = 1;

     p = q = prog;
     while (*q)
     {
          if (*q == '\r' || *q =='\n') {
#ifdef ARDUINO
               Serial.print(l);
               Serial.print(F(" "));
#else
               printf("%d ", l);
#endif
               writeS(p, q-p);
               printSln("");
               l ++;
               if (*q == '\r' && *(q+1) == '\n')
               q ++;
               q ++;
               p = q;
               continue;
          }
          q ++;
     }
}

///////////////////////////////////////////////////////////////////////////
// Main function for PC and UNIX
///////////////////////////////////////////////////////////////////////////
#ifndef ARDUINO
int
main(int argc, char **argv)
{
     int v;

     if (argc == 2) {
          if (eval2(argv[1], strlen(argv[1]), &v) != 0)
          printf("error\n");
          print(v);
     } else {
          prog = progbuf;
          for (;;)
          loop();
     }

     return 0;
}
#endif

/// TODO
// ????????(???????????????????)
// ????????
// ???(RAM)???

// ???????  //
/* // LED ??? (1) //
pinMode(13, OUTPUT);
for(;;) {
digitalWrite(13, HIGH);
delay(500);
digitalWrite(13, LOW);
delay(500);
}
*/
