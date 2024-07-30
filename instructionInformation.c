/*
 * instructionInformation.c
 *
 * Contains functions for getting the necessary information about the different instructions to encode them.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "instructionInformation.h"

#include <string.h>

#include "globals.h"

Index getOperationIndex(char operation[]) {
    static char *OPERATIONS[OPERATION_COUNT] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};

    Index index;

    for (index = INITIAL_VALUE; index < OPERATION_COUNT; index++) {
        if (strcmp(operation, OPERATIONS[index]) == EQUAL_STRINGS) {
            return index;
        }
    }

    return INVALID_OPERATION;
}

OperandCount getOperandCount(char operation[]) {
    Index operationIndex;

    operationIndex = getOperationIndex(operation);

    if (operationIndex <= LAST_OPERATION_WITH_TWO_OPERANDS) {
        return TWO_OPERANDS;
    }

    if (operationIndex <= LAST_OPERATION_WITH_ONE_OPERAND) {
        return ONE_OPERAND;
    }

    return NO_OPERANDS;
}

OperandType getOperandType(char operand[]) {
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

Boolean doesOperationAcceptOperand(char operation[], char operand[], Boolean isSource) {
    Index index;
    OperandType type;

    index = getOperationIndex(operation);
    type = getOperandType(operand);

    if (index == RTS || index == STOP) {
        return FALSE;
    }

    if (isSource) {
        switch (type) {
            case IMMEDIATE:
                return index == MOV || index == CMP || index == ADD ||
                       index == SUB;

            case DIRECT:
                return index == MOV || index == CMP || index == ADD ||
                       index == SUB || index == LEA;

            case INDIRECT_REGISTER:
                return index == MOV || index == CMP || index == ADD ||
                       index == SUB;

            case DIRECT_REGISTER:
                return index == MOV || index == CMP || index == ADD ||
                       index == SUB;

            default:
                return FALSE;
        }
    }

    switch (type) {
        case IMMEDIATE:
            return index == CMP || index == PRN;

        case DIRECT:
            return index == MOV || index == CMP || index == ADD ||
                   index == SUB || index == LEA || index == CLR ||
                   index == NOT || index == INC || index == DEC ||
                   index == JMP || index == BNE || index == RED ||
                   index == PRN || index == JSR;

        case INDIRECT_REGISTER:
            return index == MOV || index == CMP || index == ADD ||
                   index == SUB || index == LEA || index == CLR ||
                   index == NOT || index == INC || index == DEC ||
                   index == JMP || index == BNE || index == RED ||
                   index == PRN || index == JSR;

        case DIRECT_REGISTER:
            return index == MOV || index == CMP || index == ADD ||
                   index == SUB || index == LEA || index == CLR ||
                   index == NOT || index == INC || index == DEC ||
                   index == RED || index == PRN;

        default:
            return FALSE;
    }
}
