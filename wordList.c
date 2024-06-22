#include "wordList.h"

#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "utils.h"

void addWord(word **wordList) {
    word *newWord;

    newWord = allocate(sizeof(word));

    newWord->data1 = 0;
    newWord->data2 = 0;
    newWord->next = *wordList;
}

void freeWordList(word *wordList) {
    word *next;

    while (wordList != NULL) {
        next = wordList->next;
        free(wordList);
        wordList = next;
    }
}

boolean validateOperation(char operation[]) {
    return contains(operation, OPERATIONS, 16);
}

unsigned char getOperandCount(char operation[]) {
    switch (indexOf(operation, OPERATIONS, 16)) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            return 2;
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            return 1;
        default:
            return 0;
    }
}

void toggleBit(word *wordToModify, unsigned char bitPosition) {
    if (bitPosition < 8) {
        wordToModify->data1 |= (1 << bitPosition);
    }

    wordToModify->data2 |= (1 << bitPosition);
}

void applyMask(word *wordToModify, unsigned char mask, unsigned char from) {
    while (mask != 0 && from < 15) {
        if (mask & 1) {
            toggleBit(wordToModify, from);
        }

        mask >>= 1;
        from++;
    }
}

operandType getOperandType(char operand[]) {
    switch (operand[0]) {
        case 'r':
            return DIRECT_REGISTER;
        case '*':
            return INDIRECT_REGISTER;
        case '#':
            return IMMEDIATE;
        default:
            return DIRECT;
    }
}

void encodeOperation(word *wordToModify, char operation[]) {
    wordToModify->data2 |= (indexOf(operation, OPERATIONS, 16) << 2);
}

void encodeOperand(word *wordToModify, char operand[], int isSource) {
    applyMask(wordToModify, getOperandType(operand), 3);
}

void encodeMetadata(word *wordToModify, char metadata) {
    switch (metadata) {
        case 'A':
            toggleBit(wordToModify, 2);
            break;
        case 'R':
            toggleBit(wordToModify, 1);
            break;
        case 'E':
            toggleBit(wordToModify, 0);
            break;
        default:
            break;
    }
}

void encodeNumber(word *wordToModify, int number) {
    applyMask(wordToModify, number, 3);
}

void encodeRegister(word *wordToModify, int registerNumber, boolean isSource) {
    applyMask(wordToModify, registerNumber, isSource ? 3 : 6);
}

void encodeExtraWord(word *wordToModify, char operand[], boolean isSource) {
    switch (operand[0]) {
        case 'r':
            encodeRegister(wordToModify, atoi(&operand[1]), isSource);
            break;
        case '*':
            encodeRegister(wordToModify, atoi(&operand[2]), isSource);
            break;
        case '#':
            encodeNumber(wordToModify, atoi(&operand[1]));
            break;
        default:
            break;
    }
}

void encodeData(word *wordToModify, int data) {
    applyMask(wordToModify, data, 0);
}

boolean validateString(char string[]) {
}

void encodeString(word **data, char string[]) {
    string = &string[1];
    while (*string != '\"') {
        addWord(data);
        encodeData(*data, *string);
    }

    addWord(data);
}

void encodeNumberList(word **data, char numberList[]) {
    while (*numberList != NULL) {
        addWord(data);
    }
}
