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

char *skipWhitespace(char line[]) {
    /* Skip all whitespace characters and commas. */
    while (isspace(*line) || *line == ',') {
        line++;
    }

    /* Return the pointer to the character where the loop stopped. */
    return line;
}

char *skipCharacters(char line[]) {
    /* Skip all non-whitespace characters that are also not commas. */
    while (*line != '\0' && !isspace(*line) && *line != ',') {
        line++;
    }

    /* Return the pointer to the character where the loop stopped. */
    return line;
}

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

Boolean checkIfLabel(char token[]) {
    /* Check the last character of the token for a colon character. */
    return token[strlen(token) - LAST_INDEX_DIFF] == ':';
}

Boolean checkIfFollowedByComma(char line[]) {
    /* Skip all whitespace characters. */
    while (isspace(*line)) {
        line++;
    }

    /* Return if the first non-whitespace character is a comma. */
    return *line == ',';
}

Boolean checkForConsecutiveCommas(char line[]) {
    /* Skip all whitespace characters. */
    while (isspace(*line)) {
        line++;
    }

    /* Check for a comma and then for another one. */
    return *line == ',' && checkIfFollowedByComma(&line[SECOND_INDEX]);
}

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

unsigned char convertDigitToNumber(char digit) {
    /* Check if the character is a digit. */
    if (isdigit(digit)) {
        /* Subtracting ASCII zero value converts the character to a number. */
        return (unsigned char)(digit - '0');
    }

    /* The character is not a digit. */
    return INVALID_DIGIT;
}
