#include "macroExpansion.h"

#include <stdio.h>
#include <string.h>

#include "globals.h"
#include "macroTable.h"
#include "utils.h"

void expandMacros(char fileName[]) {
    char *inputFileName;
    char *outputFileName;
    FILE *inputFile;
    FILE *outputFile;

    inputFileName = addExtension(fileName, "as");
    outputFileName = addExtension(fileName, "am");

    inputFile = openFile(inputFileName, "r");
    outputFile = openFile(outputFileName, "w");

    expandFileMacros(inputFileName, outputFileName);

    free(inputFileName);
    free(outputFileName);
    fclose(inputFile);
    fclose(outputFile);
}

void expandFileMacros(FILE *inputFile, FILE *outputFile) {
    macro *macroTable;
    int isInsideMacro;
    char line[82];
    char *content;
    char *macroName;
    char *noWhitespaceLine;

    macroTable = NULL;
    isInsideMacro = 0;

    while (fgets(line, sizeof(line), inputFile) != NULL) {
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
            fputs(line, outputFile);
            continue;
        }

        noWhitespaceLine = allocate(sizeof(char) * (strlen(line) + 1));

        strcpy(noWhitespaceLine, line);
        removeEndingWhitespace(noWhitespaceLine);

        content = getMacroContent(macroTable, noWhitespaceLine);
        if (content != NULL) {
            fputs(content, outputFile);
            continue;
        }

        fputs(line, outputFile);
    }

    freeMacroTable(macroTable);
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

int isSingleWord(char line[]) {
    line = skipWhitespace(line);
    line = skipCharacters(line);
    line = skipWhitespace(line);
    return *line == '\0';
}
