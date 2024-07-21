#include "encoder.h"

#include <stdlib.h>

#include "globals.h"
#include "instructionInformation.h"
#include "utils.h"
#include "wordList.h"

void toggleBit(word *wordToModify, unsigned char bitPosition) {
    if (bitPosition < (sizeof(wordToModify->data1) * BITS_PER_BYTE)) {
        wordToModify->data1 |= (SINGLE_BIT << bitPosition);
        return;
    }

    wordToModify->data2 |= (SINGLE_BIT << (bitPosition - (sizeof(wordToModify->data1) * BITS_PER_BYTE)));
}

void applyMask(word *wordToModify, unsigned mask, unsigned char from) {
    while (mask != EMPTY && from < BITS_PER_MEMORY_CELL) {
        if (mask & SINGLE_BIT) {
            toggleBit(wordToModify, from);
        }

        mask >>= SINGLE_BIT;
        from++;
    }
}

void encodeOperation(word *wordToModify, char operation[]) {
    applyMask(wordToModify, getOperationIndex(operation), STARTING_OPERATION_BIT);
}

void encodeOperand(word *wordToModify, char operand[], boolean isSource) {
    if (isSource) {
        toggleBit(wordToModify, getOperandType(operand) + STARTING_SOURCE_OPERAND_BIT);
        return;
    }

    toggleBit(wordToModify, getOperandType(operand) + STARTING_DESTINATION_OPERAND_BIT);
}

void encodeMetadata(word *wordToModify, char metadata) {
    switch (metadata) {
        case 'E':
            toggleBit(wordToModify, FIRST_BIT);
            break;
        case 'R':
            toggleBit(wordToModify, SECOND_BIT);
            break;
        case 'A':
            toggleBit(wordToModify, THIRD_BIT);
            break;
        default:
            break;
    }
}

void encodeNumber(word *wordToModify, int number) {
    applyMask(wordToModify, number, STARTING_NUMBER_BIT);
}

void encodeRegister(word *wordToModify, unsigned char registerNumber, boolean isSource) {
    applyMask(wordToModify, registerNumber, isSource ? STARTING_SOURCE_REGISTER_BIT : STARTING_DESTINATION_REGISTER_BIT);
}

void encodeExtraWord(word *wordToModify, char operand[], boolean isSource) {
    switch (getOperandType(operand)) {
        case DIRECT_REGISTER:
            encodeMetadata(wordToModify, 'A');
            encodeRegister(wordToModify, convertDigitToNumber(operand[SECOND_INDEX]), isSource);
            break;
        case INDIRECT_REGISTER:
            encodeMetadata(wordToModify, 'A');
            encodeRegister(wordToModify, convertDigitToNumber(operand[THIRD_INDEX]), isSource);
            break;
        case IMMEDIATE:
            encodeMetadata(wordToModify, 'A');
            encodeNumber(wordToModify, atoi(&operand[SECOND_INDEX]));
            break;
        default:
            break;
    }
}

void encodeData(word *wordToModify, int data) {
    applyMask(wordToModify, data, STARTING_DATA_BIT);
}

void encodeString(word **data, char string[], unsigned *dataCount) {
    string++;

    while (*string != '\"') {
        (*dataCount)++;
        *data = addWord(*data);
        encodeData(*data, *string);

        string++;
    }

    (*dataCount)++;
    *data = addWord(*data);
}

void encodeNumberList(word **data, char numberList[], unsigned *dataCount) {
    char *token;

    while (*numberList != '\0') {
        token = getNextToken(numberList);
        removeEnding(token, ',');
        (*dataCount)++;
        *data = addWord(*data);
        encodeData(*data, atoi(token));

        free(token);
        numberList = skipCharacters(numberList);
        numberList = skipWhitespace(numberList);
    }
}

void encodeLabel(word *wordToModify, unsigned labelAddress) {
    applyMask(wordToModify, labelAddress, STARTING_LABEL_BIT);
}
