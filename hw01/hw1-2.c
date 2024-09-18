#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "debug.h"

#define _LOW    1
#define _UP     10
#define LEN_LOW 10
#define LEN_UP  20
#define I_LOW   1
#define I_UP    25
#define K_LOW   0
#define K_UP    25
#define RAND(ITEM) (rand() % (ITEM##_UP - ITEM##_LOW + 1) + ITEM##_LOW)
#define HEAD -1
#define ISHEAD(v) (v->value == HEAD)
#define NSHOW 10

struct node {
    int value;
    struct node* next;
};
typedef struct node node;

node* init_list(int v) {
    node* ret = (node*) malloc(sizeof(node));
    ret->value = v;
    ret->next = NULL;
    return ret;
}

node* add_last(node* n, int v) {
    assert(n);
    node* ret = (node*) malloc(sizeof(node));
    ret->value = v;
    ret->next = NULL;
    node* ptr = n;
    while (ptr->next) 
        ptr = ptr->next;
    ptr->next = ret;
    return ret;
}

void del(node* pre, node* del) {
    assert(pre && del);
    pre->next = del->next;
    free(del);
}

node* random_list(int len) {
    node* ret = init_list(HEAD);
    for (int i = 0; i < len; i++)
        add_last(ret, RAND());
    return ret;
}

void free_list(node* v) {
    assert(ISHEAD(v));
    node* pre = v;
    node* ptr = pre->next;
    while (ptr) {
        del(pre, ptr);
        ptr = pre->next;
    }
    free(v);
}

void print_list(node* v) {
    assert(ISHEAD(v));
    if (v == NULL) {
        printf("blank\n");
        return;
    }
    node* ptr = v->next;
    printf("HEAD");
    while (ptr) {
        printf("-%d", ptr->value);
        ptr = ptr->next;
    }
    printf("\n");
}

void del_k_node_from_i(node* v, int i, int k) {
    assert(ISHEAD(v));
    assert(i > 0);
    assert(k >= 0);
    node* ptr = v;
    node* pre = v;
    while (i--) {
        if (ptr->next) {
            pre = ptr;
            ptr = ptr->next;
        }
        else return;
    }
    while (k--) {
        if (ptr) {
            del(pre, ptr);
            ptr = pre->next;
        }
        else return;
    }
}

int main() {
    for (int j = 0; j < NSHOW; j++) {
        Log("Test %d", j + 1);
        int len = RAND(LEN);
        node* v = random_list(len);
        printf("Create list len = %d\n", len);
        print_list(v);
        int i = RAND(I);
        int k = RAND(K);
        printf("delete i = %d, k = %d\n", i, k);
        del_k_node_from_i(v, i, k);
        printf("after del:\n");
        print_list(v);
        printf("\n");
    }
}
