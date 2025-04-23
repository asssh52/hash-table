#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <immintrin.h>
#include "../hpp/hashtable_v1.hpp"

int main(int argc, const char* argv[]){
    unsigned long long start_rdtsc = __rdtsc();
    const char* input = nullptr;

    if (argc == 2) input = argv[1];
    else printf("usage: ./main input_file\n");

    hashTbl_t* hashtbl = 0;
    hashTblCtor(&hashtbl);

    printf("file:%s\n", argv[1]);
    if (input) textParse(hashtbl, input);

    //hashTblDump(hashtbl);

    double timeTest = doTest(hashtbl, "test.txt");

    unsigned long long end_rdtsc = __rdtsc();
    double timeMain = (double)(end_rdtsc - start_rdtsc) / (double)TIME_SCALE;

    printf("MAIN: cycles passed * 10**9:\t%lf\n", timeMain);
    printf("TEST: cycles passed * 10**9:\t%lf\n", timeTest);

    hashTblDtor(hashtbl);


    return 0;
}
