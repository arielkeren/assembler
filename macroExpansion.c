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

/**
 * Handles a potential macro definition in the given line.
 * Returns whether or not the definition is valid.
 * If the definition is valid, adds the macro.
 * If the definition is invalid, prints an error message.
 *
 * Assumes that the given pointer to the macros is not NULL.
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that the given line is not NULL and is null-terminated.
 *
 * @param macros The macro table.
 * @param fileName The name of the source file.
 * @param line The current line.
 * @param lineNumber The current line's line number.
 * @return TRUE if the definition is valid, FALSE otherwise.
 */
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

/**
 * Handles lines that are part of a macro definition.
 * Sets the isInsideMacro flag to FALSE if the given line is the macro's end.
 * Returns whether or not the line is valid.
 * If so and if it is not the end, adds the line to the macro's content.
 *
 * Assumes that the given macro pointer is not NULL.
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that the given line is not NULL and is null-terminated.
 * Assumes that the given isInsideMacro pointer is not NULL.
 *
 * @param macro The macro that this line is a part of.
 * @param fileName The name of the source file.
 * @param line The current line.
 * @param lineNumber The current line's line number.
 * @param isInsideMacro The pointer to the isInsideMacro flag.
 */
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

/**
 * If the given line contains ONLY an already-defined macro's name, expands the
 * macro's content into the given output file.
 * Otherwise, writes the given line into the given output file.
 *
 * Assumes that the given output file pointer is not NULL.
 * Assumes that the given line is not NULL and is null-terminated.
 * Assumes that the given token is not NULL and is null-terminated.
 *
 * @param outputFile The .am file pointer.
 * @param macros The macro table.
 * @param line The current line.
 * @param token The first token in the line (to check for a macro).
 */
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
