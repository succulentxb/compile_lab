/*
 * util.c - commonly used utility functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
void *checked_malloc(int len)
{void *p = malloc(len);
 if (!p) {
    fprintf(stderr,"\nRan out of memory!\n");
    exit(1);
 }
 return p;
}

string String(char *s)
{string p = checked_malloc(strlen(s)+1);
 strcpy(p,s);
 return p;
}

string stripQuote(string s) {
    int len = strlen(s);
//    printf("s=%s, strlen(s)=%d\n", s, len);
    string p = checked_malloc(len-2+1);
    s = s+1;
//    printf("s+1=%s\n", s);
    strncpy(p, s, len-2);
//    printf("p=%s, strlen(p)=%d\n", p);
    return p;
}

string parseStr(string s) {
    int len = strlen(s);
    if (len == 0) {
        return "(null)";
    }
    string p = checked_malloc(len+1);
    int si = 0, pi = 0;
    while (si < len) {
        if (*(s+si) == '\\') {
            if (si >= len-1)
                return NULL;
            switch (*(s+si+1)) {
                case 'n':
                    *(p+pi) = '\n';
                    break;
                case '\\':
                    *(p+pi) = '\\';
                    break;
//                default:
            }
            pi++;
            si = si + 2;
        } else {
            *(p+pi) = *(s+si);
            pi++;
            si++;
        }
    }
    return p;
}

U_boolList U_BoolList(bool head, U_boolList tail)
{ U_boolList list = checked_malloc(sizeof(*list));
  list->head = head;
  list->tail = tail;
  return list;
}
