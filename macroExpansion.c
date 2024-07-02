#include "macroExpansion.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "lineValidation.h"
#include "macroTable.h"
#include "utils.h"

void expandMacros(char fileName[], macro **macros) {
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

    expandFileMacros(inputFile, outputFile, macros);

    fclose(inputFile);
    fclose(outputFile);
}

void expandFileMacros(FILE *inputFile, FILE *outputFile, macro **macros) {
    boolean isInsideMacro;
    unsigned lineNumber;
    char line[82];

    isInsideMacro = FALSE;
    lineNumber = 0;

    while (fgets(line, sizeof(line), inputFile) != NULL) {
        lineNumber++;
        expandLineMacros(inputFile, outputFile, macros, line, lineNumber, &isInsideMacro);
    }
}

void expandLineMacros(FILE *inputFile, FILE *outputFile, macro **macros, char line[], unsigned lineNumber, boolean *isInsideMacro) {
    char *current;
    char *token;
    char *content;

    if (*isInsideMacro) {
        if (isEndOfMacro(line)) {
            *isInsideMacro = FALSE;
            return;
        }

        addMacroContent(*macros, line);
        return;
    }

    current = skipWhitespace(line);

    if (*current == '\0') {
        fputs(line, outputFile);
        return;
    }

    token = getNextToken(current);

    if (strcmp(token, "endmacr") == 0) {
        free(token);
        printError("End of macro definition without declaring a macro.", lineNumber);
        return;
    }

    if (strcmp(token, "macr") == 0) {
        free(token);
        current = skipCharacters(current);
        current = skipWhitespace(current);

        if (*current == '\0') {
            printError("Macro definition without a name.", lineNumber);
            return;
        }

        if (*skipWhitespace(skipCharacters(current)) != '\0') {
            printError("Extra non-whitespace characters after the macro name.", lineNumber);
            return;
        }

        token = getNextToken(current);

        if (!validateLabel(token, lineNumber)) {
            free(token);
            return;
        }

        if (getMacroContent(*macros, token) != NULL) {
            printError("Macro with the same name already defined.", lineNumber);
        }

        addMacro(macros, token);
        *isInsideMacro = TRUE;
        return;
    }

    content = getMacroContent(*macros, token);
    if (content != NULL) {
        free(token);
        fputs(content, outputFile);
        return;
    }

    free(token);
    fputs(line, outputFile);
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
