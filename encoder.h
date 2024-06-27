#ifndef ENCODER_H
#define ENCODER_H

#include "globals.h"

void toggleBit(word *wordToModify, unsigned char bitPosition);
void applyMask(word *wordToModify, unsigned mask, unsigned char from);
void encodeOperation(word *wordToModify, char operation[]);
void encodeOperand(word *wordToModify, char operand[], boolean isSource);
void encodeMetadata(word *wordToModify, char metadata);
void encodeNumber(word *wordToModify, int number);
void encodeRegister(word *wordToModify, int registerNumber, boolean isSource);
void encodeExtraWord(word *wordToModify, char operand[], boolean isSource);
void encodeData(word *wordToModify, int data);
void encodeString(word **data, char string[], unsigned *dataCount);
void encodeNumberList(word **data, char numberList[], unsigned *dataCount);
void encodeLabel(word *wordToModify, unsigned labelAddress);

#endif
