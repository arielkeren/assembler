#ifndef LINE_VALIDATION_H
#define LINE_VALIDATION_H

#include "globals.h"

boolean validateLine(char line[]);
boolean validateLabel(char label[]);
boolean validateData(char data[]);
boolean validateNumber(char number[]);
boolean validateString(char string[]);
boolean validateInstruction(char instruction[]);
boolean validateOperation(char operation[]);
boolean validateOperand(char operand[]);
boolean validateImmediate(char immediate[]);
boolean validateIndirectRegister(char directRegister[]);

#endif
