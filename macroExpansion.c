#include "macroExpansion.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macroTable.h"

void expandMacros(char *fileNames[], int fileCount) {
    char *inputFileName;
    char *outputFileName;

    while (fileCount > 0) {
        inputFileName = addExtension(*fileNames, "as");
        outputFileName = addExtension(*fileNames, "am");
        expandFileMacros(inputFileName, outputFileName);

        free(inputFileName);
        free(outputFileName);
        fileNames++;
    }
}

void expandFileMacros(char inputFileName[], char outputFileName[]) {
    FILE *input;
    FILE *output;
    macro *macroTable;
    int isInsideMacro;
    char line[82];
    char *content;
    char *macroName;
    char *noWhitespaceLine;

    input = fopen(inputFileName, "r");
    if (input == NULL) {
        fprintf(stderr, "Failed to open a file: %s.\n", inputFileName);
        exit(1);
    }

    output = fopen(outputFileName, "w");
    if (output == NULL) {
        fprintf(stderr, "Failed to open a file: %s.\n", outputFileName);
        exit(1);
    }

    macroTable = NULL;
    isInsideMacro = 0;

    while (fgets(line, sizeof(line) * sizeof(char), input) != NULL) {
        if (isInsideMacro) {
            if (isEndOfMacro(line)) {
                isInsideMacro = 0;
                continue;
            }

            addContent(macroTable, macroName, line);
            continue;
        }

        macroName = getMacroName(line);
        if (macroName != NULL) {
            macroName = addMacro(&macroTable, macroName);
            isInsideMacro = 1;
            continue;
        }

        if (!isSingleWord(line)) {
            fputs(line, output);
            continue;
        }

        noWhitespaceLine = malloc(sizeof(char) * (strlen(line) + 1));

        if (noWhitespaceLine == NULL) {
            fprintf(stderr, "Failed to allocate enough memory for a line.\n");
            exit(1);
        }

        strcpy(noWhitespaceLine, line);
        removeEndingWhitespace(noWhitespaceLine);

        content = getMacroContent(macroTable, noWhitespaceLine);
        if (content != NULL) {
            fputs(content, output);
            continue;
        }

        fputs(line, output);
    }

    freeMacroTable(macroTable);
    fclose(input);
    fclose(output);
}

char *addExtension(char fileName[], char extension[]) {
    size_t fileNameLength;
    size_t extensionLength;
    char *name;

    fileNameLength = strlen(fileName);
    extensionLength = strlen(extension);

    name = malloc(sizeof(char) * (fileNameLength + extensionLength + 2));

    if (name == NULL) {
        fprintf(stderr, "Failed to allocate enough memory for a file name.\n");
        exit(1);
    }

    strcpy(name, fileName);
    name[fileNameLength] = '.';
    strcpy(&name[fileNameLength + 1], extension);
    name[fileNameLength + extensionLength + 1] = '\0';

    return name;
}

int isEndOfMacro(char line[]) {
    line = skipWhitespace(line);

    if (strncmp(line, "endmacr", 7) != 0) {
        return 0;
    }

    line = &line[7];
    line = skipWhitespace(line);

    return *line == '\0';
}

char *getMacroName(char line[]) {
    line = skipWhitespace(line);

    if (strncmp(line, "macr", 4) != 0) {
        return NULL;
    }

    line = &line[4];
    line = skipWhitespace(line);

    if (!isValidMacroName(line)) {
        return NULL;
    }

    removeEndingWhitespace(line);

    return line;
}

int isValidMacroName(char line[]) {
    line = skipCharacters(line);
    line = skipWhitespace(line);
    return *line == '\0';
}

void removeEndingWhitespace(char line[]) {
    line = skipCharacters(line);
    *line = '\0';
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

int isSingleWord(char line[]) {
    line = skipWhitespace(line);
    line = skipCharacters(line);
    line = skipWhitespace(line);
    return *line == '\0';
}
