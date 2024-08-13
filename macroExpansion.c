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

Boolean expandMacros(char fileName[], Macro **macros) {
    Boolean isSuccessful; /* Whether the macro expansion is successful. */
    FILE *inputFile;      /* The .as file. */
    FILE *outputFile;     /* The .am file. */

    /* Try to open the .as file. */
    inputFile = openFile(fileName, "as", "r");

    /* Check if the .as file could not be opened. */
    if (inputFile == NULL) {
        return FALSE;
    }

    /* Try to open the .am file. */
    outputFile = openFile(fileName, "am", "w");

    /* Check if the .am file could not be opened. */
    if (outputFile == NULL) {
        /* Close the open .as file. */
        fclose(inputFile);
        return FALSE;
    }

    /* Expand the macros in the .as file into the .am file. */
    isSuccessful = expandFileMacros(inputFile, outputFile, macros, fileName);

    /* Close both files. */
    fclose(inputFile);
    fclose(outputFile);

    /* Return if the macro expansion was successful. */
    return isSuccessful;
}

Boolean expandFileMacros(FILE *inputFile, FILE *outputFile, Macro **macros,
                         char fileName[]) {
    Boolean isSuccessful;  /* Whether the macro expansion is successful. */
    Boolean isInsideMacro; /* Whether the current line is part of a macro. */
    LineNumber lineNumber; /* The current line's line number. */
    char line[MAX_LINE_LENGTH + NEWLINE_BYTE + NULL_BYTE]; /* Current line. */

    /* Initialize the necessary variables. */
    isSuccessful = TRUE;
    isInsideMacro = FALSE;
    lineNumber = INITIAL_VALUE;

    /* Read each line of the .as file. */
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        /* Update the line number. */
        lineNumber++;

        /* Check if the line is too long (more than 80 characters). */
        if (line[strlen(line) - LAST_INDEX_DIFF] != '\n' && !feof(inputFile)) {
            /* Print an error message. */
            printMacroError(
                "Line is too long. Maximum length is 80 characters (including "
                "whitespace, not including the newline character).",
                fileName, lineNumber);
            isSuccessful = FALSE;

            /* Skip the rest of the line. */
            while (getc(inputFile) != '\n' && !feof(inputFile));
            /* Move on to the next line. */
            continue;
        }

        /* Handle the current line. */
        if (!expandLineMacros(inputFile, outputFile, macros, fileName, line,
                              lineNumber, &isInsideMacro)) {
            /* Finish the macro expansion and stop, in case of an error. */
            isSuccessful = FALSE;
        }
    }

    /* Return if the macro expansion was successful. */
    return isSuccessful;
}

Boolean expandLineMacros(FILE *inputFile, FILE *outputFile, Macro **macros,
                         char fileName[], char line[], LineNumber lineNumber,
                         Boolean *isInsideMacro) {
    char *token; /* The first token in the line. */

    /* Check if the line is part of a macro. */
    if (*isInsideMacro) {
        return handleInsideMacro(*macros, fileName, line, lineNumber,
                                 isInsideMacro);
    }

    /* Check if the line is empty. */
    if (*skipWhitespace(line) == '\0') {
        /* Write the line to the .am file. */
        fputs(line, outputFile);
        return TRUE;
    }

    /* Get the first token in the line. */
    token = getNextToken(skipWhitespace(line));

    /* Check if the first token is "endmacr", but not after a macro. */
    if (strcmp(token, "endmacr") == EQUAL_STRINGS) {
        /* This is invalid. */
        free(token);
        printMacroError("End of macro definition without declaring a macro.",
                        fileName, lineNumber);

        return FALSE;
    }

    /* Check if the line is the start of a macro definition. */
    if (strcmp(token, "macr") == EQUAL_STRINGS) {
        free(token);

        /* Check if the definition is valid. */
        if (!validateMacroDefinition(line, fileName, lineNumber)) {
            return FALSE;
        }

        /* Perform some more validation and add the macro if it is valid. */
        *isInsideMacro =
            handleMacroDefinition(macros, fileName, line, lineNumber);

        /* isInsideMacro should be TRUE, otherwise the macro is invalid. */
        return *isInsideMacro;
    }

    /* Decide whether to write the line or expand a macro. */
    writeLine(outputFile, *macros, line, token);
    /* The token is no longer used. */
    free(token);

    return TRUE;
}

Boolean handleMacroDefinition(Macro **macros, char fileName[], char line[],
                              LineNumber lineNumber) {
    char *macroName; /* The name of the macro. */

    /* Skip the "macr" keyword to get the macro name. */
    line = skipWhitespace(line);
    line = skipCharacters(line);
    line = skipWhitespace(line);

    /* Get the macro name. */
    macroName = getNextToken(line);

    /* Check if the macro name does not follow the rules. */
    if (!validateMacro(macroName, fileName, lineNumber)) {
        free(macroName);
        return FALSE;
    }

    /* Check if the macro is already defined. */
    if (getMacroContent(*macros, macroName) != NULL) {
        free(macroName);
        printMacroError("Macro with the same name already defined.", fileName,
                        lineNumber);
        return FALSE;
    }

    /* After all the checks, add the macro. */
    addMacro(macros, macroName);
    return TRUE;
}

Boolean handleInsideMacro(Macro *macro, char fileName[], char line[],
                          LineNumber lineNumber, Boolean *isInsideMacro) {
    char *token; /* The first token in the line. */

    /* Get the first token. */
    token = getNextToken(skipWhitespace(line));

    /* Check if the line is the end of the macro. */
    if (token != NULL && strcmp(token, "endmacr") == EQUAL_STRINGS) {
        free(token);
        /* Stop the macro definition. */
        *isInsideMacro = FALSE;

        /* Check if this "endmacr" line is valid. */
        return validateMacroEnd(line, fileName, lineNumber);
    }

    /* Free the token if it is not empty. */
    if (token != NULL) {
        free(token);
    }

    /* Add the line to the macro's content. */
    addMacroContent(macro, line);
    return TRUE;
}

void writeLine(FILE *outputFile, Macro *macros, char line[], char token[]) {
    char *content; /* The macro's content. */

    /* Check if the given line consists of a single token. */
    if (*skipWhitespace(skipCharacters(skipWhitespace(line))) == '\0') {
        /* Check if the token is an already-defined macro's name. */
        content = getMacroContent(macros, token);
        /* If so, expand the macro's content. If not, write the actual line. */
        fputs(content == NULL ? line : content, outputFile);

        return;
    }

    /* Write the line, as it definitely is not a call to a macro. */
    fputs(line, outputFile);
}
