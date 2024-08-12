/*
 * lineValidation.h
 *
 * Contains the function prototypes for the functions in lineValidation.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef LINE_VALIDATION_H
#define LINE_VALIDATION_H

#include "globals.h" /* Typedefs. */

/**
 * Checks and returns if the given line is valid.
 * Only checks for errors and warnings that can be found with no reference to
 * the existing state of the program.
 * Prints errors and warnings to stdout.
 * NOTE: Warnings do not mean the line is invalid. They are only printed.
 *
 * Assumes that the given line is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param line The line to check.
 * @param fileName The name of the file in which the line is.
 * @param lineNumber The line's line number.
 * @return TRUE if the line contains no errors, FALSE otherwise.
 */
Boolean validateLine(char line[], char fileName[], LineNumber lineNumber);

/**
 * Splits the validation into the different types of lines.
 * The type of the line depends on the given token.
 * Returns whether or not the line is valid.
 * Prints errors to stdout.
 *
 * Assumes that the given line is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that the given token is not NULL and is null-terminated.
 *
 * @param line The line to check.
 * @param fileName The name of the file in which the line is.
 * @param lineNumber The line's line number.
 * @param token The token to check with.
 * @return TRUE if the line contains no errors, FALSE otherwise.
 */
Boolean splitLineValidation(char line[], char fileName[], LineNumber lineNumber,
                            char token[]);

/**
 * Checks and returns if the given entry/extern declaration is valid.
 * Prints all the errors to stdout.
 *
 * Assumes that the given label string is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param label The entry/extern declaration to check.
 * @param fileName The name of the file in which the line is.
 * @param lineNumber The line's line number.
 * @return TRUE if this part contains no errors, FALSE otherwise.
 */
Boolean validateEntryExtern(char label[], char fileName[],
                            LineNumber lineNumber);

/**
 * Checks and returns if the given label name is valid.
 * Prints all the errors to stdout.
 *
 * Assumes that the given label string is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param label The label to check.
 * @param fileName The name of the file in which the line is.
 * @param lineNumber The line's line number.
 * @return TRUE if this part contains no errors, FALSE otherwise.
 */
Boolean validateLabel(char label[], char fileName[], LineNumber lineNumber);

/**
 * Checks and returns if the given macro name is valid.
 * Prints all the errors to stdout.
 *
 * Assumes that the given macro string is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param macro The macro to check.
 * @param fileName The name of the file in which the line is.
 * @param lineNumber The line's line number.
 * @return TRUE if this part contains no errors, FALSE otherwise.
 */
Boolean validateMacro(char macro[], char fileName[], LineNumber lineNumber);

/**
 * Checks and returns if the given name is valid.
 * Prints all the errors to stdout.
 * Names are either label names or macro names, as they follow the same rules:
 * 1. Non-empty.
 * 2. Starts with a lowercase or uppercase letter in the English alphabet.
 * 3. Has a length of at most 31 characters.
 * 4. Is not a keyword in the language (includes "macr", "endmacr", operation
 * names, register names).
 * 5. The characters, not including the first character, have to be either:
 * - Lowercase characters in the English alphabet.
 * - Uppercase characters in the English alphabet.
 * - Digits.
 * - The underscore character.
 *
 * Assumes that the given name string is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param name The name to check.
 * @param fileName The name of the file in which the line is.
 * @param lineNumber The line's line number.
 * @param isMacro Whether the name of a macro or of a label.
 * @return TRUE if this part contains no errors, FALSE otherwise.
 */
Boolean validateName(char name[], char fileName[], LineNumber lineNumber,
                     Boolean isMacro);

/**
 * Checks and returns if all of the given name's characters are valid.
 * A character is valid if it is an underscore or a lowercase or uppercase
 * letter in the English alphabet.
 *
 * Assumes that the given name string is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param name The name to check.
 * @param fileName The name of the file in which the line is.
 * @param lineNumber The line's line number.
 * @param isMacro Whether the name of a macro or of a label.
 * @return Whether all of the given name's characters are valid.
 */
Boolean validateNameCharacters(char name[], char fileName[],
                               LineNumber lineNumber, Boolean isMacro);

/**
 * Checks and returns if the given macro definition line is valid.
 *
 * Assumes that the given line string is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param line The macro definition line to check.
 * @param fileName The name of the file in which the line is.
 * @param lineNumber The line's line number.
 * @return TRUE if this part contains no errors, FALSE otherwise.
 */
Boolean validateMacroDefinition(char line[], char fileName[],
                                LineNumber lineNumber);

/**
 * Checks and returns if the given "endmacr" line is valid.
 *
 * Assumes that the given line string is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param line The "endmacr" line to check.
 * @param fileName The name of the file in which the line is.
 * @param lineNumber The line's line number.
 * @return TRUE if this part contains no errors, FALSE otherwise.
 */
Boolean validateMacroEnd(char line[], char fileName[], LineNumber lineNumber);

/**
 * Checks and returns if the given .data line is valid.
 * Prints all the errors to stdout.
 *
 * Assumes that the given data string is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param data The .data line to check.
 * @param fileName The name of the file in which the line is.
 * @param lineNumber The line's line number.
 * @return TRUE if this part contains no errors, FALSE otherwise.
 */
Boolean validateData(char data[], char fileName[], LineNumber lineNumber);

/**
 * Checks and returns if the given number in some .data line is valid.
 * Prints all the errors to stdout.
 * The number has to be between -16384 and 16383, inclusive (to fit in 15 bits).
 * Also, the number can include a plus or minus sign in the beginning.
 * Aside from that, the number has to include only decimal digits.
 *
 * Assumes that the given number string is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param number The number to check.
 * @param fileName The name of the file in which the line is.
 * @param lineNumber The line's line number.
 * @return TRUE if this part contains no errors, FALSE otherwise.
 */
Boolean validateNumber(char number[], char fileName[], LineNumber lineNumber);

/**
 * Checks and returns if the given .string line is valid.
 * Prints all the errors to stdout.
 *
 * Assumes that the given string is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param string The .string line to check.
 * @param fileName The name of the file in which the line is.
 * @param lineNumber The line's line number.
 * @return TRUE if this part contains no errors, FALSE otherwise.
 */
Boolean validateString(char string[], char fileName[], LineNumber lineNumber);

/**
 * Checks and returns if the given instruction line (not .data, .string, .entry,
 * .extern) is valid. Prints all the errors to stdout.
 *
 * Assumes that the given instruction string is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param instruction The instruction line to check.
 * @param fileName The name of the file in which the line is.
 * @param lineNumber The line's line number.
 * @return TRUE if this part contains no errors, FALSE otherwise.
 */
Boolean validateInstruction(char instruction[], char fileName[],
                            LineNumber lineNumber);

/**
 * Checks and returns if the given operation is valid.
 * Prints all the errors to stdout.
 *
 * Assumes that the given operation string is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param operation The operation to check.
 * @param fileName The name of the file in which the line is.
 * @param lineNumber The line's line number.
 * @return TRUE if this part contains no errors, FALSE otherwise.
 */
Boolean validateOperation(char operation[], char fileName[],
                          LineNumber lineNumber);

/**
 * Checks and returns if the given operand is valid.
 * Prints all the errors to stdout.
 *
 * Assumes that the given operand string is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param operand The operand to check.
 * @param fileName The name of the file in which the line is.
 * @param lineNumber The line's line number.
 * @return TRUE if this part contains no errors, FALSE otherwise.
 */
Boolean validateOperand(char operand[], char fileName[], LineNumber lineNumber);

/**
 * Checks and returns if the given immediate value is valid.
 * Prints all the errors to stdout.
 * The immediate value has to be between -2048 and 2047, inclusive (to fit in 12
 * bits).
 * Also, it could begin with a plus or minus sign (after the hash symbol).
 * Aside from the hash symbol and the possible sign, it has to include only
 * decimal digits.
 *
 * Assumes that the given immediate value string is not NULL and is
 * null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param immediate The immediate value to check.
 * @param fileName The name of the file in which the line is.
 * @param lineNumber The line's line number.
 * @return TRUE if this part contains no errors, FALSE otherwise.
 */
Boolean validateImmediate(char immediate[], char fileName[],
                          LineNumber lineNumber);

/**
 * Checks and returns if the given indirect register is valid.
 * Prints all the errors to stdout.
 * The possible indirect registers are *r0, *r1, *r2, *r3, *r4, *r5, *r6, *r7.
 *
 * Assumes that the given indirect register string is not NULL and is
 * null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param indirectRegister The indirect register to check.
 * @param fileName The name of the file in which the line is.
 * @param lineNumber The line's line number.
 * @return TRUE if this part contains no errors, FALSE otherwise.
 */
Boolean validateIndirectRegister(char indirectRegister[], char fileName[],
                                 LineNumber lineNumber);

#endif
