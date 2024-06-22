#ifndef UTILS_H
#define UTILS_H

void *allocate(size_t size);
FILE *openFile(char fileName[], char mode[]);
char *addExtension(char fileName[], char extension[]);

#endif
