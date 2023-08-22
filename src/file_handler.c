// C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#define GROWTH_FACTOR 3

#define MEM_ERR "Memory allocation error"

typedef struct {
    FILE **filePtrs;
    char **fileNames;
    unsigned int activeFiles;
    unsigned int storeSize;
} FileStore;

// Static functions
static FileStore outFiles;
static FILE *errFile;

char *duplicate(const char *src) {
    char *dst = malloc(strlen(src) + 1);
    if (dst == NULL) return NULL;
    strcpy(dst, src); 
    return dst;
}

static FILE *open_file(const char *fileName) {
    return fopen(fileName, "a");
}

static void print_err(const char *errMsg) {
    fprintf(errFile, "ERROR: %s\n", errMsg);
    fflush(errFile);
}

static _Bool realloc_files(FILE ***arr, unsigned int newSize) {
    FILE **tmp = realloc(*arr, newSize * sizeof(FILE*));
    if(tmp == NULL) return 1;
    *arr = tmp;
    return 0;
}

static _Bool realloc_names(char ***arr, unsigned int newSize) {
    char **tmp = realloc(*arr, newSize * sizeof(char*));
    if(tmp == NULL) return 1;
    *arr = tmp;
    return 0;
}

// Public functions
int file_init_store(int storeSize, const char *errFileName) {
    outFiles.filePtrs = malloc(sizeof(FILE*) * storeSize);
    if(outFiles.filePtrs == NULL) return -1;
    outFiles.fileNames = malloc(sizeof(char*) * storeSize);
    if(outFiles.fileNames == NULL) {
        free(outFiles.filePtrs);
        return -1;
    }
    if(errFileName == NULL) errFile = stdout;
    else {
        errFile = open_file(errFileName);
        if(errFile == NULL) {
            free(outFiles.filePtrs);
            free(outFiles.fileNames);
            return -2;
        }
    }
    outFiles.activeFiles = 0;
    outFiles.storeSize = storeSize;
    return 0;
}

int file_print(char *fileName, const char *formatString, ...) {
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
        if(outFiles.activeFiles >= outFiles.storeSize) {
            if(realloc_files(&outFiles.filePtrs, outFiles.storeSize * GROWTH_FACTOR)) {
                print_err(MEM_ERR);
                return -1;
            }
            if(realloc_names(&outFiles.fileNames, outFiles.storeSize * GROWTH_FACTOR)) {
                print_err(MEM_ERR);
                return -1;
            }
            outFiles.storeSize *= GROWTH_FACTOR;
        }
        outFiles.filePtrs[outFiles.activeFiles] = open_file(fileName);
        if(outFiles.filePtrs[outFiles.activeFiles] == NULL) {
            print_err(strerror(errno));
            return -2;
        }
        outFiles.fileNames[outFiles.activeFiles] = duplicate(fileName);
        if(outFiles.fileNames[outFiles.activeFiles] == NULL) {
            fclose(outFiles.filePtrs[outFiles.activeFiles]);
            print_err(MEM_ERR);
            return -1;
        }
        fileIndex = outFiles.activeFiles++;
    }
    va_list printArgs;
    va_start(printArgs, formatString);
    vfprintf(outFiles.filePtrs[fileIndex], formatString, printArgs);
    va_end(printArgs);
    fflush(outFiles.filePtrs[fileIndex]);
    return 0;
}

_Bool file_close(const char *fileName) {
    for(int i = 0; i < outFiles.activeFiles; i++) {
        if(strcmp(fileName, outFiles.fileNames[i]) == 0) {
            fclose(outFiles.filePtrs[i]);
            free(outFiles.fileNames[i]);
            for(int j = i; j < --outFiles.activeFiles; j++) {
                outFiles.filePtrs[j] = outFiles.filePtrs[j + 1];
                outFiles.fileNames[j] = outFiles.fileNames[j + 1];
            }
            return 1;
        }
    }
    return 0;
}

void file_close_all(_Bool endFiles) {
    for(int i = 0; i < outFiles.activeFiles; i++) {
        fclose(outFiles.filePtrs[i]);
        free(outFiles.fileNames[i]);
    }
    outFiles.activeFiles = 0;
    if(endFiles) {
        free(outFiles.fileNames);
        outFiles.fileNames = NULL;
        free(outFiles.filePtrs);
        outFiles.filePtrs = NULL;
        if(errFile != stdout) fclose(errFile);
        errFile = NULL;
    }
}