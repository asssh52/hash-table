#include "../hpp/hashtable_v1.hpp"
#include "../hpp/colors.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

const int NUM_BCKT = 16;

int     hashTblCtor (hashTbl_t** hashtbl);
int     hashTblVtor (hashTbl_t*  hashtbl);
int     hashTblDtor (hashTbl_t*  hashtbl);
int     hashTblDump (hashTbl_t*  hashtbl);
int     hashTblAdd  (hashTbl_t* hashtbl, const char* name);
int     hashTblFind (hashTbl_t* hashtbl, const char* name);
int     listFind    (bucket_t*  list,    const char* name);
int     bucketAdd   (bucket_t** list,    const char* name);
int     countHash   (const char* name);
int     textParse   (hashTbl_t* hashtbl, const char* file);
size_t  getFileSize (const char* filename);
int     textParse   (hashTbl_t* hashtbl, const char* file);

//=====================================================//


int hashTblCtor(hashTbl_t** hashtbl_addr){
    *hashtbl_addr = (hashTbl_t*) calloc(1, sizeof(**hashtbl_addr));
    hashTbl_t* hashtbl = *hashtbl_addr;

    if (!hashtbl) return ERR;

    hashtbl->buckets        = (bucket_t**) calloc(NUM_BCKT, sizeof(bucket_t*));
    hashtbl->num_bckts    = NUM_BCKT;

    if (hashTblVtor(hashtbl)) return ERR;

    return OK;
}


//=====================================================//


int hashTblVtor(hashTbl_t* hashtbl){
    if (!hashtbl)           return ERR;
    if (!hashtbl->buckets)  return ERR;

    return OK;
}


//=====================================================//


int hashTblDtor(hashTbl_t* hashtbl){
    if (hashTblVtor(hashtbl)) return ERR;
    bucket_t* next = 0;

    for (int i = 0; i < hashtbl->num_bckts; i++){
        for (bucket_t* j = hashtbl->buckets[i]; j != 0;){
            if (j->name) fprintf(stderr, "%s\n", j->name);

            bucket_t* next = j->next;

            if (j->name) free(j);

            j = next;
        }
    }

    if (hashtbl->buckets) free(hashtbl->buckets);
    free(hashtbl);

    return OK;
}


//=====================================================//


int hashTblDump(hashTbl_t* hashtbl){
    printf(GRN "========================================\n" RST);
    printf("hashtbl ptr:\t%p\n", hashtbl);
    printf("buckets:\t%d\n"    , hashtbl->num_bckts);
    printf(GRN "========================================\n" RST);
    for (int i = 0; i < hashtbl->num_bckts; i++){
        printf(BLU "bucketnum_%d:\n" RST, i);
        for (bucket_t* j = hashtbl->buckets[i]; j != 0; j = j->next){
            printf("%p:\tname:%p(%s), next:%p\t\n",j, j->name, j->name, j->next);
        }
        printf("\n\n");
    }
    printf(GRN "========================================\n" RST);

    return OK;
}


//=====================================================//


int hashTblAdd(hashTbl_t* hashtbl, const char* name){
    if (hashTblVtor(hashtbl)) return ERR;

    int hash = countHash(name);

    bucketAdd(hashtbl->buckets + hash % hashtbl->num_bckts, name);

    return OK;
}

//=====================================================//

int bucketAdd(bucket_t** list, const char* name){
    if (listFind(*list, name)) return OK;

    bucket_t* old_list = *list;

    *list = (bucket_t*)calloc(1, sizeof(bucket_t));

    int len = strlen(name) % MAX_NAME;
    memcpy((*list)->name, name, len);

    (*list)->next = old_list;

    return OK;
}
//=====================================================//

int hashTblFind(hashTbl_t* hashtbl, const char* name){
    if (hashTblVtor(hashtbl)) return ERR;

    int hash = countHash(name);

    int found = listFind(hashtbl->buckets[hash % hashtbl->num_bckts], name);
    if (found) printf(CYN "%s found!\n" RST, name);
    else       printf(MAG "%s not found!\n" RST, name);

    return OK;
}

//=====================================================//

int countHash(const char* name){
    return (int)strlen(name);
}

//=====================================================//

int listFind(bucket_t* list, const char* name){
    int found = 0;

    for (bucket_t* j = list; j != 0; j = j->next){
        if (!strncmp(j->name, name, MAX_NAME)){
            found = 1;
            break;
        };
    }

    return found;
}

//=====================================================//

size_t getFileSize (const char* filename){
    size_t size = 0;

    struct stat st;
    stat(filename, &st);
    size = st.st_size;

    return size;
}

//=====================================================//

int textParse(hashTbl_t* hashtbl, const char* file){
    FILE* fileIn = fopen(file, "r");
    size_t fileSize = getFileSize(file);

    printf("fileSize:%ld\n", fileSize);

    char* buffer = (char*)calloc(fileSize, sizeof(char));
    fread(buffer, sizeof(char), fileSize, fileIn);


    for (int i = 0; i < fileSize; i++){
        if(isalpha(buffer[i])){
            hashTblAdd(hashtbl, buffer + i);
            while(isalpha(buffer[i])) i++;
        }
    }

    free(buffer);
    fclose(fileIn);

    return OK;
}
