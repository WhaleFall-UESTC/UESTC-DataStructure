#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "itemset.h"
#include "database.h"
#include "file.h"

int
read_file(database *db, const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return -1;
    }
    
    fscanf(fp, "%d\n", &db->len);
    fscanf(fp, "%d\n", &db->max_index);
    db->size = IDX2SIZE(db->max_index);

    char line[BUF];
    char *token;
    itemset items;
    while (fgets(line, BUF, fp) != NULL) {
        items = init_itemset(db->size);
        token = strtok(line, " \t\n");
        while (token != NULL) {
            int no = atoi(token);
            add_itemset(items, no);
            token = strtok(NULL, " \t\n");
        }
        insert_db(db, items);
    }
    
    fclose(fp);
    return 0;
}

int
random_file(const char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        return -1;
    }

    srand((unsigned)time(NULL));
    int len = RAND(MIN_LEN, MAX_LEN);
    int max_index = RAND(MIN_INDEX, MAX_INDEX);
    fprintf(fp, "%d\n%d\n", len, max_index);
    
    for (int i = 0; i < len; i++) {
        int size = RAND(MIN_GROUP_NUM, MAX_GROUP_NUM);
        for (int j = 0; j < size; j++) {
            int no = RAND(1, max_index);
            fprintf(fp, "%d ", no);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    return 0;
}