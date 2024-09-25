#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"

#define SIZE 101
typedef int item;

static item ringq[SIZE] = {};
static int first = 0;
static int last = 1;

void add(item n) {
    if (last == first)
        panic("Overflow, n = %d, f = l = %d", n, first);
    ringq[last] = n;
    last = (last + 1) % SIZE;
}

item pop() {
    int len = 0;
    if (first < last) len = last - first -1;
    else len = SIZE - 1 - first + last;
    if (len <= 0) panic("ringq has no item");
    first = (first + 1) % SIZE;
    return ringq[first];
}

item op(item n) {
    item ret;
    switch (n) {
        case -1: ret = pop(); break;
        case 0:  ret = 0; break;
        default: add(n); ret = -1; break;
    }
    return ret;
}

int main() {
    int n = 0;
    while (scanf("%d", &n) && n) 
        op(n);
    return 0;
}

void test() {
        // for (int i = 0; i < 3; i++) {
    //     puts("Add 1 ~ 100 to ringq");
    //     for (int i = 1; i <= 100; i++) {
    //         add(i);
    //     }
    //     printf("pop: ");
    //     for (int i = 1; i <= 100; i++) {
    //         item p = pop();
    //         assert(p == i);
    //     }
    //     puts("pass check");
    // }
    // puts("check pop blank ringq");
    // pop();
    // // puts("check outof bound, add 101 items");
    // // for (int i = 1; i <= 101; i++) {
    // //     add(i);
    // // }
}
