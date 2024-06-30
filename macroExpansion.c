#include "macroExpansion.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "lineValidation.h"
#include "macroTable.h"
#include "utils.h"

void expandMacros(char fileName[]) {
    FILE *inputFile;
    FILE *outputFile;

    inputFile = openFile(fileName, "as", "r");

    if (inputFile == NULL) {
        return;
    }

    outputFile = openFile(fileName, "am", "w");

    if (outputFile == NULL) {
        return;
    }

    expandFileMacros(inputFile, outputFile);

    fclose(inputFile);
    fclose(outputFile);
}

void expandFileMacros(FILE *inputFile, FILE *outputFile) {
    macro *macroTable;
    boolean isInsideMacro;
    unsigned lineNumber;
    char line[82];
    char *current;
    char *token;
    char *content;

    macroTable = NULL;
    isInsideMacro = FALSE;
    lineNumber = 0;

    while (fgets(line, sizeof(line), inputFile) != NULL) {
        lineNumber++;

        if (isInsideMacro) {
            if (isEndOfMacro(line)) {
                isInsideMacro = FALSE;
                continue;
            }

            addMacroContent(macroTable, line);
            continue;
        }

        current = skipWhitespace(line);

        if (*current == '\0') {
            fputs(line, outputFile);
            continue;
        }

        token = getNextToken(current);

        if (strcmp(token, "endmacr") == 0) {
            free(token);
            printError("End of macro definition without declaring a macro.", lineNumber);
            continue;
        }

        if (strcmp(token, "macr") == 0) {
            free(token);
            current = skipCharacters(current);
            current = skipWhitespace(current);

            if (*current == '\0') {
                printError("Macro definition without a name.", lineNumber);
                continue;
            }

            if (*skipWhitespace(skipCharacters(current)) != '\0') {
                printError("Extra non-whitespace characters after the macro name.", lineNumber);
                continue;
            }

            token = getNextToken(current);

            if (!validateLabel(token, lineNumber)) {
                free(token);
                continue;
            }

            if (getMacroContent(macroTable, token) != NULL) {
                printError("Macro with the same name already defined.", lineNumber);
            }

            addMacro(&macroTable, token);
            isInsideMacro = TRUE;
            continue;
        }

        content = getMacroContent(macroTable, token);
        if (content != NULL) {
            free(token);
            fputs(content, outputFile);
            continue;
        }

        free(token);
        fputs(line, outputFile);
    }

    freeMacroTable(macroTable);
}

boolean isEndOfMacro(char line[]) {
    char *token;

    line = skipWhitespace(line);
    token = getNextToken(line);

    if (strcmp(token, "endmacr") != 0) {
        return FALSE;
    }

    line = skipCharacters(line);
    line = skipWhitespace(line);

    return *line == '\0';
}
