#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "itemset.h"
#include "freqlist.h"
#include "heap.h"
#include "debug.h"

heap*
init_heap()
{
    heap* ret = (heap*) malloc(sizeof(heap));
    ret->n = 0;
    ret->capacity = HEAP_INIT;
    ret->pq = (heapitem*) malloc(HEAP_INIT * sizeof(heapitem));
    memset(ret->pq, 0, HEAP_INIT * sizeof(heapitem));
    return ret;
}

void
free_heap(heap* h)
{
    if (h == NULL) return;
    if (h->pq != NULL) free(h->pq);
    free(h);
}

static inline void
resize_heap(heap* h, int len)
{
    // Log("resize heap to %d", len);
    heapitem* prev = h->pq;
    h->pq = (heapitem*) malloc(len * sizeof(heapitem));
    memset(h->pq, 0, len * sizeof(heapitem));
    // zero to h->n, total h->n + 1
    memmove(h->pq, prev, (h->n + 1) * sizeof(heapitem));
    free(prev);
    h->capacity = len;
}

static inline bool
legal(heap* h, int idx)
{
    return (idx >= 1 && idx <= h->n);
}

static inline void
exch(heap* h, int idx1, int idx2)
{
    heapitem tmp;
    memmove(&tmp, &h->pq[idx1], sizeof(heapitem));
    memmove(&h->pq[idx1], &h->pq[idx2], sizeof(heapitem));
    memmove(&h->pq[idx2], &tmp, sizeof(heapitem));
}

static inline bool
smaller(heap* h, int idx1, int idx2)
{
    return (h->pq[idx1].sup < h->pq[idx2].sup);
}

static inline void 
swim(heap* h, int k)
{
    while (k > 1 && smaller(h, k / 2, k)) {
        exch(h, k / 2, k);
        k /= 2;
    }
}

static inline void
sink(heap* h, int k)
{
    while (k * 2 < h->n) {
        int j = k * 2;
        if (j < h->n && smaller(h, j, j + 1))
            j++;
        if (!smaller(h, k, j))
            break;
        exch(h, k, j);
        k = j;
    }
}

static inline bool
is_heap_order(heap* h, int k)
{
    if (k > h->n) return true;
    int l = k * 2;
    int r = l + 1;
    if (l <= h->n && smaller(h, k, l)) return false;
    if (r <= h->n && smaller(h, k, r)) return false;
    return is_heap_order(h, l) && is_heap_order(h, r);
}

static inline bool
is_heap(heap* h) {
    for (int i = 1; i <= h->n; i++)
        if (h->pq[i].items == NULL) {
            Log("pq[%d] should not be NULL, h->n = %d", i, h->n);
            return false;
        }
    for (int i = h->n + 1; i < h->capacity; i++)
        if (h->pq[i].items != NULL) {
            Log("pq[%d] should be NULL, h->n = %d", i, h->n);
            return false;
        }
    if (h->pq[0].items != NULL)
        return false;
    return is_heap_order(h, 1);
}

void
insert_heap(heap* h, itemset items, int sup)
{
    if (h->n == h->capacity - 1)
        resize_heap(h, h->capacity * 2);
    heapitem item = (heapitem){.items = items, .sup = sup};
    memmove(&h->pq[++h->n], &item, sizeof(heapitem));
    swim(h, h->n);
    assert(is_heap(h));
}

heapitem
pop_heap(heap* h)
{
    assert(h->n > 0);
    heapitem ret;
    memmove(&ret, &h->pq[1], sizeof(heapitem));
    exch(h, 1, h->n--);
    sink(h, 1);
    memset(&h->pq[h->n + 1], 0, sizeof(heapitem));
    if ((h->n > 0) && (h->n == (h->capacity - 1) / 4) && h->capacity >= HEAP_INIT)
        resize_heap(h, h->capacity / 2);
    assert(is_heap(h));
    return ret;
}

void
add_freqlist(heap* h, freqlist *fl)
{
    freqitem* ptr = fl->list.next;
    while (ptr) {
        insert_heap(h, ptr->items, ptr->sup);
        ptr = ptr->next;
    }
}

/* test for heap */
// static void
// print_heap(heap *h)
// {
//     Log("heap capacity = %d, n = %d", h->capacity, h->n);
//     int i = 1;
//     while (i <= (h->n + 1) / 2) {
//         printf("%d --- %d %d\n", h->pq[i].sup, h->pq[i * 2].sup, h->pq[i * 2 + 1].sup);
//         i++;
//     }
// }

// int 
// main()
// {
//     heap* h = init_heap();
//     Log("test inset");
//     for (int i = 1; i <= 32; i++) {
//         insert_heap(h, &i, i);
//         print_heap(h);
//     }
//     Log("test del");
//     for (int i = 1; i <= 32; i++) {
//         heapitem tmp = pop_heap(h);
//         Log("pop max sup %d", tmp.sup);
//         print_heap(h);
//     }
// }

