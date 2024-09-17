#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "itemset.h"

itemset
init_itemset(int size)
{
    itemset ret = (itemset) malloc(size);
    memset(ret, 0, size);
    return ret;
}

int
contain(itemset p, itemset c, int size)
{
    int len = size / NBYTES;
    // printf("contain %p %p\n", p, c);
    for (int i = 0; i < len; i++) {
        if ((p[i] | c[i]) != p[i]) {
            return -1;
        }
    }
    return 0;
}

static inline int
count_block_bits(itemblock n)
{
    int cnt = 0;
    while (n) {
        n &= (n - 1);
        cnt++;
    }
    return cnt;
}

/* conjunct two itemset. if the num of 1s > k, return NULL */
/* set alloc=1 to malloc a new itemset */
/* otherwise, the result will be stored in s1 */
itemset
conjunct(itemset s1, itemset s2, int size, int k, int alloc)
{
    itemset ret;
    if (alloc) ret = init_itemset(size);
    else ret = s1;

    int len = size / NBYTES;
    int cnt_k = 0;
    for (int i = 0; i < len; i++) {
        ret[i] = (s1[i] | s2[i]); 
        cnt_k += count_block_bits(ret[i]);
        if (cnt_k > k) {
            if (alloc) free(ret);
            return NULL;
        }
    }
    return ret;
}

/* no starts from 1 */
void
add_itemset(itemset items, int no)
{
    no -= 1;
    int idx = no / NBITS;
    int off = no - idx * NBITS;
    items[idx] |= (1 << off);
}

void
del_itemset(itemset items, int no)
{
    // printf("delete number %d\n", no);
    no -= 1;
    int idx = no / NBITS;
    int off = no - idx * NBITS;
    items[idx] &= ~(1 << off);
}

itemset
copy_itemset(itemset items, int size)
{
    // printf("copy itemset %p\n", items);
    // print_itemset(items, size);
    itemset ret = init_itemset(size);
    int len = size / NBYTES;
    for (int i = 0; i < len; i++) {
        ret[i] = items[i];
    }
    return ret;
}

void
print_itemset(itemset items, int size)
{
    int len = size / NBYTES;
    printf("[ ");
    for (int i = 0; i < len; i++)
        for (int j = 0; j < NBITS; j++) 
            if (CHECKBIT(items[i], j))
                printf("%d ", NO(i, j));
    printf("]\n");
}
