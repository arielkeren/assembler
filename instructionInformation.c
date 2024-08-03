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

#include <string.h> /* strcmp. */

#include "globals.h" /* Constants and typedefs. */

/**
 * Computes and returns the unique index of the given operation from an array in memory.
 *
 * Assumes that the given operation is not NULL and is null-terminated.
 *
 * @param operation The operation to get the index of.
 * @return The unique index of the given operation.
 */
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

/**
 * Computes and returns the number of operands the given operation expects to get.
 * Can be either 0, 1 or 2.
 *
 * Assumes that the given operation is not NULL, is null-terminated and represents an actual operation.
 *
 * @param operation The operation to get the number of operands for.
 * @return The number of operands the given operation expects to get (0, 1 or 2).
 */
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

/**
 * Computes and returns the type (addressing mode) of the given operand.
 * Can be either IMMEDIATE, DIRECT, INDIRECT_REGISTER or DIRECT_REGISTER.
 *
 * Assumes that the given operand is not NULL, is null-terminated and represents an actual operand.
 *
 * @param operand The operand to get the type (addressing mode) of.
 * @return The type (addressing mode) of the operand.
 */
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

/**
 * Checks and returns whether or not the given operation can receive the given operand.
 *
 * Assumes that the given operation is not NULL, is null-terminated and represents an actual operation.
 * Assumes that the given operand is not NULL, is null-terminated and represents an actual operand.
 *
 * @param operation The operation to check.
 * @param operand The operand to check.
 * @param isSource Whether the operand is the source or the destination of the instruction.
 * @return Whether or not the given operation can receive the given operand.
 */
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
