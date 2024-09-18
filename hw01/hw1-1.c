#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "debug.h"

#define LOW 1
#define UP  10
#define RAND (rand() % (UP - LOW + 1) + LOW)

struct node {
    int value;
    struct node* next;
};
typedef struct node node;

void (*handle)(node*, node*);

node* init_list(int v) {
    node* ret = (node*) malloc(sizeof(node));
    ret->value = v;
    ret->next = ret;
    return ret;
}

node* add_last(node* n, int v) {
    assert(n);
    node* ret = (node*) malloc(sizeof(node));
    ret->value = v;
    ret->next = n->next;
    n->next = ret;
    return ret;
}

void del(node* pre, node* del) {
    assert(pre && del);
    pre->next = del->next;
    free(del);
}

node* remove_x(node* n, int x) {
    assert(n);
    node* ptr = n->next;
    node* pre = n;
    
    while (ptr != n) {
        if (ptr->value == x) {
            del(pre, ptr);
            ptr = pre->next;
        } else {
            pre = ptr;
            ptr = ptr->next;
        }
    }

    if (n->value == x) {
        if (n == n->next) {
            free(n);
            return NULL;
        } 
        del(pre, n);
        return pre;
    }

    return n;
}

void print_list(node* n) {
    if (n == NULL) {
        printf("blank\n");
        return;
    }
    node* ptr = n->next;
    printf("%d", n->value);
    while (ptr != n) {
        printf("-%d", ptr->value);
        ptr = ptr->next;
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    int num = atoi(argv[1]);
    int* record = (int*) malloc(sizeof(int) * num);
    memset(record, 0, sizeof(int) * num);
    srand((unsigned int)time(NULL));

    int init_v = RAND;
    node* init = init_list(init_v);
    printf("init the list\n");
    printf("add: %d", init_v);
    record[init_v - 1] = 1;
    for (int i = 0; i < num; i++) {
        int v = RAND;
        record[v - 1] = 1;
        printf(" %d", v);
        add_last(init, v);
    }
    printf("\ncreate list:\n");
    print_list(init);
    
    for (int i = 0; i < num; i++) {
        if (record[i]) {
            printf("remove %d\n", i + 1);
            init = remove_x(init, i + 1);
            printf("after remove %d:\n", i + 1);
            print_list(init);
        }
    }

    return 0;
}
