/*
 * lineValidation.h
 *
 * Contains the function prototypes for the functions in lineValidation.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef LINE_VALIDATION_H
#define LINE_VALIDATION_H

#include "globals.h"

Boolean validateLine(char line[], char fileName[], LineNumber lineNumber);
Boolean validateEntryExtern(char label[], char fileName[], LineNumber lineNumber);
Boolean validateLabel(char label[], char fileName[], LineNumber lineNumber);
Boolean validateMacro(char label[], char fileName[], LineNumber lineNumber);
Boolean validateName(char name[], char fileName[], LineNumber lineNumber, Boolean isLabel);
Boolean validateData(char data[], char fileName[], LineNumber lineNumber);
Boolean validateNumber(char number[], char fileName[], LineNumber lineNumber);
Boolean validateString(char string[], char fileName[], LineNumber lineNumber);
Boolean validateInstruction(char instruction[], char fileName[], LineNumber lineNumber);
Boolean validateOperation(char operation[], char fileName[], LineNumber lineNumber);
Boolean validateOperand(char operand[], char fileName[], LineNumber lineNumber);
Boolean validateImmediate(char immediate[], char fileName[], LineNumber lineNumber);
Boolean validateIndirectRegister(char directRegister[], char fileName[], LineNumber lineNumber);

#endif
