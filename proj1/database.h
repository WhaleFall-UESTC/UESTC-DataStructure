struct dbitem {
    itemset items;
    struct dbitem* next;
};
typedef struct dbitem dbitem;

struct database {
    int len;
    int max_index;
    int size;
    dbitem list;
};
typedef struct database database;

database*           init_db(database* db);
void                insert_db(database* db, itemset items);
int                 count_minsup(database* db, itemset items);
void                print_db(database db);
freqlist*           scan_db(database db, int minsup);
void                free_db(database* db);
