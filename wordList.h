#ifndef WORD_LIST_H
#define WORD_LIST_H

#include "globals.h"

void addWord(word **wordList);
void freeWordList(word *wordList);
boolean validateOperation(char operation[]);
unsigned char getOperandCount(char operation[]);
void toggleBit(word *wordToModify, unsigned char bitPosition);
void applyMask(word *wordToModify, unsigned char mask, unsigned char from);
operandType getOperandType(char operand[]);
void encodeOperation(word *wordToModify, char operation[]);
void encodeOperand(word *wordToModify, char operand[], int isSource);
void encodeMetadata(word *wordToModify, char metadata);
void encodeNumber(word *wordToModify, int number);
void encodeRegister(word *wordToModify, int registerNumber, boolean isSource);
void encodeExtraWord(word *wordToModify, char operand[], boolean isSource);
void encodeData(word *wordToModify, int data);
boolean validateString(char string[]);
void encodeString(word **data, char string[]);
void encodeNumberList(word **data, char numberList[]);

#endif
