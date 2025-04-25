#include "../hpp/hashtable_v1.hpp"
#include "../hpp/colors.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <ctype.h>
#include <immintrin.h>
#include <math.h>
#include <stdint.h>

#define MEOW fprintf(stderr, "MEOW\n");

const int NUM_BCKT      = 1400;
const int TEST_ITER     = 5000;
const int POLYNOM       = 0xEDB88320;

int         hashTblCtor     (hashTbl_t** hashtbl);
int         hashTblVtor     (hashTbl_t*  hashtbl);
int         hashTblDtor     (hashTbl_t*  hashtbl);
int         hashTblDump     (hashTbl_t*  hashtbl);
int         hashTblAdd      (hashTbl_t* hashtbl, char* name);
int         hashTblFind     (hashTbl_t* hashtbl, char* name);
int         listFind        (hashTbl_t* hashtbl, bucket_t*  list,    char* name);
int         bucketAdd       (hashTbl_t* hashtbl, bucket_t** list,    char* name);
int         textParse       (hashTbl_t* hashtbl, const char* file);
size_t      getFileSize     (const char* filename);
double      doTest          (hashTbl_t* hashtbl, const char* name);
int         checkBucketSizes(hashTbl_t* hashtbl);
int         crc32_bitwise   (const void* data, size_t length);

unsigned long long    countHash  (char* name);
extern "C" int        meowcmp    (char* a, char* b);
extern "C" int        strlen_memcpy    (char* to, char* from);

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
            //if (j->name) fprintf(stderr, "%s\n", j->name);

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

    checkBucketSizes(hashtbl);

    return OK;
}


//=====================================================//


int hashTblAdd(hashTbl_t* hashtbl, char* name){
    if (hashTblVtor(hashtbl)) return ERR;

    char smallBuff[MAX_NAME] = {};

    strlen_memcpy(smallBuff, name);

    unsigned long long hash = countHash(smallBuff);
    bucketAdd(hashtbl, hashtbl->buckets + hash % NUM_BCKT, smallBuff);

    return OK;
}

//=====================================================//

int bucketAdd(hashTbl_t* hashtbl, bucket_t** list, char* name){
    if (listFind(hashtbl, *list, name)) return OK;

    bucket_t* old_list = *list;

    *list = (bucket_t*)calloc(1, sizeof(bucket_t));

    // int len = strlen(name) % MAX_NAME;
    //memcpy((*list)->name, name, MAX_NAME);
    asm(".intel_syntax noprefix\n"
        "vmovups ymm0, ymmword ptr [rdi]\n"
        "vmovups ymmword ptr [rsi], ymm0\n"
        ".att_syntax prefix\n"
        : /*no input*/
        : "D"(name), "S"((*list)->name)
        : "ymm0"
        );

    (*list)->next = old_list;

    return OK;
}
//=====================================================//

int hashTblFind(hashTbl_t* hashtbl, char* name){
    if (hashTblVtor(hashtbl)) return ERR;

    volatile unsigned long long hash = countHash(name);

    volatile int found = listFind(hashtbl, hashtbl->buckets[hash % NUM_BCKT], name);
    if (!found) printf(CYN "%s not found!\n" RST, name);
    //else       printf(MAG "%s not found!\n" RST, name);

    return OK;
}

//=====================================================//

unsigned long long countHash(char* name){

    unsigned long long crc = 0xFFFFFFFFFFFFFFFF; // same as previousCrc32 ^ 0xFFFFFFFF
    for (int i = 0; i < MAX_NAME / 8; i++){
        unsigned long long next = *((unsigned long long*)name + i);
        crc = _mm_crc32_u64(crc, next);
    }

    return crc;
}

//=====================================================//

int listFind(hashTbl_t* hashtbl, bucket_t* list, char* name){
    int found = 0;

    for (bucket_t* j = list; j != 0; j = j->next){
        if (!(~meowcmp(j->name, name))){
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

    char* buffer = (char*)calloc(fileSize, sizeof(char));
    int check = fread(buffer, sizeof(char), fileSize, fileIn);
    if (!check) abort();

    for (int i = 0; i < fileSize; i++){
        if(isalpha(buffer[i])){
            hashTblAdd(hashtbl, buffer + i);
            while(isalpha(buffer[i])){
                i++;
            }
        }
    }

    free(buffer);
    fclose(fileIn);

    return OK;
}

//=====================================================//

double doTest (hashTbl_t* hashtbl, const char* name){

    FILE* fileIn = fopen(name, "r");
    size_t size = getFileSize(name);

    char* buffer = (char*)calloc(size, sizeof(*buffer));
    int check = fread(buffer, sizeof(char), size, fileIn);
    if (!check) abort();

    unsigned long long start_rdtsc = __rdtsc();

    for (int j = 0; j < TEST_ITER; j++){
        for (int i = 0; i < size / MAX_NAME; i++){
            hashTblFind(hashtbl, buffer + i * MAX_NAME);
        }
    }

    unsigned long long end_rdtsc = __rdtsc();

    double time = (double)(end_rdtsc - start_rdtsc) / (double)TIME_SCALE;
    free(buffer);
    fclose(fileIn);

    return time;
}

//=====================================================//

int checkBucketSizes(hashTbl_t* hashtbl){
    int* buff = (int*)calloc(NUM_BCKT, sizeof(*buff));
    float sum = 0;

    for (int i = 0; i < NUM_BCKT; i++){
        for (bucket_t* j = hashtbl->buckets[i]; j != 0; j = j->next){
            buff[i]++;
        }
    }

    for (int i = 0; i < NUM_BCKT; i++){
        printf("i(%4.0d):%2.0d, ", i, buff[i]);
        sum += buff[i];
    }

    float loadFactor = sum / NUM_BCKT;

    free(buff);

    printf("load factor:%g\n", loadFactor);

    return OK;
}

//=====================================================//
