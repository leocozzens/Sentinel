#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

_Bool file_init_store(int storeSize);
_Bool file_print(char *fileName, char *formatString, ...);
void file_close_all(void);

#endif