/*
 * instructionInformation.h
 *
 * Contains the function prototypes for the functions in
 * instructionInformation.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef INSTRUCTION_INFORMATION_H
#define INSTRUCTION_INFORMATION_H

#include "globals.h" /* Typedefs. */

/**
 * Computes and returns the unique index of the given operation from an array in
 * memory.
 *
 * Assumes that the given operation is not NULL and is null-terminated.
 *
 * @param operation The operation to get the index of.
 * @return The unique index of the given operation.
 */
Index getOperationIndex(char operation[]);

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
OperandCount getOperandCount(char operation[]);

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
OperandType getOperandType(char operand[]);

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
                                   Boolean isSource);

#endif
