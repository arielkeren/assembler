#include "instructionInformation.h"

#include <string.h>

#include "globals.h"

unsigned char getOperationIndex(char operation[]) {
    static char *OPERATIONS[16] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};

    unsigned char index;

    for (index = 0; index < 16; index++) {
        if (strcmp(operation, OPERATIONS[index]) == 0) {
            return index;
        }
    }

    return 16;
}

unsigned char getOperandCount(char operation[]) {
    unsigned char operationIndex;

    operationIndex = getOperationIndex(operation);

    if (operationIndex <= 4) {
        return 2;
    }

    if (operationIndex <= 13) {
        return 1;
    }

    return 0;
}

operandType getOperandType(char operand[]) {
    switch (operand[0]) {
        case 'r':
            return ((operand[1] >= '0' && operand[1] <= '7') && operand[2] == '\0') ? DIRECT_REGISTER : DIRECT;
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
