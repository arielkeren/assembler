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
    size_t length;

    length = strlen(string);

    if (string[length - LAST_INDEX_DIFF] == ending) {
        string[length - LAST_INDEX_DIFF] = '\0';
    }
}

boolean checkIfLabel(char token[]) {
    return token[strlen(token) - LAST_INDEX_DIFF] == ':';
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

void printMessage(char message[], char fileName[], unsigned lineNumber, boolean isError, boolean isMacro) {
    static unsigned errorCount = INITIAL_VALUE;
    static unsigned warningCount = INITIAL_VALUE;

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
