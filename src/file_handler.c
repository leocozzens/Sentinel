// C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <time.h>

typedef struct {
    FILE **filePtrs;
    char **fileNames;
    unsigned int activeFiles;
    unsigned int storeSize;
} FileStore;

// Static functions
static FileStore outFiles;
static FILE *open_file(char *fileName) {
    return fopen(fileName, "a");
}

// Public functions
_Bool file_init_store(int storeSize) {
    outFiles.filePtrs = malloc(sizeof(FILE*) * storeSize);
    if(outFiles.filePtrs == NULL) return 1;
    outFiles.fileNames = malloc(sizeof(char*) * storeSize);
    if(outFiles.fileNames == NULL) return 1;
    outFiles.activeFiles = 0;
    outFiles.storeSize = storeSize;
    return 0;
}

_Bool file_print(char *fileName, char *formatString, ...) {
    int fileIndex;
    _Bool found = 0;
    for(int i = 0; i < outFiles.activeFiles; i++) {
        if(strcmp(fileName, outFiles.fileNames[i]) == 0) {
            fileIndex = i;
            found = 1;
            break;
        }
    }
    if(!found) {
        outFiles.filePtrs[outFiles.activeFiles] = open_file(fileName);
        if(outFiles.filePtrs[outFiles.activeFiles] == NULL) return 1;
        outFiles.fileNames[outFiles.activeFiles] = fileName;
        fileIndex = outFiles.activeFiles++;
    }
    va_list printArgs;
    va_start(printArgs, formatString);
    vfprintf(outFiles.filePtrs[fileIndex], formatString, printArgs);
    va_end(printArgs);
    fflush(outFiles.filePtrs[fileIndex]);
    return 0;
}

void file_close_all(void) {
    for(int i = 0; i < outFiles.activeFiles; i++) {
        fclose(outFiles.filePtrs[i]);
    }
}