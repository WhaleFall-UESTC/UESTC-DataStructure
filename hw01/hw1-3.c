#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "debug.h"

#define NSHOW 5
#define _LOW    1
#define _UP     10
#define LEN_LOW 10
#define LEN_UP  20
#define RAND(ITEM) (rand() % (ITEM##_UP - ITEM##_LOW + 1) + ITEM##_LOW)
#define print_order(order, n) \
    do { \
        printf("print " #order " order:\n"); \
        printf("%d", n->value); \
        node* ptr = n->order; \
        while (ptr != n) { \
            printf("-%d", ptr->value); \
            ptr = ptr->order; \
        } \
        printf("\n"); \
    } while(0);

struct node {
    int value;
    struct node* prev;
    struct node* next;
};
typedef struct node node;

node* init_list(int v) {
    node* ret = (node*) malloc(sizeof(node));
    ret->value = v;
    ret->prev = ret->next = ret;
}

node* add_last(node* n, int v) {
    assert(n);
    node* ret = (node*) malloc(sizeof(node));
    ret->value = v;
    node* n_next = n->next;
    n->next = ret;
    ret->prev = n;
    n_next->prev = ret;
    ret->next = n_next;
    return ret;
}

node* random_list(int len, int init_v) {
    node* init = init_list(init_v);
    while (len--) {
        add_last(init, RAND());
    }
    return init;
}

void print_list(node *n) {
    assert(n);
    printf("print link list start at %p\n", n);
    print_order(next, n);
    print_order(prev, n);
}

void reverse(node* n) {
    assert(n);
    node* current = n;
    do {
        node* next = current->next;
        current->next = current->prev;
        current->prev = next;
        current = next;
    } while (current != n);
}

void free_list(node* v) {
    node *ptr = v;
    do {
        node* next = ptr->next;
        free(ptr);
        ptr = ptr->next;
    } while (ptr != v);
}

int main() {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < NSHOW; i++) {
        node* list = random_list(RAND(LEN), RAND());
        Log("randomly init link list:");
        print_list(list);
        Log("reverse it:");
        reverse(list);
        print_list(list);
        free_list(list);
        printf("\n\n");
    }
}
