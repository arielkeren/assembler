#include "lineValidation.h"

#include "ctype.h"
#include "globals.h"
#include "utils.h"
#include "wordList.h"

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

    if (strcmp(token, ".entry") == 0) {
        isValid = isValid && validateLabel(skipWhitespace(skipCharacters(line)));
    } else if (strcmp(token, ".extern") == 0) {
        isValid = isValid && validateLabel(skipWhitespace(skipCharacters(line)));
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

boolean validateLabel(char label[]) {
    boolean isValid;
    char *current;

    if (label[0] == '\0') {
        printf("ERROR: No label specified.\n");
        return FALSE;
    }

    isValid = TRUE;
    current = label;

    if (!isalpha(label[0])) {
        printf("ERROR: Label \"%s\" starts with an invalid character \"%c\" - not a lowercase or uppercase letter in the English alphabet.\n", label, label[0]);
        isValid = FALSE;
    }

    while (*current != '\0') {
        if (!isalnum(*current) && *current != '_') {
            printf("ERROR: Label \"%s\" contains an invalid character \"%c\" - not a digit, nor a lowercase or uppercase letter in the English alphabet, nor an underscore.\n", label, *label);
            isValid = FALSE;
        }

        current++;
    }

    if (strlen(label) > 31) {
        printf("ERROR: Label \"%s\" is too long - maximum length is 31 characters.\n", label);
        return FALSE;
    }

    return isValid;
}

boolean validateData(char data[]) {
    boolean isValid;
    boolean isFollowedByComma;
    char *current;
    char *token;

    if (data[0] == '\0') {
        printf("ERROR: No data specified.\n");
        return FALSE;
    }

    isValid = TRUE;
    isFollowedByComma = TRUE;
    current = data;
    token = getNextToken(data);

    while (token != NULL) {
        if (!isFollowedByComma) {
            printf("ERROR: The numbers in \"%s\" are not separated by commas correctly - a comma is missing.\n", data);
            isValid = FALSE;
        }

        current = skipCharacters(current);

        isValid = isValid && validateNumber(token);
        free(token);

        if (checkForConsecutiveCommas(current)) {
            printf("ERROR: Multiple consecutive commas in \"%s\".\n", data);
            isValid = FALSE;
        }

        isFollowedByComma = checkIfFollowedByComma(current);
        current = skipWhitespace(current);
        token = getNextToken(current);
    }

    if (isFollowedByComma) {
        printf("ERROR: The last number in \"%s\" is followed by a comma.\n", data);
        return FALSE;
    }

    return isValid;
}

boolean validateNumber(char number[]) {
    char *current;

    current = number;

    if (number[0] == '+' || number[0] == '-') {
        current++;
    }

    while (*current != '\0') {
        if (!isdigit(*current)) {
            printf("ERROR: Data \"%s\" contains an invalid character \"%c\" - not a valid number.\n", number, *current);
            return FALSE;
        }

        current++;
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

    if (token[0] != '\"') {
        printf("ERROR: String \"%s\" does not start with a quotation mark.\n", token);
        isValid = FALSE;
    }

    if (token[strlen(token) - 1] != '\"') {
        printf("ERROR: String \"%s\" does not end with a quotation mark.\n", token);
        isValid = FALSE;
    }

    string = skipCharacters(string);
    string = skipWhitespace(string);

    if (*string != '\0') {
        printf("ERROR: Extra non-whitespace characters after the string.\n");
        return FALSE;
    }

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
        printf("ERROR: Extra non-whitespace characters after the operation - \"%s\" should have no operands.\n", operation);
        free(operation);
        return FALSE;
    }

    token = getNextToken(instruction);
}

boolean validateOperation(char operation[]) {
    return contains(operation, OPERATIONS, 16);
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
            return validateDirectRegister(operand);
        default:
            return FALSE;
    }
}

boolean validateImmediate(char immediate[]) {
    int number;

    immediate++;

    if (!validateNumber(immediate)) {
        printf("ERROR: \"%s\" is not a number, even though it is preceded by a hash.\n", token);
        return FALSE;
    }

    number = atoi(immediate);

    if (number > 2047) {
        printf("ERROR: \"%s\" is too large to store in just 12 bits - largest possible value is 2047.\n", token);
        return FALSE;
    }

    if (number < -2048) {
        printf("ERROR: \"%s\" is too small to store in just 12 bits - smallest possible value is -2048.\n", token);
        return FALSE;
    }

    return TRUE;
}

boolean validateDirectRegister(char directRegister[]) {
    directRegister++;

    if (!validateNumber(directRegister)) {
        printf("ERROR: \"%s\" is not a number, even though it is preceded by an \"r\", signaling that it should be a register number.\n", directRegister);
        return FALSE;
    }
}
