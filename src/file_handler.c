// C standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <time.h>

typedef struct {
    FILE **filePtrs;
    char **fileNames;
    int activeFiles;
    int storeSize;
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
    outFiles.filePtrs[0] = open_file("a.test");
    outFiles.activeFiles++;
    return 0;
}

_Bool file_print(char *fileName, char *formatString, ...) {
    va_list args;
    va_start(args, formatString);
    vfprintf(outFiles.filePtrs[0], formatString, args);
    va_end(args);
    fflush(outFiles.filePtrs[0]);
    return 0;
}

void file_close_all(void) {
    for(int i = 0; i < outFiles.activeFiles; i++) {
        fclose(outFiles.filePtrs[i]);
    }
}