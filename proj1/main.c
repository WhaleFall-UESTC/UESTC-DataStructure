#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "itemset.h"
#include "freqlist.h"
#include "database.h"
#include "file.h"

typedef unsigned char bool;
#define true 1
#define false 0
#define BUF 512
#define RING_BUF 8
#define RING_NEXT(p) (((p) + 1) % RING_BUF)
#define RING_PRE(p)  (((p) + RING_BUF - 1) % RING_BUF)

#define LWC_SUP    1    // filter whose sup is less than min_sup
#define LWC_CHILD  2    // filter whose child is not frequent
#define LWC_REPEAT 4    // filter those repeat
#define LWC_ALL    7    // filter all

#define TEST(no) do {   \
    ring_buffer[no] = link_with_cut(ring_buffer[no-1], LWC_ALL); \
    if (ring_buffer[no] == NULL) { \
        free_freqlist(ring_buffer[no]); \
        goto end; \
    } \
    print_freqlist(ring_buffer[no]); \
} while (0);

static unsigned outset = OUTPUT_DEFAULT;
static char filename[BUF] = DEFAULT_FILE;
static int topn = DEFAULT_TOPN;
static bool rand_file = false;
static int min_support = DEFAULT_MIN_SUP;

static database db;

static int 
prase_args(int argc, char *argv[])
{
    const struct option table[] = {
        {"src"  , optional_argument, NULL, 's'},
        {"all"  , no_argument      , NULL, 'A'},
        {"max"  , no_argument      , NULL, 'M'},
        {"top"  , optional_argument, NULL, 'T'},
        {"rand" , optional_argument, NULL, 'r'},
        {"min-support", optional_argument, NULL, 'm'},
        {0      , 0                , NULL,  0 },
    };
    int o;
    while ((o = getopt_long(argc, argv, "s::AMT::r::m::", table, NULL)) != -1) {
        switch (o) {
            case 's': if (optind < argc && argv[optind][0] != '-' && optarg == NULL) {
                        strcpy(filename, argv[optind++]);
                      }
                      break;
            case 'A': outset |= OUTPUT_ALL; break;
            case 'M': outset |= OUTPUT_MAX; break;
            case 'T': outset |= OUTPUT_TOPN;
                      if (optarg)
                        sscanf(optarg, "%d", &topn); 
                      break;
            case 'r': rand_file = true; 
                      if (optind < argc && argv[optind][0] != '-' && optarg == NULL) {
                        strcpy(filename, argv[optind++]);
                      }
                      break;
            case 'm': min_support = (optarg ? atoi(optarg) : min_support);
                      break;           
            default:
                printf("Usage: too lazy to write it\n");
        }
    }
    return 0;
}

freqlist* 
link_with_cut(freqlist* fl, int flag) 
{
    freqlist* ret = init_freqlist(NULL, fl->k + 1, fl->size);
    freqitem* pi = fl->list.next;
    bool enter_cycle = false;
    while (pi && pi->next) {
        // printf("new pi at %p\n", pi);
        enter_cycle = true;
        freqitem* pj = pi->next;
        while (pj) {
            // printf("new pj at %p\n", pj);
            // printf("comb\n");
            itemset comb = conjunct(pi->items, pj->items, fl->size, fl->k + 1, ALLOC);
            if (comb == NULL) {
                pj = pj->next;
                continue;
            }
            // printf("conjunct\n");
            bool pass_sup = true, pass_child = true;
            int sup = count_minsup(&db, comb);
            // printf("SUP\n");
            if (flag & LWC_SUP) {
                if (sup < min_support) 
                    pass_sup = false;
            }
            if (flag & LWC_CHILD) {
                int* nos = (int*) malloc(fl->size * sizeof(int));
                int nop = 0;
                int len = fl->size / NBYTES;
                // printf("get index, find:\n");
                for (int i = 0; i < len; i++) {
                    for (int j = 0; j < NBITS; j++)
                        if (CHECKBIT(comb[i], j)) {
                            // printf("%d ", NO(i, j));
                            nos[nop++] = NO(i, j);
                        }
                }
                // printf("\nget all index, comb and its children:\n");
                // print_itemset(comb, fl->size);
                for (int i = 0; i < nop; i++) {
                    itemset child = copy_itemset(comb, fl->size);
                    del_itemset(child, nos[i]);
                    // print_itemset(child, fl->size);
                    if (itemset_in_freqlist(fl, child) < 0) {
                        // printf("This child not freq\n");
                        free(child);
                        pass_child = false;
                        break;
                    }
                    free(child);
                }
                free(nos);
            }
            
            if (pass_sup && pass_child) {
                // printf("before insert comb %p\n", comb);
                insert_freqlist(ret, comb, sup);
            } else {
                // printf("before free comb %p\n", comb);
                // print_itemset(comb, fl->size);;
                free(comb);
                // printf("after free comb %p\n", comb);
            }

            pj = pj->next;
            // printf("pj renew OK\n");
        }
        pi = pi->next;
        // printf("pi renew OK, new pi: %p, next: %p\n", pi, pi->next);
        // puts("OK");
    }

    if (!enter_cycle || ret->len == 0) {
        // printf("no next\n");
        free(ret);
        // puts("free ret");
        return NULL;
    }
    return ret;
}

int 
main(int argc, char *argv[])
{
    init_db(&db);
    prase_args(argc, argv);

    if (rand_file)
        random_file(filename);
    read_file(&db, filename);
    // print_db(db);

    freqlist* fl1 = scan_db(db, min_support);
    // print_freqlist(fl1);
    freqlist* ring_buffer[RING_BUF];
    memset(ring_buffer, 0, RING_BUF * sizeof(freqlist*));
    // int bufptr = 1;
    ring_buffer[0] = link_with_cut(fl1, LWC_SUP);
    if (ring_buffer[0] == NULL) {
        free_freqlist(ring_buffer[0]);
        goto end;
    }
    print_freqlist(ring_buffer[0]);

    TEST(1);
    TEST(2);
    TEST(3);
    TEST(4);
    TEST(5);
    TEST(6);


    // while ((ring_buffer[bufptr] = 
    //     link_with_cut(ring_buffer[RING_PRE(bufptr)], LWC_ALL)))
    // {
    //     print_freqlist(ring_buffer[bufptr]);
    //     bufptr = RING_NEXT(bufptr);
    //     printf("bufptr up to %d\n", bufptr);
    //     if (ring_buffer[bufptr] != NULL)
    //         free_freqlist(ring_buffer[bufptr]);
    // }
    
end:
    free_freqlist(fl1);
    for (int i = 0; i < RING_BUF; i++) 
        free_freqlist(ring_buffer[i]);
    free_db(&db);
    puts("Over");
}