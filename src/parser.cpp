#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>

size_t getFileSize (const char* filename){
    size_t size = 0;

    struct stat st;
    stat(filename, &st);
    size = st.st_size;

    return size;
}

int parseFile(const char* input, const char* output){
    size_t inputSize = getFileSize(input);
    char* fileBuffer = (char*)calloc(1, inputSize);

    FILE* fileIn    = fopen(input, "r");
    FILE* fileOut   = fopen(output, "w");

    fread(fileBuffer, sizeof(char), inputSize, fileIn);

    for (int i = 0; i < inputSize; i++){
        if (!isalpha(fileBuffer[i]))        fileBuffer[i] = 0;
        else if (isupper(fileBuffer[i]))    fileBuffer[i] = tolower(fileBuffer[i]);
    }

    fwrite(fileBuffer, sizeof(char), inputSize, fileOut);

    fclose(fileIn);
    fclose(fileOut);
    free(fileBuffer);
    return 0;
}

int main(int argc, const char* argv[]){
    if (argc == 3){
        const char* input  = argv[1];
        const char* output = argv[2];

        parseFile(input, output);
    }
    else{
        printf("usage: ./main input_file output_file\n");
        return 0;
    }

    return 0;
}
