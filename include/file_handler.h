#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#define MEM_ERR "Memory allocation error"

int file_init_store(int storeSize, const char *errFileName);
int file_print(char *fileName, const char *formatString, ...);
_Bool file_close(const char *fileName);
void file_close_all(_Bool endFiles);

#endif