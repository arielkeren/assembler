/*
 * utils.c
 *
 * Contains utility functions that are used in the program:
 * - Allocating memory
 * - Opening files
 * - Parsing lines
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "utils.h"

#include <ctype.h>  /* isdigit and isspace. */
#include <stdio.h>  /* fopen, sprintf. */
#include <stdlib.h> /* malloc, free, exit. */
#include <string.h> /* strlen, strcpy, strncpy. */

#include "errorHandling.h" /* Printing errors. */
#include "freeingLogic.h"  /* Freeing memory. */
#include "globals.h"       /* Constants and typedefs. */

/**
 * Tries to allocate memory of the given size on the heap.
 * If succeeds, returns the pointer to the allocated memory.
 * If fails, prints an error message, frees all memory used by the linked lists
 * and exits.
 * NOTE: Does not initialize the allocated memory.
 * IMPORTANT: The caller must free the returned memory.
 *
 * Assumes that the given size is not zero.
 *
 * @param size The size of the memory to allocate in bytes.
 * @return A pointer to the allocated memory if successful.
 */
void *allocate(size_t size) {
    void *allocatedPointer; /* The pointer to the allocated memory. */

    /* Allocate the desired amount of memory. */
    allocatedPointer = malloc(size);

    /* Check if the allocation has failed. */
    if (allocatedPointer == NULL) {
        /* Print an allocation failure error message. */
        printAllocationError();
        /* Free all the memory used by the linked lists in the program. */
        freeAll();
        /* Exit the program. */
        exit(ERROR);
    }

    /* Return the pointer to the allocated memory. */
    return allocatedPointer;
}

/**
 * Tries to open a file with the given name and extension in the given mode.
 * If succeeds, returns the pointer to the opened file.
 * If fails, prints an error message and returns NULL.
 * IMPORTANT: The caller must close the returned file.
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that the given extension is not NULL and is null-terminated.
 * Assumes that the given mode is not NULL and is null-terminated.
 *
 * @param fileName The name of the file to open.
 * @param extension The extension of the file to open (should not include the
 * dot).
 * @param mode The mode in which to open the file (e.g. "r" - reading, "w" -
 * writing).
 * @return A pointer to the opened file, or NULL if the file could not be
 * opened.
 */
FILE *openFile(char fileName[], char extension[], char mode[]) {
    char *fileNameWithExtension; /* The full file name with the extension. */
    FILE *file;                  /* The pointer to the opened file. */

    /* Combine the file name with the extension. */
    fileNameWithExtension = addExtension(fileName, extension);
    /* Open the file. */
    file = fopen(fileNameWithExtension, mode);

    /* Check if there was a problem opening the file. */
    if (file == NULL) {
        /* Print an error message to notify the user. */
        printFileError(fileNameWithExtension);
    }

    /* Free the no longer needed full file name. */
    free(fileNameWithExtension);
    /* Return the pointer to the opened file. */
    return file;
}

/**
 * Skips all whitespace characters and commas in the given line.
 * Returns a pointer to the first non-whitespace character that is also not a
 * comma.
 * If such a character does not exist, returns a pointer to the null terminator
 * of the given line.
 * Does not modify the given line.
 *
 * Assumes that the given line is not NULL and is null-terminated.
 *
 * @param line The line to skip.
 * @return A pointer to the first non-whitespace character that is also not a
 * comma in the given line.
 */
char *skipWhitespace(char line[]) {
    /* Skip all whitespace characters and commas. */
    while (isspace(*line) || *line == ',') {
        line++;
    }

    /* Return the pointer to the character where the loop stopped. */
    return line;
}

/**
 * Skips all non-whitespace characters that are also not commas in the given
 * line.
 * Returns a pointer to the first whitespace character or comma (whichever comes
 * first).
 * If such a character does not exist, returns a pointer to the null terminator
 * of the given line.
 * Does not modify the given line.
 *
 * Assumes that the given line is not NULL and is null-terminated.
 *
 * @param line The line to skip.
 * @return A pointer to the first whitespace character or comma in the given
 * line.
 */
char *skipCharacters(char line[]) {
    /* Skip all non-whitespace characters that are also not commas. */
    while (*line != '\0' && !isspace(*line) && *line != ',') {
        line++;
    }

    /* Return the pointer to the character where the loop stopped. */
    return line;
}

/**
 * Returns a pointer to a newly allocated string containing the next token in
 * the given line.
 * Stops the token at the first whitespace character or comma.
 * If the first character in the given line is whitespace or a comma, returns
 * NULL.
 * Allocates a new string and copies the token into it.
 * IMPORTANT: The caller must free the returned string.
 *
 * Assumes that the given line is not NULL and is null-terminated.
 *
 * @param line The line to get the next token from.
 * @return A pointer to a newly allocated string containing the next token in
 * the given line.
 */
char *getNextToken(char line[]) {
    size_t size; /* The size of the token. */
    char *token; /* The token to extract. */

    /* Compute the size of the token. */
    size = skipCharacters(line) - line;

    /* Check for an empty token. */
    if (size == EMPTY) {
        return NULL;
    }

    /* Allocate enough memory for the token. */
    token = allocate(sizeof(char) * (size + NULL_BYTE));
    /* Copy the token from the line. */
    strncpy(token, line, size);
    /* Terminate the token with a null character. */
    token[size] = '\0';
    /* Return the desired token. */
    return token;
}

/**
 * Removes the given ending from the given string.
 * Checks the last character in the string and determines if it is the desired
 * ending.
 * If so, removes it by replacing it with a null character.
 * If not, does not modify the string at all.
 *
 * Assumes that the given string is not NULL and is null-terminated.
 *
 * @param string The string to remove the ending from.
 * @param ending The ending to remove.
 */
void removeEnding(char string[], char ending) {
    Length length; /* The length of the given string. */

    /* Compute the given string's length. */
    length = (Length)strlen(string);

    /* Check if the last character is the ending to remove. */
    if (string[length - LAST_INDEX_DIFF] == ending) {
        /* Remove the ending by replacing it with a null character. */
        string[length - LAST_INDEX_DIFF] = '\0';
    }
}

/**
 * Returns the length of the given string operand that was set in a .string
 * line.
 * Returns 0 if the string is invalid (does not contain at least 2 double-quote
 * characters).
 * The length is the number of characters inside the most distant double-quote
 * characters in the string.
 * Does not count the double-quote characters.
 *
 * Assumes that the given string is not NULL and is null-terminated.
 *
 * @param string The string to get the length of.
 * @return The length of the string.
 */
Length getStringLength(char string[]) {
    Length length; /* The current length. */

    /* Compute the given string's length. Start from the end. */
    for (length = (Length)strlen(string) - LAST_INDEX_DIFF;
         length >= SECOND_INDEX; length--) {
        /* Check for the closing double-quote character. */
        if (string[length] == '\"') {
            /* The length can be computed by not counting the double-quote. */
            return length - LAST_INDEX_DIFF;
        }
    }

    /* Since there is no closing double-quote, the string is invalid. */
    return INVALID_STRING_LENGTH;
}

/**
 * Checks and returns if the given string has a valid ending.
 * A string has a valid ending if its last non-whitespace character is a
 * double-quote.
 *
 * Assumes that the given string is not NULL and is null-terminated.
 *
 * @param string The string to check.
 * @return TRUE if the string has a valid ending, FALSE otherwise.
 */
Boolean checkStringEnding(char string[]) {
    Index index; /* The current index. */

    /* Loop over the string from the end. */
    for (index = (Index)strlen(string) - LAST_INDEX_DIFF; index >= SECOND_INDEX;
         index--) {
        /* Check for a closing double-quote character. */
        if (string[index] == '\"') {
            /* The string has a valid ending. */
            return TRUE;
        }

        /* Check for a non-whitespace character. */
        if (!isspace(string[index])) {
            /* Double-quote must end the string (there could be whitespace). */
            return FALSE;
        }
    }

    /* Since no double-quote was found, the string is invalid. */
    return FALSE;
}

/**
 * Checks and returns if the given token is a potential label.
 * A token is a potential label if its last character is a colon.
 * Does not modify the given token.
 *
 * Assumes that the given token is not NULL and is null-terminated.
 *
 * @param token The token to check.
 * @return TRUE if the token is a potential label, FALSE otherwise.
 */
Boolean checkIfLabel(char token[]) {
    /* Check the last character of the token for a colon character. */
    return token[strlen(token) - LAST_INDEX_DIFF] == ':';
}

/**
 * Checks and returns if there is a comma before any non-whitespace character in
 * the given line.
 * Does not modify the given line.
 *
 * Assumes that the given line is not NULL and is null-terminated.
 *
 * @param line The line to check.
 * @return TRUE if there is a comma before any other non-whitespace character in
 * the given line, FALSE otherwise.
 */
Boolean checkIfFollowedByComma(char line[]) {
    /* Skip all whitespace characters. */
    while (isspace(*line)) {
        line++;
    }

    /* Return if the first non-whitespace character is a comma. */
    return *line == ',';
}

/**
 * Checks and returns if there are multiple consecutive commas before any other
 * non-whitespace character in the given line.
 * Does not modify the given line.
 *
 * Assumes that the given line is not NULL and is null-terminated.
 *
 * @param line The line to check.
 * @return TRUE if there are multiple consecutive commas before any other
 * non-whitespace character in the given line, FALSE otherwise.
 */
Boolean checkForConsecutiveCommas(char line[]) {
    /* Skip all whitespace characters. */
    while (isspace(*line)) {
        line++;
    }

    /* Check for a comma and then for another one. */
    return *line == ',' && checkIfFollowedByComma(&line[SECOND_INDEX]);
}

/**
 * Adds the given extension to the given file name.
 * Returns a pointer to a newly-allocated string that is
 * "<fileName>.<extension>".
 * IMPORTANT: The caller must free the returned string.
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that the given extension is not NULL and is null-terminated.
 *
 * @param fileName The file name to add the extension to.
 * @param extension The extension to add.
 * @return A pointer to a string that is "<fileName>.<extension>".
 */
char *addExtension(char fileName[], char extension[]) {
    size_t fileNameLength;  /* The length of the given file name. */
    size_t extensionLength; /* The length of the given extension. */
    char *name;             /* The combined name. */

    /* Compute the length of each string argument. */
    fileNameLength = strlen(fileName);
    extensionLength = strlen(extension);

    /* Allocate enough memory for the combined name (with a dot). */
    name = allocate(sizeof(char) *
                    (fileNameLength + DOT_BYTE + extensionLength + NULL_BYTE));

    /* Copy the file name to the combined name. */
    strcpy(name, fileName);
    /* Add a dot. */
    name[fileNameLength] = '.';
    /* Copy the extension to the combined name. */
    strcpy(&name[fileNameLength + DOT_BYTE], extension);
    /* Add a null character. */
    name[fileNameLength + DOT_BYTE + extensionLength] = '\0';

    /* Return the combined name. */
    return name;
}

/**
 * Converts the given digit character to its corresponding number and returns
 * it.
 * If the given character is not a digit, returns 0.
 *
 * @param digit The digit character to convert.
 * @return The digit character's corresponding number, or 0 if not a digit
 * character.
 */
unsigned char convertDigitToNumber(char digit) {
    /* Check if the character is a digit. */
    if (isdigit(digit)) {
        /* Subtracting ASCII zero value converts the character to a number. */
        return (unsigned char)(digit - '0');
    }

    /* The character is not a digit. */
    return INVALID_DIGIT;
}
