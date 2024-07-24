#include "lineValidation.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "instructionInformation.h"
#include "utils.h"

boolean validateLine(char line[], char fileName[], unsigned lineNumber) {
    boolean isValid;
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
        isValid = isValid && validateEntryExtern(skipWhitespace(skipCharacters(line)), fileName, lineNumber);
    } else if (strcmp(token, ".extern") == EQUAL_STRINGS) {
        isValid = isValid && validateEntryExtern(skipWhitespace(skipCharacters(line)), fileName, lineNumber);
    } else if (strcmp(token, ".data") == EQUAL_STRINGS) {
        isValid = isValid && validateData(skipWhitespace(skipCharacters(line)), fileName, lineNumber);
    } else if (strcmp(token, ".string") == EQUAL_STRINGS) {
        isValid = isValid && validateString(skipWhitespace(skipCharacters(line)), fileName, lineNumber);
    } else {
        isValid = isValid && validateInstruction(line, fileName, lineNumber);
    }

    free(token);
    return isValid;
}

boolean validateEntryExtern(char label[], char fileName[], unsigned lineNumber) {
    boolean isValid;
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

boolean validateLabel(char label[], char fileName[], unsigned lineNumber) {
    return validateName(label, fileName, lineNumber, TRUE);
}

boolean validateMacro(char label[], char fileName[], unsigned lineNumber) {
    return validateName(label, fileName, lineNumber, FALSE);
}

boolean validateName(char name[], char fileName[], unsigned lineNumber, boolean isLabel) {
    boolean isValid;

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

boolean validateData(char data[], char fileName[], unsigned lineNumber) {
    boolean isValid;
    boolean isFollowedByComma;
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
        isValid = isValid && validateNumber(token, fileName, lineNumber);
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

boolean validateNumber(char number[], char fileName[], unsigned lineNumber) {
    if (*number == '+' || *number == '-') {
        number++;
    }

    while (*number != '\0') {
        if (!isdigit(*number)) {
            printError("Invalid integer.", fileName, lineNumber);
            return FALSE;
        }

        number++;
    }

    return TRUE;
}

boolean validateString(char string[], char fileName[], unsigned lineNumber) {
    boolean isValid;
    char *token;

    if (*string == '\0') {
        printError("No string specified.", fileName, lineNumber);
        return FALSE;
    }

    isValid = TRUE;
    token = getNextToken(string);

    if (*token != '\"') {
        printError("String does not start with a quotation mark.", fileName, lineNumber);
        isValid = FALSE;
    }

    if (token[strlen(token) - LAST_INDEX_DIFF] != '\"') {
        printError("String does not end with a quotation mark.", fileName, lineNumber);
        isValid = FALSE;
    }

    string = skipCharacters(string);

    if (checkIfFollowedByComma(string)) {
        printError("Comma after the string.", fileName, lineNumber);
        isValid = FALSE;
    }

    string = skipWhitespace(string);

    if (*string != '\0') {
        printError("Extra non-whitespace characters after the string.", fileName, lineNumber);
        isValid = FALSE;
    }

    free(token);
    return isValid;
}

boolean validateInstruction(char instruction[], char fileName[], unsigned lineNumber) {
    char *operation;
    char *token;
    operandCount operands;

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

boolean validateOperation(char operation[], char fileName[], unsigned lineNumber) {
    if (getOperationIndex(operation) == INVALID_OPERATION) {
        printError("Invalid operation.", fileName, lineNumber);
        return FALSE;
    }

    return TRUE;
}

boolean validateOperand(char operand[], char fileName[], unsigned lineNumber) {
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

boolean validateImmediate(char immediate[], char fileName[], unsigned lineNumber) {
    int number;

    immediate++;

    if (!validateNumber(immediate, fileName, lineNumber)) {
        printError("Not a number, even though it is preceded by a hash (a label cannot start with a hash).", fileName, lineNumber);
        return FALSE;
    }

    number = atoi(immediate);

    if (number > MAX_NUMBER) {
        printError("Number is too large to store in just 12 bits - largest possible value is 2047.", fileName, lineNumber);
        return FALSE;
    }

    if (number < MIN_NUMBER) {
        printError("Number is too small to store in just 12 bits - smallest possible value is -2048.", fileName, lineNumber);
        return FALSE;
    }

    return TRUE;
}

boolean validateIndirectRegister(char directRegister[], char fileName[], unsigned lineNumber) {
    directRegister++;
    if (*directRegister != 'r') {
        printError("Token starts with an asterisk but does not include any register right after it (a label cannot start with an asterisk).", fileName, lineNumber);
        return FALSE;
    }

    directRegister++;
    if (*directRegister == '\0') {
        printError("Token looks like it should be a register, but does not specify which one (a label cannot start with an asterisk).", fileName, lineNumber);
        return FALSE;
    }

    if (directRegister[FIRST_INDEX] > '7' || directRegister[FIRST_INDEX] < '0' || directRegister[SECOND_INDEX] != '\0') {
        printError("Token does not specify a valid register - a register's number should be between 0 and 7, including 0 and 7.", fileName, lineNumber);
        return FALSE;
    }

    return TRUE;
}
