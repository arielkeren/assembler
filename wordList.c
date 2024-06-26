#include "wordList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "utils.h"

void addWord(word **wordList) {
    word *newWord;

    newWord = allocate(sizeof(word));

    newWord->data1 = 0;
    newWord->data2 = 0;
    newWord->next = *wordList;
    *wordList = newWord;
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
    size_t operationIndex;

    operationIndex = indexOf(operation, OPERATIONS, 16);

    if (operationIndex <= 4) {
        return 2;
    }

    if (operationIndex <= 13) {
        return 1;
    }

    return 0;
}

void toggleBit(word *wordToModify, unsigned char bitPosition) {
    if (bitPosition < 8) {
        wordToModify->data1 |= (1 << bitPosition);
        return;
    }

    wordToModify->data2 |= (1 << (bitPosition - 8));
}

void applyMask(word *wordToModify, unsigned mask, unsigned char from) {
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
    applyMask(wordToModify, indexOf(operation, OPERATIONS, 16), 11);
}

void encodeOperand(word *wordToModify, char operand[], boolean isSource) {
    if (isSource) {
        toggleBit(wordToModify, getOperandType(operand) + 7);
        return;
    }

    toggleBit(wordToModify, getOperandType(operand) + 3);
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
    applyMask(wordToModify, registerNumber, isSource ? 6 : 3);
}

void encodeExtraWord(word *wordToModify, char operand[], boolean isSource) {
    switch (operand[0]) {
        case 'r':
            encodeMetadata(wordToModify, 'A');
            encodeRegister(wordToModify, atoi(&operand[1]), isSource);
            break;
        case '*':
            encodeMetadata(wordToModify, 'A');
            encodeRegister(wordToModify, atoi(&operand[2]), isSource);
            break;
        case '#':
            encodeMetadata(wordToModify, 'A');
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
    size_t length;

    length = strlen(string);

    return length >= 2 && ((string[0] == '\"') && (string[length - 1] == '\"'));
}

void encodeString(word **data, char string[], unsigned *dataCount) {
    string = &string[1];

    while (*string != '\"') {
        (*dataCount)++;
        addWord(data);
        encodeData(*data, *string);

        string++;
    }

    (*dataCount)++;
    addWord(data);
}

void encodeNumberList(word **data, char numberList[], unsigned *dataCount) {
    char *token;

    while (*numberList != '\0') {
        token = getNextToken(numberList);
        removeEnding(token, ',');
        (*dataCount)++;
        addWord(data);
        encodeData(*data, atoi(token));

        free(token);
        numberList = skipCharacters(numberList);
        numberList = skipWhitespace(numberList);
    }
}

void encodeLabel(word *wordToModify, unsigned labelAddress) {
    applyMask(wordToModify, labelAddress, 3);
}
