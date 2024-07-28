#ifndef INSTRUCTION_INFORMATION_H
#define INSTRUCTION_INFORMATION_H

#include "globals.h"

Index getOperationIndex(char operation[]);
OperandCount getOperandCount(char operation[]);
OperandType getOperandType(char operand[]);
Boolean doesOperationAcceptOperand(char operation[], char operand[], Boolean isSource);

#endif
