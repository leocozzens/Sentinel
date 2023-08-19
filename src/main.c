// POSIX headers
#include <pthread.h>
// Local headers
#include <logger.h>

#define FILE_NAME "a.test"
#define FORMAT_STRING "IP: %s, PROTOCOL: %s%c"

int main(void) {
    FileData fd = { FILE_NAME, FORMAT_STRING };
    pthread_t logger;
    pthread_create(&logger, NULL, run_logger, &fd);
    pthread_join(logger, NULL);
    return 0;
}