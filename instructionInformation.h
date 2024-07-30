/*
 * instructionInformation.h
 *
 * Contains the function prototypes for the functions in instructionInformation.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef INSTRUCTION_INFORMATION_H
#define INSTRUCTION_INFORMATION_H

#include "globals.h"

Index getOperationIndex(char operation[]);
OperandCount getOperandCount(char operation[]);
OperandType getOperandType(char operand[]);
Boolean doesOperationAcceptOperand(char operation[], char operand[], Boolean isSource);

#endif
