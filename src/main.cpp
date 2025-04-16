#include <stdio.h>
#include "../hpp/hashtable_v1.hpp"

int main(int argc, const char* argv[]){
    const char* input = nullptr;

    if (argc == 2) input = argv[1];
    else printf("usage: ./main input_file\n");

    hashTbl_t* hashtbl = 0;
    hashTblCtor(&hashtbl);

    printf("file:%s\n", argv[1]);
    if (input) textParse(hashtbl, input);

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
