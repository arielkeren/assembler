/*
 * utils.h
 *
 * Contains the function prototypes for the functions in utils.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h> /* FILE, size_t. */

#include "globals.h" /* Typedefs. */

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
void *allocate(size_t size);

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
FILE *openFile(char fileName[], char extension[], char mode[]);

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
char *skipWhitespace(char line[]);

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
char *skipCharacters(char line[]);

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
char *getNextToken(char line[]);

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
void removeEnding(char string[], char ending);

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
Length getStringLength(char string[]);

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
Boolean checkStringEnding(char string[]);

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
Boolean checkIfLabel(char token[]);

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
Boolean checkIfFollowedByComma(char line[]);

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
Boolean checkForConsecutiveCommas(char line[]);

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
char *addExtension(char fileName[], char extension[]);

/**
 * Converts the given digit character to its corresponding number and returns
 * it.
 * If the given character is not a digit, returns 0.
 *
 * @param digit The digit character to convert.
 * @return The digit character's corresponding number, or 0 if not a digit
 * character.
 */
unsigned char convertDigitToNumber(char digit);

#endif
