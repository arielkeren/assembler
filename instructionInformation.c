/*
 * instructionInformation.c
 *
 * Contains functions for getting the necessary information about the different
 * instructions to encode them.
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
 * Computes and returns the unique index of the given operation from an array in
 * memory.
 *
 * Assumes that the given operation is not NULL and is null-terminated.
 *
 * @param operation The operation to get the index of.
 * @return The unique index of the given operation.
 */
Index getOperationIndex(char operation[]) {
    /* The array defining the unique identifiers for each operation. */
    static char *OPERATIONS[OPERATION_COUNT] = {
        "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
        "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};

    Index index; /* The current index in the array. */

    /* Loop over the array to search for the given operation. */
    for (index = INITIAL_VALUE; index < OPERATION_COUNT; index++) {
        /* Compare the names. */
        if (strcmp(operation, OPERATIONS[index]) == EQUAL_STRINGS) {
            /* The operation has been found. */
            return index;
        }
    }

    /* The operation has not been found. */
    return INVALID_OPERATION;
}

/**
 * Computes and returns the number of operands the given operation expects to
 * get.
 * Can be either 0, 1 or 2.
 *
 * Assumes that the given operation is not NULL, is null-terminated and
 * represents an actual operation.
 *
 * @param operation The operation to get the number of operands for.
 * @return The number of operands the given operation expects to get (0, 1 or
 * 2).
 */
OperandCount getOperandCount(char operation[]) {
    Index operationIndex; /* The unique index of the given operation. */

    /* Operations that expect the same number of operands are close together. */
    operationIndex = getOperationIndex(operation);

    /* The last operation that expects 2 operands is the 5th one. */
    if (operationIndex <= LAST_OPERATION_WITH_TWO_OPERANDS) {
        return TWO_OPERANDS;
    }

    /* The last operation that expects 1 operand is the 14th one. */
    if (operationIndex <= LAST_OPERATION_WITH_ONE_OPERAND) {
        return ONE_OPERAND;
    }

    /* After that, all the operations expect no operands. */
    return NO_OPERANDS;
}

/**
 * Computes and returns the type (addressing mode) of the given operand.
 * Can be either:
 * 0 - immediate, 1 - direct, 2 - indirect register or 3 - direct register.
 *
 * Assumes that the given operand is not NULL, is null-terminated and represents
 * an actual operand.
 *
 * @param operand The operand to get the type (addressing mode) of.
 * @return The type (addressing mode) of the operand.
 */
OperandType getOperandType(char operand[]) {
    /* Determine the type based on the first character of the operand. */
    switch (*operand) {
        case 'r':
            /* Not all operands that start with an R are direct registers. */
            return ((operand[SECOND_INDEX] >= '0' &&
                     operand[SECOND_INDEX] <= '7') &&
                    operand[THIRD_INDEX] == '\0')
                       ? DIRECT_REGISTER
                       : DIRECT;
        case '*':
            /* After validation, these should all be indirect registers. */
            return INDIRECT_REGISTER;
        case '#':
            /* After validation, these should all be immediate values. */
            return IMMEDIATE;
        default:
            /* All other operands are direct. */
            return DIRECT;
    }
}

/**
 * Checks and returns whether or not the given operation can receive the given
 * operand.
 *
 * Assumes that the given operation is not NULL, is null-terminated and
 * represents an actual operation.
 * Assumes that the given operand is not NULL, is null-terminated and represents
 * an actual operand.
 *
 * @param operation The operation to check.
 * @param operand The operand to check.
 * @param isSource Whether the operand is the source or the destination of the
 * instruction.
 * @return Whether or not the given operation can receive the given operand.
 */
Boolean doesOperationAcceptOperand(char operation[], char operand[],
                                   Boolean isSource) {
    Index index;      /* The index of the given operation. */
    OperandType type; /* The type of the given operand. */

    /* Get the index and the addressing mode. */
    index = getOperationIndex(operation);
    type = getOperandType(operand);

    /* These operations do not expect any operands. */
    if (index == RTS || index == STOP) {
        return FALSE;
    }

    /* Split the cases into 2 groups - source and destination. */
    if (isSource) {
        /* Split based on the addressing mode. */
        switch (type) {
            case IMMEDIATE:
                /* Only the operations that appear are valid for the type. */
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
                /* Should not happen. */
                return FALSE;
        }
    }

    /* Split based on the addressing mode. */
    switch (type) {
        case IMMEDIATE:
            /* Only the operations that appear are valid for the type. */
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
            /* Should not happen. */
            return FALSE;
    }
}
