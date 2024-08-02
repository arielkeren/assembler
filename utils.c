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
 * If fails, prints an error message, frees all memory used by the linked lists and exits.
 * NOTE: Does not initialize the allocated memory.
 * IMPORTANT: The caller must free the returned memory.
 *
 * @param size The size of the memory to allocate in bytes.
 * @return A pointer to the allocated memory if successful.
 */
void *allocate(size_t size) {
    void *allocatedPointer;

    allocatedPointer = malloc(size);

    if (allocatedPointer == NULL) {
        printAllocationError();
        freeAll();
        exit(ERROR);
    }

    return allocatedPointer;
}

/**
 * Tries to open a file with the given name and extension in the given mode.
 * If succeeds, returns the pointer to the opened file.
 * If fails, prints an error message and returns NULL.
 * IMPORTANT: The caller must close the returned file.
 *
 * @param fileName The name of the file to open.
 * @param extension The extension of the file to open (should not include the dot).
 * @param mode The mode in which to open the file (e.g. "r" - reading, "w" - writing).
 * @return A pointer to the opened file, or NULL if the file could not be opened.
 */
FILE *openFile(char fileName[], char extension[], char mode[]) {
    char *fileNameWithExtension;
    FILE *file;

    fileNameWithExtension = addExtension(fileName, extension);
    file = fopen(fileNameWithExtension, mode);

    if (file == NULL) {
        printFileError(fileNameWithExtension);
    }

    free(fileNameWithExtension);
    return file;
}

/**
 * Skips all whitespace characters and commas in the given line.
 * Returns a pointer to the first non-whitespace character that is also not a comma.
 * If such a character does not exist, returns a pointer to the null terminator of the given line.
 * Does not modify the given line.
 *
 * @param line The line to skip.
 * @return A pointer to the first non-whitespace character that is also not a comma in the given line.
 */
char *skipWhitespace(char line[]) {
    while (isspace(*line) || *line == ',') {
        line++;
    }

    return line;
}

/**
 * Skips all non-whitespace characters that are also not commas in the given line.
 * Returns a pointer to the first whitespace character or comma (whichever comes first).
 * If such a character does not exist, returns a pointer to the null terminator of the given line.
 * Does not modify the given line.
 *
 * @param line The line to skip.
 * @return A pointer to the first whitespace character or comma in the given line.
 */
char *skipCharacters(char line[]) {
    while (*line != '\0' && !isspace(*line) && *line != ',') {
        line++;
    }

    return line;
}

/**
 * Returns a pointer to a newly allocated string containing the next token in the given line.
 * Stops the token at the first whitespace character or comma.
 * If the first character in the given line is whitespace or a comma, returns NULL.
 * Allocates a new string and copies the token into it.
 * IMPORTANT: The caller must free the returned string.
 *
 * @param line The line to get the next token from.
 * @return A pointer to a newly allocated string containing the next token in the given line.
 */
char *getNextToken(char line[]) {
    size_t size;
    char *token;

    size = skipCharacters(line) - line;

    if (size == EMPTY) {
        return NULL;
    }

    token = allocate(sizeof(char) * (size + NULL_BYTE));
    strncpy(token, line, size);
    token[size] = '\0';
    return token;
}

/**
 * Removes the given ending from the given string.
 * Checks the last character in the string and determines if it is the desired ending.
 * If so, removes it by replacing it with a null character.
 * If not, does not modify the string at all.
 *
 * @param string The string to remove the ending from.
 * @param ending The ending to remove.
 */
void removeEnding(char string[], char ending) {
    Length length;

    length = (Length)strlen(string);

    if (string[length - LAST_INDEX_DIFF] == ending) {
        string[length - LAST_INDEX_DIFF] = '\0';
    }
}

/**
 * Returns the length of the given string operand that was set in a .string line.
 * Returns 0 if the string is invalid (does not contain at least 2 double-quote characters).
 * The length is the number of characters inside the most distant double-quote characters in the string.
 * Does not count the double-quote characters.
 *
 * @param string The string to get the length of.
 * @return The length of the string.
 */
Length getStringLength(char string[]) {
    Length length;

    for (length = (Length)strlen(string) - LAST_INDEX_DIFF; length >= SECOND_INDEX; length--) {
        if (string[length] == '\"') {
            return length - LAST_INDEX_DIFF;
        }
    }

    return INVALID_STRING_LENGTH;
}

/**
 * Checks and returns if the given string has a valid ending.
 * A string has a valid ending if its last non-whitespace character is a double-quote.
 *
 * @param string The string to check.
 * @return TRUE if the string has a valid ending, FALSE otherwise.
 */
Boolean checkStringEnding(char string[]) {
    Index index;

    for (index = (Index)strlen(string) - LAST_INDEX_DIFF; index >= SECOND_INDEX; index--) {
        if (string[index] == '\"') {
            return TRUE;
        }

        if (!isspace(string[index])) {
            return FALSE;
        }
    }

    return FALSE;
}

/**
 * Checks and returns if the given token is a potential label.
 * A token is a potential label if its last character is a colon.
 * Does not modify the given token.
 *
 * @param token The token to check.
 * @return TRUE if the token is a potential label, FALSE otherwise.
 */
Boolean checkIfLabel(char token[]) {
    return token[strlen(token) - LAST_INDEX_DIFF] == ':';
}

/**
 * Checks and returns if there is a comma before any non-whitespace character in the given line.
 * Does not modify the given line.
 *
 * @param line The line to check.
 * @return TRUE if there is a comma before any other non-whitespace character in the given line, FALSE otherwise.
 */
Boolean checkIfFollowedByComma(char line[]) {
    while (isspace(*line)) {
        line++;
    }

    return *line == ',';
}

/**
 * Checks and returns if there are multiple consecutive commas before any other non-whitespace character in the given line.
 * Does not modify the given line.
 *
 * @param line The line to check.
 * @return TRUE if there are multiple consecutive commas before any other non-whitespace character in the given line, FALSE otherwise.
 */
Boolean checkForConsecutiveCommas(char line[]) {
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

/**
 * Adds the given extension to the given file name.
 * Returns a pointer to a newly-allocated string that is "<fileName>.<extension>".
 * IMPORTANT: The caller must free the returned string.
 *
 * @param fileName The file name to add the extension to.
 * @param extension The extension to add.
 * @return A pointer to a string that is "<fileName>.<extension>".
 */
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

/**
 * Converts the given digit character to its corresponding number and returns it.
 * If the given character is not a digit, returns 0.
 *
 * @param digit The digit character to convert.
 * @return The digit character's corresponding number, or 0 if not a digit character.
 */
unsigned char convertDigitToNumber(char digit) {
    if (!isdigit(digit)) {
        return INVALID_DIGIT;
    }

    return (unsigned char)(digit - '0');
}
