#include "lineValidation.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "instructionInformation.h"
#include "utils.h"

boolean validateLine(char line[]) {
    boolean isValid;
    char *token;

    isValid = TRUE;
    line = skipWhitespace(line);

    if (line[0] == '\0' || line[0] == ';') {
        return TRUE;
    }

    token = getNextToken(line);

    if (checkIfLabel(token)) {
        removeEnding(token, ':');
        isValid = validateLabel(token);

        free(token);
        line = skipCharacters(line);
        line = skipWhitespace(line);
        token = getNextToken(line);

        if (token == NULL) {
            printf("ERROR: Label \"%s\" with nothing after it.", token);
            return FALSE;
        }

        if (strcmp(token, ".entry") == 0) {
            printf("WARNING: Label \"%s\" before .entry.\n", token);
        } else if (strcmp(token, ".extern") == 0) {
            printf("WARNING: Label \"%s\" before .extern.\n", token);
        }
    }

    if (checkIfFollowedByComma(skipCharacters(line))) {
        printf("ERROR: Comma after the first word in the command (excluding the label, if there is one).\n");
        isValid = FALSE;
    }

    if (strcmp(token, ".entry") == 0) {
        isValid = isValid && validateEntryExtern(skipWhitespace(skipCharacters(line)));
    } else if (strcmp(token, ".extern") == 0) {
        isValid = isValid && validateEntryExtern(skipWhitespace(skipCharacters(line)));
    } else if (strcmp(token, ".data") == 0) {
        isValid = isValid && validateData(skipWhitespace(skipCharacters(line)));
    } else if (strcmp(token, ".string") == 0) {
        isValid = isValid && validateString(skipWhitespace(skipCharacters(line)));
    } else {
        isValid = isValid && validateInstruction(line);
    }

    free(token);
    return isValid;
}

boolean validateEntryExtern(char label[]) {
    boolean isValid;
    char *token;

    token = getNextToken(label);
    isValid = validateLabel(token);
    free(token);
    label = skipCharacters(label);

    if (checkIfFollowedByComma(label)) {
        printf("ERROR: Comma after the label.\n");
        isValid = FALSE;
    }

    label = skipWhitespace(label);

    if (*label != '\0') {
        printf("ERROR: Extra non-whitespace characters after the label.\n");
        return FALSE;
    }

    return isValid;
}

boolean validateLabel(char label[]) {
    boolean isValid;

    if (*label == '\0') {
        printf("ERROR: No label specified.\n");
        return FALSE;
    }

    isValid = TRUE;

    if (!isalpha(*label)) {
        printf("ERROR: Label starts with an invalid character - not a lowercase or uppercase letter in the English alphabet.\n");
        isValid = FALSE;
    }

    if (strlen(label) > 31) {
        printf("ERROR: Label is too long - maximum length is 31 characters.\n");
        isValid = FALSE;
    }

    if (getOperationIndex(label) != 16) {
        printf("ERROR: Label cannot share the same name as an operation.\n");
        isValid = FALSE;
    }

    if (label[0] == 'r' && label[1] >= '0' && label[1] <= '7' && label[2] == '\0') {
        printf("ERROR: Label cannot share the same name as a register.\n");
        isValid = FALSE;
    }

    while (*label != '\0') {
        if (!isalnum(*label) && *label != '_') {
            printf("ERROR: Label contains an invalid character - not a digit, nor a lowercase or uppercase letter in the English alphabet, nor an underscore.\n");
            isValid = FALSE;
        }

        label++;
    }

    return isValid;
}

boolean validateData(char data[]) {
    boolean isValid;
    boolean isFollowedByComma;
    char *token;

    if (*data == '\0') {
        printf("ERROR: No data specified.\n");
        return FALSE;
    }

    isValid = TRUE;
    isFollowedByComma = TRUE;

    while (*data != '\0') {
        if (!isFollowedByComma) {
            printf("ERROR: Missing comma between the numbers.\n");
            isValid = FALSE;
        }

        token = getNextToken(data);
        isValid = isValid && validateNumber(token);
        free(token);

        data = skipCharacters(data);

        if (checkForConsecutiveCommas(data)) {
            printf("ERROR: Multiple consecutive commas between the numbers.\n");
            isValid = FALSE;
        }

        isFollowedByComma = checkIfFollowedByComma(data);
        data = skipWhitespace(data);
    }

    if (isFollowedByComma) {
        printf("ERROR: The last number is followed by a comma.\n");
        return FALSE;
    }

    return isValid;
}

boolean validateNumber(char number[]) {
    if (*number == '+' || *number == '-') {
        number++;
    }

    while (*number != '\0') {
        if (!isdigit(*number)) {
            printf("ERROR: Invalid integer.\n");
            return FALSE;
        }

        number++;
    }

    return TRUE;
}

boolean validateString(char string[]) {
    boolean isValid;
    char *token;

    if (string[0] == '\0') {
        printf("ERROR: No string specified.\n");
        return FALSE;
    }

    isValid = TRUE;
    token = getNextToken(string);

    if (*token != '\"') {
        printf("ERROR: String does not start with a quotation mark.\n");
        isValid = FALSE;
    }

    if (token[strlen(token) - 1] != '\"') {
        printf("ERROR: String does not end with a quotation mark.\n");
        isValid = FALSE;
    }

    string = skipCharacters(string);

    if (checkIfFollowedByComma(string)) {
        printf("ERROR: Comma after the string.\n");
        isValid = FALSE;
    }

    string = skipWhitespace(string);

    if (*string != '\0') {
        printf("ERROR: Extra non-whitespace characters after the string.\n");
        isValid = FALSE;
    }

    free(token);
    return isValid;
}

boolean validateInstruction(char instruction[]) {
    char *operation;
    char *token;
    unsigned char operandCount;

    operation = getNextToken(instruction);

    if (!validateOperation(operation)) {
        printf("ERROR: \"%s\" is not a valid operation.\n", operation);
        free(operation);
        return FALSE;
    }

    operandCount = getOperandCount(operation);
    instruction = skipCharacters(instruction);
    instruction = skipWhitespace(instruction);

    if (operandCount == 0 && *instruction != '\0') {
        printf("ERROR: Extra non-whitespace characters after operation - \"%s\" should have no operands.\n", operation);
        free(operation);
        return FALSE;
    }

    if (operandCount == 0) {
        free(operation);
        return TRUE;
    }

    token = getNextToken(instruction);

    if (!validateOperand(token)) {
        free(operation);
        free(token);
        return FALSE;
    }

    if (!doesOperationAcceptOperand(operation, token, operandCount != 1)) {
        printf("ERROR: Operation \"%s\" does not accept operand \"%s\".\n", operation, token);
        return FALSE;
    }

    free(token);
    instruction = skipCharacters(instruction);

    if (operandCount == 1 && checkIfFollowedByComma(instruction)) {
        printf("ERROR: Comma after the only operand.\n");
        return FALSE;
    }

    if (operandCount == 2 && !checkIfFollowedByComma(instruction)) {
        printf("ERROR: Missing comma between the first and the second operand.\n");
        return FALSE;
    }

    instruction = skipWhitespace(instruction);

    if (operandCount == 1 && *instruction != '\0') {
        printf("ERROR: Extra non-whitespace characters after the operation - \"%s\" should have only one operand.\n", operation);
        free(operation);
        return FALSE;
    }

    if (operandCount == 1) {
        free(operation);
        return TRUE;
    }

    token = getNextToken(instruction);

    if (!validateOperand(token)) {
        free(operation);
        free(token);
        return FALSE;
    }

    if (!doesOperationAcceptOperand(operation, token, FALSE)) {
        printf("ERROR: Operation \"%s\" does not accept operand \"%s\".\n", operation, token);
        return FALSE;
    }

    free(token);
    instruction = skipCharacters(instruction);

    if (checkIfFollowedByComma(instruction)) {
        printf("ERROR: Comma after the second operand.\n");
        return FALSE;
    }

    instruction = skipWhitespace(instruction);

    if (*instruction != '\0') {
        printf("ERROR: Extra non-whitespace characters after the operation - \"%s\" should have only two operands.\n", operation);
        free(operation);
        return FALSE;
    }

    return TRUE;
}

boolean validateOperation(char operation[]) {
    return getOperationIndex(operation) != 16;
}

boolean validateOperand(char operand[]) {
    switch (getOperandType(operand)) {
        case IMMEDIATE:
            return validateImmediate(operand);
        case DIRECT:
            return validateLabel(operand);
        case INDIRECT_REGISTER:
            return validateIndirectRegister(operand);
        case DIRECT_REGISTER:
            return TRUE;
        default:
            return FALSE;
    }
}

boolean validateImmediate(char immediate[]) {
    int number;

    immediate++;

    if (!validateNumber(immediate)) {
        printf("ERROR: \"%s\" is not a number, even though it is preceded by a hash (a label cannot start with a hash).\n", immediate);
        return FALSE;
    }

    number = atoi(immediate);

    if (number > 2047) {
        printf("ERROR: \"%s\" is too large to store in just 12 bits - largest possible value is 2047.\n", immediate);
        return FALSE;
    }

    if (number < -2048) {
        printf("ERROR: \"%s\" is too small to store in just 12 bits - smallest possible value is -2048.\n", immediate);
        return FALSE;
    }

    return TRUE;
}

boolean validateIndirectRegister(char directRegister[]) {
    int number;

    if (directRegister[1] != 'r') {
        printf("ERROR: Token \"%s\" starts with an asterisk but does not include any register right after it (a label cannot start with an asterisk).\n", directRegister);
        return FALSE;
    }

    if (directRegister[2] == '\0') {
        printf("ERROR: Token \"%s\" looks like it should be a reference to a register, but does not specify which one (a label cannot start with an asterisk).\n", directRegister);
        return FALSE;
    }

    if (!validateNumber(&directRegister[2])) {
        printf("ERROR: \"%s\" is not a valid label (a label cannot start with an asterisk), nor does it specify a valid register number, even though it is preceded by \"*r\", signaling that there should be a valid register number right after.\n", directRegister);
        return FALSE;
    }

    number = atoi(&directRegister[2]);

    if (number > 7 || number < 0) {
        printf("ERROR: \"%s\" does not specify a valid register - the number should be between 0 and 7, including 0 and 7.\n", directRegister);
        return FALSE;
    }

    return TRUE;
}
