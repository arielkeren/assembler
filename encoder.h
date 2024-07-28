#ifndef ENCODER_H
#define ENCODER_H

#include "globals.h"

void toggleBit(Word *word, Position bitPosition);
void applyMask(Word *word, Mask bitMask, Position from);
void encodeString(Word **data, char string[], unsigned *dataCount);
void encodeNumberList(Word **data, char numberList[], unsigned *dataCount);
void encodeMetadata(Word *word, char metadata);
void encodeExtraWord(Word *word, char operand[], Boolean isSource);
void encodeOperand(Word *word, char operand[], Boolean isSource);
void encodeRegister(Word *word, unsigned char registerNumber, Boolean isSource);
void encodeOperation(Word *word, char operation[]);
void encodeImmediate(Word *word, short number);
void encodeData(Word *word, short data);
void encodeLabel(Word *word, Address address);

#endif
