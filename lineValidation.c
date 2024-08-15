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

Boolean validateLine(char line[], char fileName[], LineNumber lineNumber) {
    Boolean isValid; /* Whether the line is valid. */
    char *token;     /* The current token. */

    /* Do not check comment lines. */
    if (*line == ';') {
        return TRUE;
    }

    isValid = TRUE;

    /* Check for a comma at the start of the line. */
    if (checkIfFollowedByComma(line)) {
        printError("Comma at the start of the line.", fileName, lineNumber);
        isValid = FALSE;
    }

    line = skipWhitespace(line);

    /* Check for an empty line. */
    if (*line == '\0') {
        return isValid;
    }

    /* Check for an invalid comment symbol, because it came after whitespace. */
    if (*line == ';') {
        printError(
            "There should not be any whitespace characters before the "
            "semicolon in a comment line.",
            fileName, lineNumber);
        return FALSE;
    }

    /* Get the first token. */
    token = getNextToken(line);

    /* Check if the first token is a label. */
    if (checkIfLabel(token)) {
        /* Remove the colon from the token. */
        removeEnding(token, ':');
        /* Check if the label is valid. */
        isValid = validateLabel(token, fileName, lineNumber) && isValid;

        /* The label is no longer needed. */
        free(token);
        line = skipCharacters(line);

        /* Check if the label is followed by a comma. */
        if (checkIfFollowedByComma(line)) {
            printError("Comma after the label.", fileName, lineNumber);
            isValid = FALSE;
        }

        line = skipWhitespace(line);
        /* Get the second token. */
        token = getNextToken(line);

        /* Check if the label is followed by nothing. */
        if (token == NULL) {
            printError("Label with nothing after it.", fileName, lineNumber);
            return FALSE;
        }

        /* Check if the label is followed by either .entry or .extern. */
        if (strcmp(token, ".entry") == EQUAL_STRINGS) {
            printWarning("Label defined before .entry.", fileName, lineNumber);
        } else if (strcmp(token, ".extern") == EQUAL_STRINGS) {
            printWarning("Label defined before .extern.", fileName, lineNumber);
        }
    }

    /* Check if the first/second token is followed by a comma. */
    if (checkIfFollowedByComma(skipCharacters(line))) {
        printError(
            "Comma after the first token in the line (excluding the label, if "
            "there is one).",
            fileName, lineNumber);
        isValid = FALSE;
    }

    /* Validate the line based on its type. */
    isValid = splitLineValidation(line, fileName, lineNumber, token) && isValid;
    /* The token is no longer used. */
    free(token);
    /* Return whether the line is valid. */
    return isValid;
}

Boolean splitLineValidation(char line[], char fileName[], LineNumber lineNumber,
                            char token[]) {
    /* Check if it is a .entry line. */
    if (strcmp(token, ".entry") == EQUAL_STRINGS) {
        return validateEntryExtern(skipWhitespace(skipCharacters(line)),
                                   fileName, lineNumber);
    }

    /* Check if it is a .extern line. */
    if (strcmp(token, ".extern") == EQUAL_STRINGS) {
        return validateEntryExtern(skipWhitespace(skipCharacters(line)),
                                   fileName, lineNumber);
    }

    /* Check if it is a .data line. */
    if (strcmp(token, ".data") == EQUAL_STRINGS) {
        return validateData(skipWhitespace(skipCharacters(line)), fileName,
                            lineNumber);
    }

    /* Check if it is a .string line. */
    if (strcmp(token, ".string") == EQUAL_STRINGS) {
        return validateString(skipWhitespace(skipCharacters(line)), fileName,
                              lineNumber);
    }

    /* Otherwise, it is an instruction line with an operation. */
    return validateInstruction(line, fileName, lineNumber);
}

Boolean validateEntryExtern(char label[], char fileName[],
                            LineNumber lineNumber) {
    Boolean isValid; /* Whether the line is valid. */
    char *token;     /* The label. */

    /* Get only the label. */
    token = getNextToken(label);
    /* Check if it represents a valid label. */
    isValid = validateLabel(token, fileName, lineNumber);
    /* The token is no longer needed. */
    free(token);
    label = skipCharacters(label);

    /* Check if the label is followed by a comma. */
    if (checkIfFollowedByComma(label)) {
        printError("Comma after the label.", fileName, lineNumber);
        isValid = FALSE;
    }

    label = skipWhitespace(label);

    /* The label might not be the only token, aside from .entry/.extern. */
    if (*label != '\0') {
        printError("Extra non-whitespace characters after the label.", fileName,
                   lineNumber);
        return FALSE;
    }

    /* Return whether the line is valid. */
    return isValid;
}

Boolean validateLabel(char label[], char fileName[], LineNumber lineNumber) {
    /* Macros and labels follow the same naming rules. */
    return validateName(label, fileName, lineNumber, FALSE);
}

Boolean validateMacro(char macro[], char fileName[], LineNumber lineNumber) {
    /* Macros and labels follow the same naming rules. */
    return validateName(macro, fileName, lineNumber, TRUE);
}

Boolean validateName(char name[], char fileName[], LineNumber lineNumber,
                     Boolean isMacro) {
    Boolean isValid; /* Whether the name is valid. */

    /* Check if the name is empty. */
    if (*name == '\0') {
        printNameError("not specified.", fileName, lineNumber, isMacro);
        return FALSE;
    }

    isValid = TRUE;

    /* Check if the first character is not a letter. */
    if (!isalpha(*name)) {
        printNameError(
            "starts with an invalid character - not a lowercase or "
            "uppercase letter in the English alphabet.",
            fileName, lineNumber, isMacro);
        isValid = FALSE;
    }

    /* Check if the name's length exceeds the maximum of 31 characters. */
    if (strlen(name) > MAX_NAME_LENGTH) {
        printNameError("is too long - maximum length is 31 characters.",
                       fileName, lineNumber, isMacro);
        isValid = FALSE;
    }

    /* Check if the name is macr, a reserved keyword. */
    if (strcmp(name, "macr") == EQUAL_STRINGS) {
        printNameError("cannot be named \"macr\".", fileName, lineNumber,
                       isMacro);
        isValid = FALSE;
    }

    /* Check if the name is endmacr, a reserved keyword. */
    if (strcmp(name, "endmacr") == EQUAL_STRINGS) {
        printNameError("cannot be named \"endmacr\".", fileName, lineNumber,
                       isMacro);
        isValid = FALSE;
    }

    /* Check if the name is some reserved keyword for an operation. */
    if (getOperationIndex(name) != INVALID_OPERATION) {
        printNameError("cannot share the same name as an operation.", fileName,
                       lineNumber, isMacro);
        isValid = FALSE;
    }

    /* Check if the name is some reserved keyword for a register. */
    if (name[FIRST_INDEX] == 'r' && name[SECOND_INDEX] >= '0' &&
        name[SECOND_INDEX] <= '7' && name[THIRD_INDEX] == '\0') {
        printNameError("cannot share the same name as a register.", fileName,
                       lineNumber, isMacro);
        isValid = FALSE;
    }

    /* All of the name's characters must be letters, digits or underscores. */
    return validateNameCharacters(&name[SECOND_INDEX], fileName, lineNumber,
                                  isMacro) &&
           isValid;
}

Boolean validateNameCharacters(char name[], char fileName[],
                               LineNumber lineNumber, Boolean isMacro) {
    Boolean isValid; /* Whether the name's characters are valid. */

    isValid = TRUE;

    /* Check each character (aside from the first one, which was not given). */
    while (*name != '\0') {
        /* Every character must be a letter, a digit or an underscore. */
        if (!isalnum(*name) && *name != '_') {
            printNameError(
                "contains an invalid character - not a digit, nor a lowercase "
                "or uppercase letter in the English alphabet, nor an "
                "underscore.",
                fileName, lineNumber, isMacro);
            isValid = FALSE;
        }

        /* Move on to the next character. */
        name++;
    }

    /* Return whether all of the name's characters are valid. */
    return isValid;
}

Boolean validateMacroDefinition(char line[], char fileName[],
                                LineNumber lineNumber) {
    Boolean isValid; /* Whether the line is valid. */

    isValid = TRUE;

    /* Check if macr is preceded by a comma. */
    if (checkIfFollowedByComma(line)) {
        printMacroError("Comma before the \"macr\" keyword.", fileName,
                        lineNumber);
        isValid = FALSE;
    }

    line = skipWhitespace(line);
    line = skipCharacters(line);

    /* Check if macr is followed by a comma. */
    if (checkIfFollowedByComma(line)) {
        printMacroError("Comma after the \"macr\" keyword.", fileName,
                        lineNumber);
        isValid = FALSE;
    }

    line = skipWhitespace(line);

    /* Check if the macro's name is empty. */
    if (*line == '\0') {
        printMacroError("No macro name specified.", fileName, lineNumber);
        return FALSE;
    }

    line = skipCharacters(line);

    /* Check if the macro's name is followed by a comma. */
    if (checkIfFollowedByComma(line)) {
        printMacroError("Comma after the macro name.", fileName, lineNumber);
        isValid = FALSE;
    }

    line = skipWhitespace(line);

    /* Check if there are any extra tokens after the name. */
    if (*line != '\0') {
        printMacroError("Extra non-whitespace characters after the macro name.",
                        fileName, lineNumber);
        isValid = FALSE;
    }

    /* Return whether the line is valid. */
    return isValid;
}

Boolean validateMacroEnd(char line[], char fileName[], LineNumber lineNumber) {
    Boolean isValid; /* Whether the line is valid. */

    isValid = TRUE;

    /* Check if endmacr is preceded by a comma. */
    if (checkIfFollowedByComma(line)) {
        printMacroError("Comma before the \"endmacr\" keyword.", fileName,
                        lineNumber);
        isValid = FALSE;
    }

    line = skipWhitespace(line);
    line = skipCharacters(line);

    /* Check if endmacr is followed by a comma. */
    if (checkIfFollowedByComma(line)) {
        printMacroError("Comma after the \"endmacr\" keyword.", fileName,
                        lineNumber);
        isValid = FALSE;
    }

    line = skipWhitespace(line);

    /* Check if there are any extra tokens after endmacr. */
    if (*line != '\0') {
        printMacroError(
            "Extra non-whitespace characters after the \"endmacr\" keyword.",
            fileName, lineNumber);
        isValid = FALSE;
    }

    /* Return whether the line is valid. */
    return isValid;
}

Boolean validateData(char data[], char fileName[], LineNumber lineNumber) {
    Boolean isValid;           /* Whether the line is valid.*/
    Boolean isFollowedByComma; /* Whether there was a comma. */
    char *token;               /* The current token. */

    /* Check if nothing is given in the .data line. */
    if (*data == '\0') {
        printError("No data specified.", fileName, lineNumber);
        return FALSE;
    }

    isValid = TRUE;
    isFollowedByComma = TRUE;

    /* Loop over the tokens in the .data line. */
    while (*data != '\0') {
        /* Check if a comma is missing between the numbers. */
        if (!isFollowedByComma) {
            printError("Missing comma between the numbers.", fileName,
                       lineNumber);
            isValid = FALSE;
        }

        /* Get the current token. */
        token = getNextToken(data);
        /* Check if the token represents a valid number. */
        isValid = validateNumber(token, fileName, lineNumber, FALSE) && isValid;
        /* The token is no longer needed. */
        free(token);

        /* Skip the token. */
        data = skipCharacters(data);

        /* Check for multiple consecutive commas before the next token. */
        if (checkForConsecutiveCommas(data)) {
            printError("Multiple consecutive commas between the numbers.",
                       fileName, lineNumber);
            isValid = FALSE;
        }

        /* Check if there is a comma before the next token. */
        isFollowedByComma = checkIfFollowedByComma(data);
        /* Skip to the next token. */
        data = skipWhitespace(data);
    }

    /* Check if the last token is followed by a comma. */
    if (isFollowedByComma) {
        printError("The last number is followed by a comma.", fileName,
                   lineNumber);
        return FALSE;
    }

    /* Return whether the .data line is valid. */
    return isValid;
}

Boolean validateNumber(char number[], char fileName[], LineNumber lineNumber,
                       Boolean isImmediate) {
    char *current; /* The current character in the number.*/
    int value;     /* The numerical value of the number.*/

    /* Start from the first character in the number. */
    current = number;

    /* Skip the possible sign. */
    if (*number == '+' || *number == '-') {
        current++;
    }

    /* Loop over every character in the number. */
    while (*current != '\0') {
        /* Check if the character is not a digit. */
        if (!isdigit(*current)) {
            printError("Invalid integer.", fileName, lineNumber);
            return FALSE;
        }

        /* Move on to the next character. */
        current++;
    }

    /* Convert the number string to an integer. */
    value = atoi(number);

    if (isImmediate) {
        /* Check if the immediate value is above the maximum allowed. */
        if (value > MAX_IMMEDIATE) {
            printError(
                "Number is too large to store in just 12 bits - largest "
                "possible value is 2047.",
                fileName, lineNumber);
            return FALSE;
        }

        /* Check if the immediate value is below the minimum allowed. */
        if (value < MIN_IMMEDIATE) {
            printError(
                "Number is too small to store in just 12 bits - smallest "
                "possible value is -2048.",
                fileName, lineNumber);
            return FALSE;
        }

        return TRUE;
    }

    /* Check if the value is above the maximum allowed. */
    if (value > MAX_NUMBER) {
        printError(
            "Number is too large to store in just 15 bits - largest possible "
            "value is 16383.",
            fileName, lineNumber);
        return FALSE;
    }

    /* Check if the value is below the minimum allowed. */
    if (value < MIN_NUMBER) {
        printError(
            "Number is too small to store in just 15 bits - smallest possible "
            "value is -16384.",
            fileName, lineNumber);
        return FALSE;
    }

    /* If the number passed every test, it is valid. */
    return TRUE;
}

Boolean validateString(char string[], char fileName[], LineNumber lineNumber) {
    Boolean isValid; /* Whether the string is valid. */

    /* Check if the string is empty. */
    if (*string == '\0') {
        printError("No string specified.", fileName, lineNumber);
        return FALSE;
    }

    isValid = TRUE;

    /* Check if the string does not start with a quotation mark. */
    if (*string != '\"') {
        printError("String does not start with a quotation mark.", fileName,
                   lineNumber);
        isValid = FALSE;
    }

    /* Check if the string does not end with a quotation mark. */
    if (!checkStringEnding(string)) {
        printError("String does not end with a quotation mark.", fileName,
                   lineNumber);
        isValid = FALSE;
    }

    /* Otherwise, the .string line's string is valid. */
    return isValid;
}

Boolean validateInstruction(char instruction[], char fileName[],
                            LineNumber lineNumber) {
    char *operation;       /* The operation of the instruction line. */
    char *token;           /* The operand of the instruction line. */
    OperandCount operands; /* The number of expected operands. */

    /* Get the operation. */
    operation = getNextToken(instruction);

    /* Check if the operation is invalid. */
    if (!validateOperation(operation, fileName, lineNumber)) {
        free(operation);
        return FALSE;
    }

    /* Get the number of expected operands. */
    operands = getOperandCount(operation);
    instruction = skipCharacters(instruction);
    instruction = skipWhitespace(instruction);

    /* Check if the operation expects no operands. */
    if (operands == NO_OPERANDS) {
        /* Check if any operands were given. */
        if (*instruction != '\0') {
            printError("Operation expects no operands.", fileName, lineNumber);
            free(operation);
            return FALSE;
        }

        /* End here, as there are no more tokens. */
        free(operation);
        return TRUE;
    }

    /* Get the first operand. */
    token = getNextToken(instruction);

    /* Check if the operand is invalid. */
    if (!validateOperand(token, fileName, lineNumber)) {
        free(operation);
        free(token);
        return FALSE;
    }

    /* Check if the operation does not accept the first operand. */
    if (!doesOperationAcceptOperand(operation, token,
                                    operands != ONE_OPERAND)) {
        printError(
            "Operation does not accept the first operand - incompatible type.",
            fileName, lineNumber);
        free(operation);
        free(token);
        return FALSE;
    }

    /* The first operand is no longer needed. */
    free(token);
    instruction = skipCharacters(instruction);

    /* Check if there is only one operand, that is followed by a comma. */
    if (operands == ONE_OPERAND && checkIfFollowedByComma(instruction)) {
        printError("Comma after the only operand.", fileName, lineNumber);
        free(operation);
        return FALSE;
    }

    /* Check if there are two operands, that are not separated by a comma. */
    if (operands == TWO_OPERANDS && !checkIfFollowedByComma(instruction)) {
        printError("Missing comma between the operands.", fileName, lineNumber);
        free(operation);
        return FALSE;
    }

    /* Check if the operands are separated by multiple commas. */
    if (operands == TWO_OPERANDS && checkForConsecutiveCommas(instruction)) {
        printError("Multiple consecutive commas between the operands.",
                   fileName, lineNumber);
        free(operation);
        return FALSE;
    }

    instruction = skipWhitespace(instruction);

    /* Check if there are more tokens when only one operand is expected. */
    if (operands == ONE_OPERAND && *instruction != '\0') {
        printError(
            "Extra non-whitespace characters after the operation - it should "
            "have only one operand.",
            fileName, lineNumber);
        free(operation);
        return FALSE;
    }

    /* Check if only one operand is expected. */
    if (operands == ONE_OPERAND) {
        /* End here, as there are no more tokens. */
        free(operation);
        return TRUE;
    }

    /* Get the second operand. */
    token = getNextToken(instruction);

    /* Check if the second operand is invalid. */
    if (!validateOperand(token, fileName, lineNumber)) {
        free(operation);
        free(token);
        return FALSE;
    }

    /* Check if the operation does not accept the second operand. */
    if (!doesOperationAcceptOperand(operation, token, FALSE)) {
        printError(
            "Operation does not accept the second operand - incompatible "
            "type.",
            fileName, lineNumber);
        free(operation);
        free(token);
        return FALSE;
    }

    /* The operation and the second operand are no longer needed. */
    free(operation);
    free(token);
    instruction = skipCharacters(instruction);

    /* Check if there is a comma after the second operand. */
    if (checkIfFollowedByComma(instruction)) {
        printError("Comma after the second operand.", fileName, lineNumber);
        return FALSE;
    }

    instruction = skipWhitespace(instruction);

    /* Check if there are more tokens when only two operands are expected. */
    if (*instruction != '\0') {
        printError(
            "Extra non-whitespace characters after the operation - it should "
            "have only two operands.",
            fileName, lineNumber);
        return FALSE;
    }

    /* End here, as there are no more tokens to check. */
    return TRUE;
}

Boolean validateOperation(char operation[], char fileName[],
                          LineNumber lineNumber) {
    /* Check if the operation could not be found (invalid). */
    if (getOperationIndex(operation) == INVALID_OPERATION) {
        printError("Invalid operation.", fileName, lineNumber);
        return FALSE;
    }

    return TRUE;
}

Boolean validateOperand(char operand[], char fileName[],
                        LineNumber lineNumber) {
    /* Split the validation based on the operand's addressing mode. */
    switch (getOperandType(operand)) {
        case IMMEDIATE:
            return validateNumber(&operand[SECOND_INDEX], fileName, lineNumber,
                                  TRUE);
        case DIRECT:
            return validateLabel(operand, fileName, lineNumber);
        case INDIRECT_REGISTER:
            return validateIndirectRegister(operand, fileName, lineNumber);
        case DIRECT_REGISTER:
            /* If it ends up here, the operand is already a valid register. */
            return TRUE;
        default:
            /* This should never happen. */
            return FALSE;
    }
}

Boolean validateIndirectRegister(char indirectRegister[], char fileName[],
                                 LineNumber lineNumber) {
    /* Skip the asterisk.*/
    indirectRegister++;
    /* Check if there is no register specified. */
    if (*indirectRegister != 'r') {
        printError(
            "Token starts with an asterisk but does not specify any register "
            "right after it (a label cannot start with an asterisk).",
            fileName, lineNumber);
        return FALSE;
    }

    /* Skip the r. */
    indirectRegister++;
    /* Check if there is only an r. */
    if (*indirectRegister == '\0') {
        printError(
            "Token looks like it should be a register, but does not specify "
            "which one (a label cannot start with an asterisk).",
            fileName, lineNumber);
        return FALSE;
    }

    /* Check if the register specified is not between 0 and 7, inclusive. */
    if (indirectRegister[FIRST_INDEX] > '7' ||
        indirectRegister[FIRST_INDEX] < '0' ||
        indirectRegister[SECOND_INDEX] != '\0') {
        printError(
            "Token does not specify a valid register - a register's number "
            "should be between 0 and 7, including 0 and 7.",
            fileName, lineNumber);
        return FALSE;
    }

    /* Otherwise, the indirect register is valid. */
    return TRUE;
}
