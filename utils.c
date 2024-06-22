#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "globals.h"

void *allocate(size_t size) {
    void *allocatedPointer;

    allocatedPointer = malloc(size);

    if (allocatedPointer == NULL) {
        fprintf(stderr, "Failed to allocate enough memory.\n");
        exit(1);
    }

    return allocatedPointer;
}

FILE *openFile(char fileName[], char mode[]) {
    FILE *file;

    file = fopen(fileName, mode);

    if (file == NULL) {
        fprintf(stderr, "Failed to open a file: %s.\n", fileName);
        exit(1);
    }

    return file;
}

char *skipWhitespace(char line[]) {
    while (isspace(*line)) {
        line++;
    }
    return line;
}

char *skipCharacters(char line[]) {
    while (!isspace(*line)) {
        line++;
    }
    return line;
}

char *getNextToken(char line[]) {
    size_t size;
    char *token;

    size = skipCharacters(line) - line;

    if (size == 0) {
        return NULL;
    }

    token = allocate(sizeof(char) * (size + 1));
    strncpy(token, line, size);
    return token;
}

void removeEndingComma(char string[]) {
    size_t length;

    length = strlen(string);

    if (string[length - 1] == ',') {
        string[length - 1] = '\0';
    }
}

char *addExtension(char fileName[], char extension[]) {
    size_t fileNameLength;
    size_t extensionLength;
    char *name;

    fileNameLength = strlen(fileName);
    extensionLength = strlen(extension);

    name = allocate(sizeof(char) * (fileNameLength + extensionLength + 2));

    strcpy(name, fileName);
    name[fileNameLength] = '.';
    strcpy(&name[fileNameLength + 1], extension);
    name[fileNameLength + extensionLength + 1] = '\0';

    return name;
}

boolean contains(char string[], char *stringArray[], size_t arraySize) {
    for (; arraySize > 0; arraySize--) {
        if (strcmp(string, *stringArray) == 0) {
            return TRUE;
        }
        stringArray++;
    }

    return FALSE;
}

size_t indexOf(char string[], char *stringArray[], size_t arraySize) {
    size_t index;

    for (index = 0; index < arraySize; index++) {
        if (strcmp(string, stringArray[index]) == 0) {
            return index;
        }
    }

    return arraySize;
}
