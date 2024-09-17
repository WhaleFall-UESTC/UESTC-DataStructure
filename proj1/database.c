#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "itemset.h"
#include "freqlist.h"
#include "database.h"

database*
init_db(database* db)
{
    if (db == NULL)
        db = (database*) malloc(sizeof(database));
    memset(db, 0, sizeof(database));
    return db;
}

void
insert_db(database* db, itemset items)
{
    dbitem* ptr = &db->list;
    while (ptr->next)
        ptr = ptr->next;
    ptr->next = (dbitem*) malloc(sizeof(dbitem));
    ptr = ptr->next;
    ptr->items = items;
    ptr->next = NULL;
    // printf("insert ptr %p\n", ptr);
    // print_itemset(items, db->size);
}

int
count_minsup(database* db, itemset items)
{
    int cnt = 0;
    // printf("count_minsup:\n");
    dbitem* ptr = db->list.next;
    while (ptr) {
        if (contain(ptr->items, items, db->size) == 0) {
            cnt++;
        }
        ptr = ptr->next;
    }
    // printf("cm get sup: %d\n", cnt);
    return cnt;
}

void
print_db(database db)
{
    printf("database info:\n");
    printf("len: %d\n", db.len);
    printf("size: %d\tmax_index: %d\n", db.size, db.max_index);
    dbitem* ptr = db.list.next;
    while (ptr) {
        print_itemset(ptr->items, db.size);
        ptr = ptr->next;
    }
    printf("\n");
}

freqlist*
scan_db(database db, int minsup)
{
    freqlist* fl = init_freqlist(NULL, 1, db.size);

    dbitem* ptr = db.list.next;
    int len = db.size / NBYTES;
    int* support = (int *) malloc(db.max_index * sizeof(int));
    memset(support, 0, db.max_index * sizeof(int));
    while (ptr) {
        for (int i = 0; i < len; i++) 
            for (int j = 0; j < NBITS; j++) 
                if (CHECKBIT(ptr->items[i], j)) 
                    support[NO(i, j) - 1]++;
        ptr = ptr->next;
    }

    for (int i = 0; i < db.max_index; i++) {
        if (support[i] >= minsup) {
            itemset items = init_itemset(db.size);
            add_itemset(items, i + 1);
            insert_freqlist(fl, items, support[i]);
        }
    }
    free(support);
    return fl;
}

void
free_db(database* db)
{
    dbitem* ptr = db->list.next;
    while (ptr) {
        dbitem* tmp = ptr;
        ptr = ptr->next;
        free(tmp);
    }
    // free(db);
}