// POSIX headers
#include <pthread.h>
#include <semaphore.h>
// Local headers
#include <logger.h>
#include <unistd.h>

#define QUEUE_LENGTH 2
#define QUEUE_GROWTH_FACTOR

#define FORMAT_STRING "%02d:%02d:%02d %s|%s [%s] {%s} %s%c"

#define EXAMPLE_FILE_NAME "a.test"
#define EXAMPLE_RULE_TYPE "ALERT"
#define EXAMPLE_RULE_NAME "INCOMING TRAFFIC"
#define EXAMPLE_IP        "192.168.0.1"
#define EXAMPLE_PROTO     "TCP"
#define EXAMPLE_MSG       "Detected incoming traffic from the above IP"
#define ENDL              '\n'

int main(void) {
    LogType newType[QUEUE_LENGTH];
    log_fill_type(newType, "a.test", EXAMPLE_RULE_TYPE, EXAMPLE_RULE_NAME, EXAMPLE_IP, EXAMPLE_PROTO, EXAMPLE_MSG, ENDL);
    log_fill_type(newType + 1, "b.test", EXAMPLE_RULE_TYPE, EXAMPLE_RULE_NAME, EXAMPLE_IP, EXAMPLE_PROTO, EXAMPLE_MSG, ENDL);
    LogData lData = { FORMAT_STRING, .runLogger = 1 };
    log_queue_init(&lData.lQueue);
    sem_init(&lData.logCount, 0, 0);

    pthread_t logger, watchdog;
    pthread_create(&logger, NULL, run_logger, &lData);
    log_enqueue(&lData.lQueue, newType);
    log_enqueue(&lData.lQueue, newType + 1);
    sem_post(&lData.logCount);
    sem_post(&lData.logCount);
    close_logger(&lData.logCount, &lData.runLogger);

    pthread_join(logger, NULL);
    return 0;
}