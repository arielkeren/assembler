/*
 * encoder.h
 *
 * Contains the function prototypes for the functions in encoder.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef ENCODER_H
#define ENCODER_H

#include "globals.h" /* Typedefs. */

void toggleBit(Word *word, Position bitPosition);
void applyMask(Word *word, Mask mask, Position from);
void encodeString(Word **data, char string[], WordCount *dataCount);
void encodeNumberList(Word **data, char numberList[], WordCount *dataCount);
void encodeAddressingMode(Word *word, char metadata);
void encodeExtraWord(Word *word, char operand[], Boolean isSource);
void encodeOperand(Word *word, char operand[], Boolean isSource);
void encodeRegister(Word *word, unsigned char registerNumber, Boolean isSource);
void encodeOperation(Word *word, char operation[]);
void encodeImmediate(Word *word, short number);
void encodeData(Word *word, short data);
void encodeLabel(Word *word, Address address);

#endif
