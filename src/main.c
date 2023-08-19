// POSIX headers
#include <pthread.h>
// Local headers
#include <logger.h>

#define FILE_NAME "a.test"
#define FORMAT_STRING "IP: %s, PROTOCOL: %s%c"

int main(void) {
    LogData ld = { FILE_NAME, FORMAT_STRING };
    pthread_t logger, watchdog;
    pthread_create(&logger, NULL, run_logger, &ld);
    pthread_join(logger, NULL);
    return 0;
}