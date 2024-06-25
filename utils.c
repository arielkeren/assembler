#include "utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
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

FILE *openFile(char fileName[], char extension[], char mode[]) {
    char *fileNameWithExtension;
    FILE *file;

    fileNameWithExtension = addExtension(fileName, extension);
    file = fopen(fileNameWithExtension, mode);

    if (file == NULL) {
        fprintf(stderr, "Failed to open a file: %s.\n", fileNameWithExtension);
        free(fileNameWithExtension);
        exit(1);
    }

    free(fileNameWithExtension);
    return file;
}

char *skipWhitespace(char line[]) {
    while (isspace(*line) || *line == ',') {
        line++;
    }
    return line;
}

char *skipCharacters(char line[]) {
    while (!isspace(*line) && *line != ',') {
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
    token[size] = '\0';
    return token;
}

void removeEnding(char string[], char ending) {
    size_t length;

    length = strlen(string);

    if (string[length - 1] == ending) {
        string[length - 1] = '\0';
    }
}

boolean checkIfLabel(char token[]) {
    return token[strlen(token) - 1] == ':';
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

boolean contains(char string[], const char *stringArray[], size_t arraySize) {
    while (arraySize > 0) {
        if (strcmp(string, *stringArray) == 0) {
            return TRUE;
        }

        stringArray++;
        arraySize--;
    }

    return FALSE;
}

size_t indexOf(char string[], const char *stringArray[], size_t arraySize) {
    size_t index;

    for (index = 0; index < arraySize; index++) {
        if (strcmp(string, stringArray[index]) == 0) {
            return index;
        }
    }

    return arraySize;
}
