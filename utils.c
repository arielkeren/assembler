/*
 * utils.c
 *
 * Contains utility functions that are used in the program:
 * - Allocating memory
 * - Opening files
 * - Parsing lines
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errorHandling.h"
#include "freeingLogic.h"
#include "globals.h"

void *allocate(size_t size) {
    void *allocatedPointer;

    allocatedPointer = malloc(size);

    if (allocatedPointer == NULL) {
        printCriticalError("Failed to allocate enough memory. Exiting the program...");
        freeAll();
        exit(ERROR);
    }

    return allocatedPointer;
}

FILE *openFile(char fileName[], char extension[], char mode[]) {
    char *fileNameWithExtension;
    char *errorMessage;
    FILE *file;

    fileNameWithExtension = addExtension(fileName, extension);
    file = fopen(fileNameWithExtension, mode);

    if (file == NULL) {
        errorMessage = malloc(sizeof(char) * (strlen(fileNameWithExtension) + ERROR_MESSAGE_LENGTH + NULL_BYTE));

        sprintf(errorMessage, "Failed to open file: %s. Moving on to the next file...", fileNameWithExtension);
        printCriticalError(errorMessage);

        free(errorMessage);
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
    while (*line != '\0' && !isspace(*line) && *line != ',') {
        line++;
    }

    return line;
}

char *getNextToken(char line[]) {
    size_t size;
    char *token;

    size = skipCharacters(line) - line;

    if (size == NO_TOKEN) {
        return NULL;
    }

    token = allocate(sizeof(char) * (size + NULL_BYTE));
    strncpy(token, line, size);
    token[size] = '\0';
    return token;
}

void removeEnding(char string[], char ending) {
    Length length;

    length = (Length)strlen(string);

    if (string[length - LAST_INDEX_DIFF] == ending) {
        string[length - LAST_INDEX_DIFF] = '\0';
    }
}

Length getStringLength(char string[]) {
    Length length;

    for (length = (Length)strlen(string) - LAST_INDEX_DIFF; length >= SECOND_INDEX; length--) {
        if (string[length] == '\"') {
            return length - LAST_INDEX_DIFF;
        }
    }

    return INVALID_STRING_LENGTH;
}

Boolean checkStringEnding(char string[]) {
    Index index;

    for (index = (Index)strlen(string) - LAST_INDEX_DIFF; index >= SECOND_INDEX; index--) {
        if (string[index] == '\"') {
            return TRUE;
        }

        if (!isspace(string[index])) {
            return FALSE;
        }
    }

    return FALSE;
}

Boolean checkIfLabel(char token[]) {
    return token[strlen(token) - LAST_INDEX_DIFF] == ':';
}

Boolean checkIfFollowedByComma(char line[]) {
    while (isspace(*line)) {
        line++;
    }

    return *line == ',';
}

Boolean checkForConsecutiveCommas(char line[]) {
    while (isspace(*line)) {
        line++;
    }

    if (*line != ',') {
        return FALSE;
    }

    line++;

    while (isspace(*line)) {
        line++;
    }

    return *line == ',';
}

char *addExtension(char fileName[], char extension[]) {
    size_t fileNameLength;
    size_t extensionLength;
    char *name;

    fileNameLength = strlen(fileName);
    extensionLength = strlen(extension);

    name = allocate(sizeof(char) * (fileNameLength + DOT_BYTE + extensionLength + NULL_BYTE));

    strcpy(name, fileName);
    name[fileNameLength] = '.';
    strcpy(&name[fileNameLength + DOT_BYTE], extension);
    name[fileNameLength + DOT_BYTE + extensionLength] = '\0';

    return name;
}

unsigned char convertDigitToNumber(char digit) {
    if (!isdigit(digit)) {
        return INVALID_DIGIT;
    }

    return (unsigned char)(digit - '0');
}
