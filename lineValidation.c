/*
 * lineValidation.c
 *
 * Contains functions to check for errors in each line of the .am file.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "lineValidation.h"

#include <ctype.h>  /* isdigit, isalpha, isalnum. */
#include <stdlib.h> /* free, atoi. */
#include <string.h> /* strlen, strcmp. */

#include "errorHandling.h"          /* Printing errors and warnings. */
#include "globals.h"                /* Constants and typedefs. */
#include "instructionInformation.h" /* Information about the different instructions. */
#include "utils.h"                  /* Parsing lines. */

/**
 * Checks and returns if the given line is valid.
 * Only checks for errors and warnings that can be found with no reference to the existing state of the program.
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
Boolean validateLine(char line[], char fileName[], LineNumber lineNumber) {
    Boolean isValid;
    char *token;

    if (*line == ';') {
        return TRUE;
    }

    line = skipWhitespace(line);

    if (*line == '\0') {
        return TRUE;
    }

    isValid = TRUE;

    if (*line == ';') {
        printError("There should not be any whitespace characters before the semicolon in a comment line.", fileName, lineNumber);
        isValid = FALSE;
    }

    token = getNextToken(line);

    if (checkIfLabel(token)) {
        removeEnding(token, ':');
        isValid = validateLabel(token, fileName, lineNumber);

        free(token);
        line = skipCharacters(line);
        line = skipWhitespace(line);
        token = getNextToken(line);

        if (token == NULL) {
            printError("Label with nothing after it.", fileName, lineNumber);
            return FALSE;
        }

        if (strcmp(token, ".entry") == EQUAL_STRINGS) {
            printWarning("Label defined before .entry.", fileName, lineNumber);
        } else if (strcmp(token, ".extern") == EQUAL_STRINGS) {
            printWarning("Label defined before .extern.", fileName, lineNumber);
        }
    }

    if (checkIfFollowedByComma(skipCharacters(line))) {
        printError("Comma after the first word in the command (excluding the label, if there is one).", fileName, lineNumber);
        isValid = FALSE;
    }

    if (strcmp(token, ".entry") == EQUAL_STRINGS) {
        isValid = validateEntryExtern(skipWhitespace(skipCharacters(line)), fileName, lineNumber) && isValid;
    } else if (strcmp(token, ".extern") == EQUAL_STRINGS) {
        isValid = validateEntryExtern(skipWhitespace(skipCharacters(line)), fileName, lineNumber) && isValid;
    } else if (strcmp(token, ".data") == EQUAL_STRINGS) {
        isValid = validateData(skipWhitespace(skipCharacters(line)), fileName, lineNumber) && isValid;
    } else if (strcmp(token, ".string") == EQUAL_STRINGS) {
        isValid = validateString(skipWhitespace(skipCharacters(line)), fileName, lineNumber) && isValid;
    } else {
        isValid = validateInstruction(line, fileName, lineNumber) && isValid;
    }

    free(token);
    return isValid;
}

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
Boolean validateEntryExtern(char label[], char fileName[], LineNumber lineNumber) {
    Boolean isValid;
    char *token;

    token = getNextToken(label);
    isValid = validateLabel(token, fileName, lineNumber);
    free(token);
    label = skipCharacters(label);

    if (checkIfFollowedByComma(label)) {
        printError("Comma after the label.", fileName, lineNumber);
        isValid = FALSE;
    }

    label = skipWhitespace(label);

    if (*label != '\0') {
        printError("Extra non-whitespace characters after the label.", fileName, lineNumber);
        return FALSE;
    }

    return isValid;
}

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
Boolean validateLabel(char label[], char fileName[], LineNumber lineNumber) {
    return validateName(label, fileName, lineNumber, TRUE);
}

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
Boolean validateMacro(char macro[], char fileName[], LineNumber lineNumber) {
    return validateName(macro, fileName, lineNumber, FALSE);
}

/**
 * Checks and returns if the given name is valid.
 * Prints all the errors to stdout.
 * Names are either label names or macro names, as they follow the same rules:
 * 1. Non-empty.
 * 2. Starts with a lowercase or uppercase letter in the English alphabet.
 * 3. Has a length of at most 31 characters.
 * 4. Is not a keyword in the language (includes "macr", "endmacr", operation names, register names).
 * 5. The characters not including the first character have to be either:
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
 * @return TRUE if this part contains no errors, FALSE otherwise.
 */
Boolean validateName(char name[], char fileName[], LineNumber lineNumber, Boolean isLabel) {
    Boolean isValid;

    if (*name == '\0') {
        if (isLabel) {
            printError("No label specified.", fileName, lineNumber);
        } else {
            printMacroError("No macro specified.", fileName, lineNumber);
        }

        return FALSE;
    }

    isValid = TRUE;

    if (!isalpha(*name)) {
        if (isLabel) {
            printError("Label starts with an invalid character - not a lowercase or uppercase letter in the English alphabet.", fileName, lineNumber);
        } else {
            printMacroError("Macro starts with an invalid character - not a lowercase or uppercase letter in the English alphabet.", fileName, lineNumber);
        }

        isValid = FALSE;
    }

    if (strlen(name) > MAX_NAME_LENGTH) {
        if (isLabel) {
            printError("Label is too long - maximum length is 31 characters.", fileName, lineNumber);
        } else {
            printMacroError("Macro is too long - maximum length is 31 characters.", fileName, lineNumber);
        }

        isValid = FALSE;
    }

    if (strcmp(name, "macr") == EQUAL_STRINGS) {
        if (isLabel) {
            printError("Label cannot be named \"macr\".", fileName, lineNumber);
        } else {
            printMacroError("Macro cannot be named \"macr\".", fileName, lineNumber);
        }

        isValid = FALSE;
    }

    if (strcmp(name, "endmacr") == EQUAL_STRINGS) {
        if (isLabel) {
            printError("Label cannot be named \"endmacr\".", fileName, lineNumber);
        } else {
            printMacroError("Macro cannot be named \"endmacr\".", fileName, lineNumber);
        }

        isValid = FALSE;
    }

    if (getOperationIndex(name) != INVALID_OPERATION) {
        if (isLabel) {
            printError("Label cannot share the same name as an operation.", fileName, lineNumber);
        } else {
            printMacroError("Macro cannot share the same name as an operation.", fileName, lineNumber);
        }

        isValid = FALSE;
    }

    if (name[FIRST_INDEX] == 'r' && name[SECOND_INDEX] >= '0' && name[SECOND_INDEX] <= '7' && name[THIRD_INDEX] == '\0') {
        if (isLabel) {
            printError("Label cannot share the same name as a register.", fileName, lineNumber);
        } else {
            printMacroError("Macro cannot share the same name as a register.", fileName, lineNumber);
        }

        isValid = FALSE;
    }

    while (*name != '\0') {
        if (!isalnum(*name) && *name != '_') {
            if (isLabel) {
                printError("Label contains an invalid character - not a digit, nor a lowercase or uppercase letter in the English alphabet, nor an underscore.", fileName, lineNumber);
            } else {
                printMacroError("Macro contains an invalid character - not a digit, nor a lowercase or uppercase letter in the English alphabet, nor an underscore.", fileName, lineNumber);
            }

            isValid = FALSE;
        }

        name++;
    }

    return isValid;
}

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
Boolean validateData(char data[], char fileName[], LineNumber lineNumber) {
    Boolean isValid;
    Boolean isFollowedByComma;
    char *token;

    if (*data == '\0') {
        printError("No data specified.", fileName, lineNumber);
        return FALSE;
    }

    isValid = TRUE;
    isFollowedByComma = TRUE;

    while (*data != '\0') {
        if (!isFollowedByComma) {
            printError("Missing comma between the numbers.", fileName, lineNumber);
            isValid = FALSE;
        }

        token = getNextToken(data);
        isValid = validateNumber(token, fileName, lineNumber) && isValid;
        free(token);

        data = skipCharacters(data);

        if (checkForConsecutiveCommas(data)) {
            printError("Multiple consecutive commas between the numbers.", fileName, lineNumber);
            isValid = FALSE;
        }

        isFollowedByComma = checkIfFollowedByComma(data);
        data = skipWhitespace(data);
    }

    if (isFollowedByComma) {
        printError("The last number is followed by a comma.", fileName, lineNumber);
        return FALSE;
    }

    return isValid;
}

/**
 * Checks and returns if the given number in some .data line is valid.
 * Prints all the errors to stdout.
 * The number has to be between -16384 and 16383, including both (to fit in 15 bits).
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
Boolean validateNumber(char number[], char fileName[], LineNumber lineNumber) {
    char *current;
    int value;

    current = number;

    if (*number == '+' || *number == '-') {
        current++;
    }

    while (*current != '\0') {
        if (!isdigit(*current)) {
            printError("Invalid integer.", fileName, lineNumber);
            return FALSE;
        }

        current++;
    }

    value = atoi(number);

    if (value > MAX_NUMBER) {
        printError("Number is too large to store in just 15 bits - largest possible value is 16383.", fileName, lineNumber);
        return FALSE;
    }

    if (value < MIN_NUMBER) {
        printError("Number is too small to store in just 15 bits - smallest possible value is -16384.", fileName, lineNumber);
        return FALSE;
    }

    return TRUE;
}

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
Boolean validateString(char string[], char fileName[], LineNumber lineNumber) {
    Boolean isValid;

    if (*string == '\0') {
        printError("No string specified.", fileName, lineNumber);
        return FALSE;
    }

    isValid = TRUE;

    if (*string != '\"') {
        printError("String does not start with a quotation mark.", fileName, lineNumber);
        isValid = FALSE;
    }

    if (!checkStringEnding(string)) {
        printError("String does not end with a quotation mark.", fileName, lineNumber);
        isValid = FALSE;
    }

    return isValid;
}

/**
 * Checks and returns if the given instruction line (not .data, .string, .entry, .extern) is valid.
 * Prints all the errors to stdout.
 *
 * Assumes that the given instruction string is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param instruction The instruction line to check.
 * @param fileName The name of the file in which the line is.
 * @param lineNumber The line's line number.
 * @return TRUE if this part contains no errors, FALSE otherwise.
 */
Boolean validateInstruction(char instruction[], char fileName[], LineNumber lineNumber) {
    char *operation;
    char *token;
    OperandCount operands;

    operation = getNextToken(instruction);

    if (!validateOperation(operation, fileName, lineNumber)) {
        free(operation);
        return FALSE;
    }

    operands = getOperandCount(operation);
    instruction = skipCharacters(instruction);
    instruction = skipWhitespace(instruction);

    if (operands == NO_OPERANDS && *instruction != '\0') {
        printError("Extra non-whitespace characters after operation - it should have no operands.", fileName, lineNumber);
        free(operation);
        return FALSE;
    }

    if (operands == NO_OPERANDS) {
        free(operation);
        return TRUE;
    }

    token = getNextToken(instruction);

    if (!validateOperand(token, fileName, lineNumber)) {
        free(operation);
        free(token);
        return FALSE;
    }

    if (!doesOperationAcceptOperand(operation, token, operands != ONE_OPERAND)) {
        printError("Operation does not accept the first operand - incompatible type.", fileName, lineNumber);
        return FALSE;
    }

    free(token);
    instruction = skipCharacters(instruction);

    if (operands == ONE_OPERAND && checkIfFollowedByComma(instruction)) {
        printError("Comma after the only operand.", fileName, lineNumber);
        return FALSE;
    }

    if (operands == TWO_OPERANDS && !checkIfFollowedByComma(instruction)) {
        printError("Missing comma between the first and the second operand.", fileName, lineNumber);
        return FALSE;
    }

    instruction = skipWhitespace(instruction);

    if (operands == ONE_OPERAND && *instruction != '\0') {
        printError("Extra non-whitespace characters after the operation - it should have only one operand.", fileName, lineNumber);
        free(operation);
        return FALSE;
    }

    if (operands == ONE_OPERAND) {
        free(operation);
        return TRUE;
    }

    token = getNextToken(instruction);

    if (!validateOperand(token, fileName, lineNumber)) {
        free(operation);
        free(token);
        return FALSE;
    }

    if (!doesOperationAcceptOperand(operation, token, FALSE)) {
        printError("Operation does not accept the second operand - incompatible type.", fileName, lineNumber);
        return FALSE;
    }

    free(token);
    instruction = skipCharacters(instruction);

    if (checkIfFollowedByComma(instruction)) {
        printError("Comma after the second operand.", fileName, lineNumber);
        return FALSE;
    }

    instruction = skipWhitespace(instruction);

    if (*instruction != '\0') {
        printError("Extra non-whitespace characters after the operation - it should have only two operands.", fileName, lineNumber);
        free(operation);
        return FALSE;
    }

    return TRUE;
}

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
Boolean validateOperation(char operation[], char fileName[], LineNumber lineNumber) {
    if (getOperationIndex(operation) == INVALID_OPERATION) {
        printError("Invalid operation.", fileName, lineNumber);
        return FALSE;
    }

    return TRUE;
}

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
Boolean validateOperand(char operand[], char fileName[], LineNumber lineNumber) {
    switch (getOperandType(operand)) {
        case IMMEDIATE:
            return validateImmediate(operand, fileName, lineNumber);
        case DIRECT:
            return validateLabel(operand, fileName, lineNumber);
        case INDIRECT_REGISTER:
            return validateIndirectRegister(operand, fileName, lineNumber);
        case DIRECT_REGISTER:
            return TRUE;
        default:
            return FALSE;
    }
}

/**
 * Checks and returns if the given immediate value is valid.
 * Prints all the errors to stdout.
 * The immediate value has to be between -2048 and 2047, including both (to fit in 12 bits).
 * Also, it could begin with a plus or minus sign (after the hash symbol).
 * Aside from the hash symbol and possible sign, it has to include only decimal digits.
 *
 * Assumes that the given immediate value string is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param immediate The immediate value to check.
 * @param fileName The name of the file in which the line is.
 * @param lineNumber The line's line number.
 * @return TRUE if this part contains no errors, FALSE otherwise.
 */
Boolean validateImmediate(char immediate[], char fileName[], LineNumber lineNumber) {
    int number;

    immediate++;

    if (!validateNumber(immediate, fileName, lineNumber)) {
        printError("Not a number, even though it is preceded by a hash (a label cannot start with a hash).", fileName, lineNumber);
        return FALSE;
    }

    number = atoi(immediate);

    if (number > MAX_IMMEDIATE) {
        printError("Number is too large to store in just 12 bits - largest possible value is 2047.", fileName, lineNumber);
        return FALSE;
    }

    if (number < MIN_IMMEDIATE) {
        printError("Number is too small to store in just 12 bits - smallest possible value is -2048.", fileName, lineNumber);
        return FALSE;
    }

    return TRUE;
}

/**
 * Checks and returns if the given indirect register is valid.
 * Prints all the errors to stdout.
 * The possible indirect registers are *r0, *r1, *r2, *r3, *r4, *r5, *r6, *r7.
 *
 * Assumes that the given indirect register string is not NULL and is null-terminated.
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param indirectRegister The indirect register to check.
 * @param fileName The name of the file in which the line is.
 * @param lineNumber The line's line number.
 * @return TRUE if this part contains no errors, FALSE otherwise.
 */
Boolean validateIndirectRegister(char indirectRegister[], char fileName[], LineNumber lineNumber) {
    indirectRegister++;
    if (*indirectRegister != 'r') {
        printError("Token starts with an asterisk but does not include any register right after it (a label cannot start with an asterisk).", fileName, lineNumber);
        return FALSE;
    }

    indirectRegister++;
    if (*indirectRegister == '\0') {
        printError("Token looks like it should be a register, but does not specify which one (a label cannot start with an asterisk).", fileName, lineNumber);
        return FALSE;
    }

    if (indirectRegister[FIRST_INDEX] > '7' || indirectRegister[FIRST_INDEX] < '0' || indirectRegister[SECOND_INDEX] != '\0') {
        printError("Token does not specify a valid register - a register's number should be between 0 and 7, including 0 and 7.", fileName, lineNumber);
        return FALSE;
    }

    return TRUE;
}
