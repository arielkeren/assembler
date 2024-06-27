#ifndef INSTRUCTION_INFORMATION_H
#define INSTRUCTION_INFORMATION_H

#include "globals.h"

unsigned char getOperationIndex(char operation[]);
unsigned char getOperandCount(char operation[]);
operandType getOperandType(char operand[]);
boolean doesOperationAcceptOperand(char operation[], char operand[], boolean isSource);

#endif
