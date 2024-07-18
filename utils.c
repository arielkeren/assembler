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
        printCriticalError("Failed to allocate enough memory. Exiting the program...");
        exit(1);
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
        errorMessage = malloc(sizeof(char) * (strlen(fileNameWithExtension) + 53 + 1));

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

boolean checkIfFollowedByComma(char line[]) {
    while (isspace(*line)) {
        line++;
    }

    return *line == ',';
}

boolean checkForConsecutiveCommas(char line[]) {
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

    name = allocate(sizeof(char) * (fileNameLength + extensionLength + 2));

    strcpy(name, fileName);
    name[fileNameLength] = '.';
    strcpy(&name[fileNameLength + 1], extension);
    name[fileNameLength + extensionLength + 1] = '\0';

    return name;
}

void printMessage(char message[], char fileName[], unsigned lineNumber, boolean isError, boolean isMacro) {
    static unsigned errorCount = 0;
    static unsigned warningCount = 0;

    if (isError) {
        printf("\n--- Error #%u ---\n", ++errorCount);
    } else {
        printf("\n--- Warning #%u ---\n", ++warningCount);
    }

    printf("File: %s.a%c\n", fileName, isMacro ? 's' : 'm');
    printf("Line: %u\n", lineNumber);
    printf("%s\n", message);
}

void printError(char message[], char fileName[], unsigned lineNumber) {
    printMessage(message, fileName, lineNumber, TRUE, FALSE);
}

void printMacroError(char message[], char fileName[], unsigned lineNumber) {
    printMessage(message, fileName, lineNumber, TRUE, TRUE);
}

void printWarning(char message[], char fileName[], unsigned lineNumber) {
    printMessage(message, fileName, lineNumber, FALSE, FALSE);
}

void printCriticalError(char message[]) {
    printf("\n--- Critical Error ---\n");
    printf("%s\n", message);
}
