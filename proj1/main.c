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

#define LWC_SUP    1    // filter whose sup is less than min_sup
#define LWC_CHILD  2    // filter whose child is not frequent
#define LWC_CUT    3    // filter both

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
    while (pi->next) {
        enter_cycle = true;
        freqitem* pj = pi->next;
        while (pj) {
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
                insert_freqlist(ret, comb, sup);
            } else {
                free(comb);
            }

            pj = pj->next;
        }
        pi = pi->next;
    }

    if (!enter_cycle || ret->len == 0) {
        // printf("no next\n");
        free(ret);
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
    freqlist* fl2 = link_with_cut(fl1, LWC_SUP);
    printf("first lwc success. fl2: %p\n", fl2);
    if (fl2 != NULL)
        print_freqlist(fl2);
    printf("fl2 OK\n");
    freqlist* fl3 = link_with_cut(fl2, LWC_CUT);
    if (fl3 != NULL)
        print_freqlist(fl3);

    free_freqlist(fl3);
    free_freqlist(fl2);
    free_freqlist(fl1);
    free_db(&db);
}