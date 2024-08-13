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
