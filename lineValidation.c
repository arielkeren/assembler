#include "lineValidation.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "instructionInformation.h"
#include "utils.h"

boolean validateLine(char line[], unsigned lineNumber) {
    boolean isValid;
    char *token;

    isValid = TRUE;
    line = skipWhitespace(line);

    if (*line == '\0' || *line == ';') {
        return TRUE;
    }

    token = getNextToken(line);

    if (checkIfLabel(token)) {
        removeEnding(token, ':');
        isValid = validateLabel(token, lineNumber);

        free(token);
        line = skipCharacters(line);
        line = skipWhitespace(line);
        token = getNextToken(line);

        if (token == NULL) {
            printError("Label with nothing after it.", lineNumber);
            return FALSE;
        }

        if (strcmp(token, ".entry") == 0) {
            printWarning("Label defined before .entry.", lineNumber);
        } else if (strcmp(token, ".extern") == 0) {
            printWarning("Label defined before .extern.", lineNumber);
        }
    }

    if (checkIfFollowedByComma(skipCharacters(line))) {
        printError("Comma after the first word in the command (excluding the label, if there is one).", lineNumber);
        isValid = FALSE;
    }

    if (strcmp(token, ".entry") == 0) {
        isValid = isValid && validateEntryExtern(skipWhitespace(skipCharacters(line)), lineNumber);
    } else if (strcmp(token, ".extern") == 0) {
        isValid = isValid && validateEntryExtern(skipWhitespace(skipCharacters(line)), lineNumber);
    } else if (strcmp(token, ".data") == 0) {
        isValid = isValid && validateData(skipWhitespace(skipCharacters(line)), lineNumber);
    } else if (strcmp(token, ".string") == 0) {
        isValid = isValid && validateString(skipWhitespace(skipCharacters(line)), lineNumber);
    } else {
        isValid = isValid && validateInstruction(line, lineNumber);
    }

    free(token);
    return isValid;
}

boolean validateEntryExtern(char label[], unsigned lineNumber) {
    boolean isValid;
    char *token;

    token = getNextToken(label);
    isValid = validateLabel(token, lineNumber);
    free(token);
    label = skipCharacters(label);

    if (checkIfFollowedByComma(label)) {
        printError("Comma after the label.", lineNumber);
        isValid = FALSE;
    }

    label = skipWhitespace(label);

    if (*label != '\0') {
        printError("Extra non-whitespace characters after the label.", lineNumber);
        return FALSE;
    }

    return isValid;
}

boolean validateLabel(char label[], unsigned lineNumber) {
    boolean isValid;

    if (*label == '\0') {
        printError("ERROR: No label specified.", lineNumber);
        return FALSE;
    }

    isValid = TRUE;

    if (!isalpha(*label)) {
        printError("Label starts with an invalid character - not a lowercase or uppercase letter in the English alphabet.", lineNumber);
        isValid = FALSE;
    }

    if (strlen(label) > 31) {
        printError("Label is too long - maximum length is 31 characters.", lineNumber);
        isValid = FALSE;
    }

    if (getOperationIndex(label) != 16) {
        printError("Label cannot share the same name as an operation.", lineNumber);
        isValid = FALSE;
    }

    if (label[0] == 'r' && label[1] >= '0' && label[1] <= '7' && label[2] == '\0') {
        printError("Label cannot share the same name as a register.", lineNumber);
        isValid = FALSE;
    }

    while (*label != '\0') {
        if (!isalnum(*label) && *label != '_') {
            printError("Label contains an invalid character - not a digit, nor a lowercase or uppercase letter in the English alphabet, nor an underscore.", lineNumber);
            isValid = FALSE;
        }

        label++;
    }

    return isValid;
}

boolean validateData(char data[], unsigned lineNumber) {
    boolean isValid;
    boolean isFollowedByComma;
    char *token;

    if (*data == '\0') {
        printError("No data specified.", lineNumber);
        return FALSE;
    }

    isValid = TRUE;
    isFollowedByComma = TRUE;

    while (*data != '\0') {
        if (!isFollowedByComma) {
            printError("Missing comma between the numbers.", lineNumber);
            isValid = FALSE;
        }

        token = getNextToken(data);
        isValid = isValid && validateNumber(token, lineNumber);
        free(token);

        data = skipCharacters(data);

        if (checkForConsecutiveCommas(data)) {
            printError("Multiple consecutive commas between the numbers.", lineNumber);
            isValid = FALSE;
        }

        isFollowedByComma = checkIfFollowedByComma(data);
        data = skipWhitespace(data);
    }

    if (isFollowedByComma) {
        printError("The last number is followed by a comma.", lineNumber);
        return FALSE;
    }

    return isValid;
}

boolean validateNumber(char number[], unsigned lineNumber) {
    if (*number == '+' || *number == '-') {
        number++;
    }

    while (*number != '\0') {
        if (!isdigit(*number)) {
            printError("Invalid integer.", lineNumber);
            return FALSE;
        }

        number++;
    }

    return TRUE;
}

boolean validateString(char string[], unsigned lineNumber) {
    boolean isValid;
    char *token;

    if (*string == '\0') {
        printError("No string specified.", lineNumber);
        return FALSE;
    }

    isValid = TRUE;
    token = getNextToken(string);

    if (*token != '\"') {
        printError("String does not start with a quotation mark.", lineNumber);
        isValid = FALSE;
    }

    if (token[strlen(token) - 1] != '\"') {
        printError("String does not end with a quotation mark.", lineNumber);
        isValid = FALSE;
    }

    string = skipCharacters(string);

    if (checkIfFollowedByComma(string)) {
        printError("Comma after the string.", lineNumber);
        isValid = FALSE;
    }

    string = skipWhitespace(string);

    if (*string != '\0') {
        printError("Extra non-whitespace characters after the string.", lineNumber);
        isValid = FALSE;
    }

    free(token);
    return isValid;
}

boolean validateInstruction(char instruction[], unsigned lineNumber) {
    char *operation;
    char *token;
    unsigned char operandCount;

    operation = getNextToken(instruction);

    if (!validateOperation(operation, lineNumber)) {
        free(operation);
        return FALSE;
    }

    operandCount = getOperandCount(operation);
    instruction = skipCharacters(instruction);
    instruction = skipWhitespace(instruction);

    if (operandCount == 0 && *instruction != '\0') {
        printError("Extra non-whitespace characters after operation - it should have no operands.", lineNumber);
        free(operation);
        return FALSE;
    }

    if (operandCount == 0) {
        free(operation);
        return TRUE;
    }

    token = getNextToken(instruction);

    if (!validateOperand(token, lineNumber)) {
        free(operation);
        free(token);
        return FALSE;
    }

    if (!doesOperationAcceptOperand(operation, token, operandCount != 1)) {
        printError("Operation does not accept the first operand - incompatible type.", lineNumber);
        return FALSE;
    }

    free(token);
    instruction = skipCharacters(instruction);

    if (operandCount == 1 && checkIfFollowedByComma(instruction)) {
        printError("Comma after the only operand.", lineNumber);
        return FALSE;
    }

    if (operandCount == 2 && !checkIfFollowedByComma(instruction)) {
        printError("Missing comma between the first and the second operand.", lineNumber);
        return FALSE;
    }

    instruction = skipWhitespace(instruction);

    if (operandCount == 1 && *instruction != '\0') {
        printError("Extra non-whitespace characters after the operation - it should have only one operand.", lineNumber);
        free(operation);
        return FALSE;
    }

    if (operandCount == 1) {
        free(operation);
        return TRUE;
    }

    token = getNextToken(instruction);

    if (!validateOperand(token, lineNumber)) {
        free(operation);
        free(token);
        return FALSE;
    }

    if (!doesOperationAcceptOperand(operation, token, FALSE)) {
        printError("Operation does not accept the second operand - incompatible type.", lineNumber);
        return FALSE;
    }

    free(token);
    instruction = skipCharacters(instruction);

    if (checkIfFollowedByComma(instruction)) {
        printError("Comma after the second operand.", lineNumber);
        return FALSE;
    }

    instruction = skipWhitespace(instruction);

    if (*instruction != '\0') {
        printError("Extra non-whitespace characters after the operation - it should have only two operands.", lineNumber);
        free(operation);
        return FALSE;
    }

    return TRUE;
}

boolean validateOperation(char operation[], unsigned lineNumber) {
    if (getOperationIndex(operation) == 16) {
        printError("Invalid operation.", lineNumber);
        return FALSE;
    }

    return TRUE;
}

boolean validateOperand(char operand[], unsigned lineNumber) {
    switch (getOperandType(operand)) {
        case IMMEDIATE:
            return validateImmediate(operand, lineNumber);
        case DIRECT:
            return validateLabel(operand, lineNumber);
        case INDIRECT_REGISTER:
            return validateIndirectRegister(operand, lineNumber);
        case DIRECT_REGISTER:
            return TRUE;
        default:
            return FALSE;
    }
}

boolean validateImmediate(char immediate[], unsigned lineNumber) {
    int number;

    immediate++;

    if (!validateNumber(immediate, lineNumber)) {
        printError("Not a number, even though it is preceded by a hash (a label cannot start with a hash).", lineNumber);
        return FALSE;
    }

    number = atoi(immediate);

    if (number > 2047) {
        printError("Number is too large to store in just 12 bits - largest possible value is 2047.", lineNumber);
        return FALSE;
    }

    if (number < -2048) {
        printError("Number is too small to store in just 12 bits - smallest possible value is -2048.", lineNumber);
        return FALSE;
    }

    return TRUE;
}

boolean validateIndirectRegister(char directRegister[], unsigned lineNumber) {
    directRegister++;
    if (*directRegister != 'r') {
        printError("Token starts with an asterisk but does not include any register right after it (a label cannot start with an asterisk).", lineNumber);
        return FALSE;
    }

    directRegister++;
    if (*directRegister == '\0') {
        printError("Token looks like it should be a register, but does not specify which one (a label cannot start with an asterisk).", lineNumber);
        return FALSE;
    }

    if (*directRegister > '7' || *directRegister < '0' || *(directRegister + 1) != '\0') {
        printError("Token does not specify a valid register - a register's number should be between 0 and 7, including 0 and 7.", lineNumber);
        return FALSE;
    }

    return TRUE;
}
