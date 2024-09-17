struct freqitem {
    itemset items;
    int sup;
    struct freqitem* next;
};
typedef struct freqitem freqitem;

struct freqlist {
    int len;        // length of list
    int size;       // size of itemset
    int k;
    freqitem list;
};
typedef struct freqlist freqlist;

freqlist*           init_freqlist(freqlist* fl, int k, int size);
void                insert_freqlist(freqlist *fl, itemset items, int sup);
void                print_freqlist(freqlist *fl);
void                free_freqlist(freqlist *fl);
int                 itemset_in_freqlist(freqlist* fl, itemset items);
