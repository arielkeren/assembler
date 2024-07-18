#include "macroExpansion.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "lineValidation.h"
#include "macroTable.h"
#include "utils.h"

boolean expandMacros(char fileName[], macro **macros) {
    boolean isSuccessful;
    FILE *inputFile;
    FILE *outputFile;

    inputFile = openFile(fileName, "as", "r");

    if (inputFile == NULL) {
        return FALSE;
    }

    outputFile = openFile(fileName, "am", "w");

    if (outputFile == NULL) {
        fclose(inputFile);
        return FALSE;
    }

    isSuccessful = expandFileMacros(inputFile, outputFile, macros, fileName);

    fclose(inputFile);
    fclose(outputFile);

    return isSuccessful;
}

boolean expandFileMacros(FILE *inputFile, FILE *outputFile, macro **macros, char fileName[]) {
    boolean isSuccessful;
    boolean isInsideMacro;
    unsigned lineNumber;
    char line[82];

    isSuccessful = TRUE;
    isInsideMacro = FALSE;
    lineNumber = 0;

    while (fgets(line, sizeof(line), inputFile) != NULL) {
        lineNumber++;
        isSuccessful = isSuccessful && expandLineMacros(inputFile, outputFile, macros, fileName, line, lineNumber, &isInsideMacro);
    }

    return isSuccessful;
}

boolean expandLineMacros(FILE *inputFile, FILE *outputFile, macro **macros, char fileName[], char line[], unsigned lineNumber, boolean *isInsideMacro) {
    char *current;
    char *token;
    char *content;

    if (*isInsideMacro) {
        if (isEndOfMacro(line)) {
            *isInsideMacro = FALSE;
            return TRUE;
        }

        addMacroContent(*macros, line);
        return TRUE;
    }

    current = skipWhitespace(line);

    if (*current == '\0') {
        fputs(line, outputFile);
        return TRUE;
    }

    token = getNextToken(current);

    if (strcmp(token, "endmacr") == 0) {
        free(token);
        printMacroError("End of macro definition without declaring a macro.", fileName, lineNumber);
        return FALSE;
    }

    if (strcmp(token, "macr") == 0) {
        free(token);
        current = skipCharacters(current);
        current = skipWhitespace(current);

        if (*current == '\0') {
            printMacroError("Macro definition without a name.", fileName, lineNumber);
            return FALSE;
        }

        if (*skipWhitespace(skipCharacters(current)) != '\0') {
            printMacroError("Extra non-whitespace characters after the macro name.", fileName, lineNumber);
            return FALSE;
        }

        token = getNextToken(current);

        if (!validateMacro(token, fileName, lineNumber)) {
            free(token);
            return FALSE;
        }

        if (getMacroContent(*macros, token) != NULL) {
            printMacroError("Macro with the same name already defined.", fileName, lineNumber);
            return FALSE;
        }

        addMacro(macros, token);
        *isInsideMacro = TRUE;
        return TRUE;
    }

    content = getMacroContent(*macros, token);
    if (content != NULL) {
        free(token);
        fputs(content, outputFile);
        return TRUE;
    }

    free(token);
    fputs(line, outputFile);
    return TRUE;
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
