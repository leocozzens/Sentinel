// POSIX headers
#include <pthread.h>
#include <semaphore.h>
// C standard library
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
// Local headers
#include <logger.h>
#include <file_handler.h>

#define FILE_STORE_INIT_SIZE 3

typedef struct tm Tm;
static Tm *take_time(void) {
    time_t currentTime = time(NULL);
    return localtime(&currentTime);
}

static void print_log(char *formatString, LogType *type) {
        file_print(type->fileName, formatString, type->localTime->tm_hour, type->localTime->tm_min, type->localTime->tm_sec, type->ruleType, type->ruleName, type->address, type->protocol, type->message, type->endl);
        type->inUse = 0;
}

static LogType *log_dequeue(LogQueue *lQueue) {
    pthread_mutex_lock(&lQueue->queueLock);
    LogType *tmp = lQueue->head;
    if(tmp == NULL) return NULL;
    lQueue->head = tmp->nextType;
    pthread_mutex_unlock(&lQueue->queueLock);
    return tmp;
}

void log_queue_init(LogQueue *lQueue) {
    lQueue->head = NULL;
    lQueue->tail = NULL;
}

void log_enqueue(LogQueue *lQueue, LogType *newType) {
    newType->nextType = NULL;
    pthread_mutex_lock(&lQueue->queueLock);
    if(lQueue->head == NULL) lQueue->head = newType;
    else lQueue->tail->nextType = newType;
    lQueue->tail = newType;
    pthread_mutex_unlock(&lQueue->queueLock);
}

void log_fill_type(LogType *newType, char *fileName, char *ruleType, char *ruleName, char *address, char *protocol, char *msg, char endl) {
    newType->localTime = take_time();
    newType->fileName = fileName;
    newType->ruleType = ruleType;
    newType->ruleName = ruleName;
    newType->address = address;
    newType->protocol = protocol;
    newType->message = msg;
    newType->endl = endl;
    newType->inUse = 1;
}

void *run_logger(void *arg) {
    LogData *lData = arg;
    int initRetval = file_init_store(FILE_STORE_INIT_SIZE, NULL);
    if(initRetval == -1) {
        fprintf(stderr, "ERROR: %s\n", MEM_ERR);
        return NULL;
    }
    else if(initRetval == -2) {
        fprintf(stderr, "ERROR: %s\n", strerror(errno));
        return NULL;
    }
    while(lData->runLogger) {
        sem_wait(&lData->logCount);
        LogType *outType = log_dequeue(&lData->lQueue);
        if(outType != NULL) print_log(lData->formatString, outType);
    }
    file_close_all(1);
    return NULL;
}

void close_logger(_sem_t *counter, _Bool *openLogger) {
    int semVal;
    do { sem_getvalue(counter, &semVal); } while(semVal != 0);
    *openLogger = 0;
    sem_post(counter);
}