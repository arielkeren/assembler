#ifndef ENCODER_H
#define ENCODER_H

#include "globals.h"

void toggleBit(word *wordToModify, position bitPosition);
void applyMask(word *wordToModify, mask bitMask, position from);
void encodeString(word **data, char string[], unsigned *dataCount);
void encodeNumberList(word **data, char numberList[], unsigned *dataCount);
void encodeMetadata(word *wordToModify, char metadata);
void encodeExtraWord(word *wordToModify, char operand[], boolean isSource);
void encodeOperand(word *wordToModify, char operand[], boolean isSource);
void encodeRegister(word *wordToModify, unsigned char registerNumber, boolean isSource);
void encodeOperation(word *wordToModify, char operation[]);
void encodeImmediate(word *wordToModify, short number);
void encodeData(word *wordToModify, short data);
void encodeLabel(word *wordToModify, unsigned short labelAddress);

#endif
