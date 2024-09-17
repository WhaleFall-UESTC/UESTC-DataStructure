#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "itemset.h"
#include "freqlist.h"

freqlist*
init_freqlist(freqlist* fl, int k, int size)
{
    if (fl == NULL)
        fl = (freqlist*) malloc(sizeof(freqlist));
    memset(fl, 0, sizeof(freqlist));
    fl->k = k;
    fl->size = size;
    return fl;
}

void
insert_freqlist(freqlist *fl, itemset items, int sup)
{
    freqitem* fi = (freqitem*) malloc(sizeof(freqitem));
    // printf("insert itemset %p\n", items);
    // print_itemset(items, fl->size);
    fi->items = items;
    fi->sup = sup;
    fi->next = NULL;

    freqitem* ptr = &fl->list;
    while (ptr->next)
        ptr = ptr->next;
    ptr->next = fi;
    fl->len++;
}

void
print_freqlist(freqlist *fl)
{
    printf("freqlist info at %p\n", fl);
    printf("len: %d\tsize: %d\n", fl->len, fl->size);
    freqitem *ptr = fl->list.next;
    printf("sup   itemset\n");
    while (ptr) {
        printf("%3d   ", ptr->sup);
        print_itemset(ptr->items, fl->size);
        ptr = ptr->next;
    }
}

void
free_freqlist(freqlist *fl)
{
    freqitem *ptr = fl->list.next;
    while (ptr) {
        freqitem *tmp = ptr;
        ptr = ptr->next;
        free(tmp);
    }
    free(fl);
}

int
itemset_in_freqlist(freqlist* fl, itemset items)
{
    freqitem* ptr = fl->list.next;
    while (ptr) {
        if (contain(ptr->items, items, fl->size) == 0) {
            return 0;
        }
        ptr = ptr->next;
    }
    return -1;
}
