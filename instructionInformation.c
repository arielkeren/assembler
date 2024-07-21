#include "instructionInformation.h"

#include <string.h>

#include "globals.h"

unsigned char getOperationIndex(char operation[]) {
    static char *OPERATIONS[OPERATION_COUNT] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};

    unsigned char index;

    for (index = INITIAL_VALUE; index < OPERATION_COUNT; index++) {
        if (strcmp(operation, OPERATIONS[index]) == EQUAL_STRINGS) {
            return index;
        }
    }

    return INVALID_OPERATION;
}

operandCount getOperandCount(char operation[]) {
    unsigned char operationIndex;

    operationIndex = getOperationIndex(operation);

    if (operationIndex <= LAST_OPERATION_WITH_TWO_OPERANDS) {
        return TWO_OPERANDS;
    }

    if (operationIndex <= LAST_OPERATION_WITH_ONE_OPERAND) {
        return ONE_OPERAND;
    }

    return NO_OPERANDS;
}

operandType getOperandType(char operand[]) {
    switch (*operand) {
        case 'r':
            return ((operand[SECOND_INDEX] >= '0' && operand[SECOND_INDEX] <= '7') && operand[THIRD_INDEX] == '\0') ? DIRECT_REGISTER : DIRECT;
        case '*':
            return INDIRECT_REGISTER;
        case '#':
            return IMMEDIATE;
        default:
            return DIRECT;
    }
}

boolean doesOperationAcceptOperand(char operation[], char operand[], boolean isSource) {
    unsigned char index;
    operandType type;

    index = getOperationIndex(operation);
    type = getOperandType(operand);

    if (index == 14 || index == 15) {
        return FALSE;
    }

    if (isSource) {
        if (index <= 3) {
            return TRUE;
        }

        if (index == 4) {
            return type == DIRECT;
        }

        return FALSE;
    } else {
        if (type == DIRECT || type == INDIRECT_REGISTER) {
            return TRUE;
        }

        if (type == IMMEDIATE) {
            return index == 1 || index == 12;
        }

        return index <= 8 || index == 11 || index == 12;
    }
}
