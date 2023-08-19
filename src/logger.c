// C standard library
#include <stdio.h>
#include <unistd.h>
// Local headers
#include <logger.h>

#define EXAMPLE_IP      "192.168.0.1"
#define EXAMPLE_PROTO   "TCP"
#define ENDL            '\n'

static FILE *init_logger(char *fileName) {
    return fopen(fileName, "w");
}

void *run_logger(void *arg) {
    FileData *fd = arg;
    FILE *outFile = init_logger(fd->fileName);
    int i = 0;
    while(i++ < 5) {
        fprintf(outFile, fd->formatString, EXAMPLE_IP, EXAMPLE_PROTO, ENDL);
        sleep(1);
    }
    fclose(outFile);
    return NULL;
}