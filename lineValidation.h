#ifndef LINE_VALIDATION_H
#define LINE_VALIDATION_H

#include "globals.h"

boolean validateLine(char line[], unsigned lineNumber);
boolean validateEntryExtern(char label[], unsigned lineNumber);
boolean validateLabel(char label[], unsigned lineNumber);
boolean validateData(char data[], unsigned lineNumber);
boolean validateNumber(char number[], unsigned lineNumber);
boolean validateString(char string[], unsigned lineNumber);
boolean validateInstruction(char instruction[], unsigned lineNumber);
boolean validateOperation(char operation[], unsigned lineNumber);
boolean validateOperand(char operand[], unsigned lineNumber);
boolean validateImmediate(char immediate[], unsigned lineNumber);
boolean validateIndirectRegister(char directRegister[], unsigned lineNumber);

#endif
