#define NBITS 8     // itemset 块的位数
#define NBYTES (NBITS >> 3)
#define IDX2SIZE(idx) (((idx) >> 3) + 1)
typedef unsigned char uint8;
typedef uint8* itemset;
typedef uint8  itemblock;

#define CHECKBIT(n, off) ((n) & (1 << off))
#define NO(i, j) (i * NBITS + j + 1)

#define ALLOC   1
#define NOALLOC 0

itemset     init_itemset(int size);
int         contain(itemset p, itemset c, int size);
itemset     conjunct(itemset s1, itemset s2, int size, int k, int alloc);
void        add_itemset(itemset items, int no);
void        del_itemset(itemset items, int no);
itemset     copy_itemset(itemset items, int size);
void        print_itemset(itemset items, int size);
