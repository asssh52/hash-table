#pragma once

#define MAX_NAME 32

enum exits{

    OK  = 0,
    ERR = 1,

};

typedef struct bucket_t{

    char name[MAX_NAME];
    bucket_t* next;

} bucket_t;

typedef struct hashTbl{

    bucket_t** buckets;
    int num_bckts;

} hashTbl_t;

int hashTblDtor (hashTbl_t* hashtbl);
int hashTblDump (hashTbl_t* hashtbl);
int hashTblAdd  (hashTbl_t* hashtbl, const char* name);
int hashTblFind (hashTbl_t* hashtbl, const char* name);
int hashTblCtor (hashTbl_t** hashtbl);
int textParse   (hashTbl_t* hashtbl, const char* file);
