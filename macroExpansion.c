/*
 * macroExpansion.c
 *
 * Contains functions to expand the macros in the .as file.
 * Generates the macro-expanded .am file, which is used in later stages of the
 * compilation.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "macroExpansion.h"

#include <stdio.h>  /* FILE, fclose, fgets, feof, getc, fputs. */
#include <stdlib.h> /* free. */
#include <string.h> /* strlen, strcmp. */

#include "errorHandling.h"  /* Printing errors. */
#include "globals.h"        /* Constants and typedefs. */
#include "lineValidation.h" /* Checking if a macro's name is valid. */
#include "macroTable.h" /* Adding a new macro, adding content to a macro and searching if a macro exists. */
#include "utils.h"      /* Opening the .as and .am files and parsing lines. */

/**
 * Expands the macros in the .as file.
 * Opens the .as for reading and the .am for writing.
 * Generates the expanded .am file, which is used in later stages of the
 * compilation.
 * Returns whether or not the macro expansion was successful.
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that the given pointer to the macros is not NULL.
 *
 * @param fileName The name of the source file.
 * @param macros The macro table.
 * @return TRUE if the macro expansion was successful, FALSE otherwise.
 */
Boolean expandMacros(char fileName[], Macro **macros) {
    Boolean isSuccessful;
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

/**
 * Expands the macros in the .as file line by line.
 * Inserts each line into the .am file if they don't contain macros.
 * Remembers definitions of macros and expands them when seen.
 * Returns whether or not the macro expansion was successful.
 *
 * Assumes that the given input file pointer is not NULL.
 * Assumes that the given output file pointer is not NULL.
 * Assumes that the given pointer to the macros is not NULL.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param inputFile The .as file.
 * @param outputFile The .am file.
 * @param macros The macro table.
 * @param fileName The name of the source file.
 * @return TRUE if the macro expansion was successful, FALSE otherwise.
 */
Boolean expandFileMacros(FILE *inputFile, FILE *outputFile, Macro **macros,
                         char fileName[]) {
    Boolean isSuccessful;
    Boolean isInsideMacro;
    LineNumber lineNumber;
    char line[MAX_LINE_LENGTH + NEWLINE_BYTE + NULL_BYTE];

    isSuccessful = TRUE;
    isInsideMacro = FALSE;
    lineNumber = INITIAL_VALUE;

    while (fgets(line, sizeof(line), inputFile) != NULL) {
        lineNumber++;

        if (line[strlen(line) - LAST_INDEX_DIFF] != '\n' && !feof(inputFile)) {
            printMacroError(
                "Line is too long. Maximum length is 80 characters (including "
                "whitespace, not including the newline character).",
                fileName, lineNumber);
            isSuccessful = FALSE;
            while (getc(inputFile) != '\n' && !feof(inputFile));
            continue;
        }

        if (!expandLineMacros(inputFile, outputFile, macros, fileName, line,
                              lineNumber, &isInsideMacro)) {
            isSuccessful = FALSE;
        }
    }

    return isSuccessful;
}

/**
 * Handles each line of the .as file.
 * Determines whether the line contains a macro defintion.
 * If so, fills in the macro's content.
 * If there is a reference to an already-defined macro, inserts its content into
 * the .am file.
 * Otherwise, just inserts the current line into the .am file.
 * Returns whether or not the handling the line was successful.
 *
 * Assumes that the given input file pointer is not NULL.
 * Assumes that the given output file pointer is not NULL.
 * Assumes that the given pointer to the macros is not NULL.
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that the given line is not NULL and is null-terminated.
 * Assumes that the given pointer to whether or not this line is inside a macro
 * is not NULL.
 *
 * @param inputFile The .as file.
 * @param outputFile The .am file.
 * @param macros The macro table.
 * @param fileName The name of the source file.
 * @param line The current line.
 * @param lineNumber The current line's line number.
 * @param isInsideMacro Whether or not the current line is part of some macro's
 * definition.
 * @return TRUE if the handling of the line was successful, FALSE otherwise.
 */
Boolean expandLineMacros(FILE *inputFile, FILE *outputFile, Macro **macros,
                         char fileName[], char line[], LineNumber lineNumber,
                         Boolean *isInsideMacro) {
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

    if (strcmp(token, "endmacr") == EQUAL_STRINGS) {
        free(token);
        printMacroError("End of macro definition without declaring a macro.",
                        fileName, lineNumber);
        return FALSE;
    }

    if (strcmp(token, "macr") == EQUAL_STRINGS) {
        free(token);
        current = skipCharacters(current);
        current = skipWhitespace(current);

        if (*current == '\0') {
            printMacroError("Macro definition without a name.", fileName,
                            lineNumber);
            return FALSE;
        }

        if (*skipWhitespace(skipCharacters(current)) != '\0') {
            printMacroError(
                "Extra non-whitespace characters after the macro name.",
                fileName, lineNumber);
            return FALSE;
        }

        token = getNextToken(current);

        if (!validateMacro(token, fileName, lineNumber)) {
            free(token);
            return FALSE;
        }

        if (getMacroContent(*macros, token) != NULL) {
            printMacroError("Macro with the same name already defined.",
                            fileName, lineNumber);
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

/**
 * Checks and returns whether the given line is the end of some macro's
 * definition.
 *
 * Assumes that the given line is not NULL and is null-terminated.
 *
 * @param line The line to check.
 * @return TRUE if the line is the end of some macro's definition, FALSE
 * otherwise.
 */
Boolean isEndOfMacro(char line[]) {
    char *token;

    line = skipWhitespace(line);
    token = getNextToken(line);

    if (strcmp(token, "endmacr") != EQUAL_STRINGS) {
        return FALSE;
    }

    line = skipCharacters(line);
    line = skipWhitespace(line);

    return *line == '\0';
}
