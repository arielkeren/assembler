#ifndef LINE_VALIDATION_H
#define LINE_VALIDATION_H

#include "globals.h"

boolean validateLine(char line[], char fileName[], unsigned lineNumber);
boolean validateEntryExtern(char label[], char fileName[], unsigned lineNumber);
boolean validateLabel(char label[], char fileName[], unsigned lineNumber);
boolean validateMacro(char label[], char fileName[], unsigned lineNumber);
boolean validateName(char name[], char fileName[], unsigned lineNumber, boolean isLabel);
boolean validateData(char data[], char fileName[], unsigned lineNumber);
boolean validateNumber(char number[], char fileName[], unsigned lineNumber);
boolean validateString(char string[], char fileName[], unsigned lineNumber);
boolean validateInstruction(char instruction[], char fileName[], unsigned lineNumber);
boolean validateOperation(char operation[], char fileName[], unsigned lineNumber);
boolean validateOperand(char operand[], char fileName[], unsigned lineNumber);
boolean validateImmediate(char immediate[], char fileName[], unsigned lineNumber);
boolean validateIndirectRegister(char directRegister[], char fileName[], unsigned lineNumber);

#endif
