#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"

#define SIZE 1024
static char stack[SIZE];
static int sp = -1;

#define CASE(l, r) \
    case r: \
        if (stack[sp] == l) pop(); \
        else Log("expect %c but %c", l, stack[sp]), pass = 0; \
        break;

void push(char c) {
    if (sp == SIZE - 1) panic("Overflow");
    stack[++sp] = c;
}

char pop() {
    if (sp < 0) panic("stack has no item");
    return stack[sp--];
}

int main() {
    char buf[SIZE];
    scanf("%s", buf);
    int ptr = 0, len = strlen(buf);
    assert(buf[len - 1] == '#');
    while (buf[ptr] != '#' && ptr < len) {
        unsigned char pass = 1;
        switch (buf[ptr]) {
            case '(': case '[': case '{':
                push(buf[ptr]);
                break;
            CASE('(', ')');
            CASE('[', ']');
            CASE('{', '}');
            default: 
                Log("unexpected %c", buf[ptr]);
        }
        if (!pass) break;
        ptr++;
    }
    if (sp != -1) Log("Not matched");
    else Log("Matched");
    return 0;
}
