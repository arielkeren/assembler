#ifndef LINE_VALIDATION_H
#define LINE_VALIDATION_H

#include "globals.h"

Boolean validateLine(char line[], char fileName[], unsigned lineNumber);
Boolean validateEntryExtern(char label[], char fileName[], unsigned lineNumber);
Boolean validateLabel(char label[], char fileName[], unsigned lineNumber);
Boolean validateMacro(char label[], char fileName[], unsigned lineNumber);
Boolean validateName(char name[], char fileName[], unsigned lineNumber, Boolean isLabel);
Boolean validateData(char data[], char fileName[], unsigned lineNumber);
Boolean validateNumber(char number[], char fileName[], unsigned lineNumber);
Boolean validateString(char string[], char fileName[], unsigned lineNumber);
Boolean validateInstruction(char instruction[], char fileName[], unsigned lineNumber);
Boolean validateOperation(char operation[], char fileName[], unsigned lineNumber);
Boolean validateOperand(char operand[], char fileName[], unsigned lineNumber);
Boolean validateImmediate(char immediate[], char fileName[], unsigned lineNumber);
Boolean validateIndirectRegister(char directRegister[], char fileName[], unsigned lineNumber);

#endif
