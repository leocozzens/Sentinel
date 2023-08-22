#ifndef LOGGER_H
#define LOGGER_H

typedef pthread_mutex_t _pthread_mutex_t;
typedef sem_t _sem_t;
struct tm;

typedef struct _LogType {
    char *fileName;
    struct tm *localTime;
    char *ruleType;
    char *ruleName;
    char *address;
    char *protocol;
    char *message;
    struct _LogType *nextType;
    char endl;
    _Bool inUse;
} LogType;

typedef struct {
    LogType *head;
    LogType *tail;
    _pthread_mutex_t queueLock;
} LogQueue;

typedef struct {
    char *formatString;
    _sem_t logCount;
    LogQueue lQueue;
    _Bool runLogger;
} LogData;

void log_queue_init(LogQueue *queue);
void log_enqueue(LogQueue *lQueue, LogType *newType);
void log_fill_type(LogType *newType, char *fileName, char *ruleType, char *ruleName, char *address, char *protocol, char *msg, char endl);
void *run_logger(void *arg);
void close_logger(_sem_t *counter, _Bool *openLogger);

#endif