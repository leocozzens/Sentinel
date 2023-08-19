#ifndef LOGGER_H
#define LOGGER_H

typedef struct {
    char *fileName;
    char *formatString;
} FileData;

void *run_logger(void *arg);

#endif