#define HEAP_INIT 8

struct heapitem {
    itemset items;
    int sup;
};
typedef struct heapitem heapitem;

struct heap {
    heapitem *pq;
    int n;
    int capacity;
};
typedef struct heap heap;

typedef unsigned char bool;
#define true  1
#define false 0

heap*           init_heap();
void            insert_heap(heap* h, itemset items, int sup);
heapitem        pop_heap(heap* h);
void            add_freqlist(heap* h, freqlist *fl);
void            free_heap(heap* h);
