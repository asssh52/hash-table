#include <stdio.h>
#include "../hpp/hashtable_v1.hpp"

int main(int argc, const char* argv[]){
    hashTbl_t* hashtbl = 0;
    hashTblCtor(&hashtbl);

    hashTblAdd(hashtbl, "meow");
    hashTblAdd(hashtbl, "mewo");
    hashTblAdd(hashtbl, "mewe");
    hashTblAdd(hashtbl, "mewowo");
    hashTblAdd(hashtbl, "mewo");
    hashTblAdd(hashtbl, "mewo");
    hashTblAdd(hashtbl, "mewo");


    hashTblFind(hashtbl, "mewo");
    hashTblFind(hashtbl, "mewo1");

    hashTblDump(hashtbl);
    hashTblDtor(hashtbl);


    return 0;
}
