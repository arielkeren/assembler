#include "encoder.h"

#include <stdlib.h>

#include "globals.h"
#include "instructionInformation.h"
#include "utils.h"
#include "wordList.h"

void toggleBit(word *wordToModify, position bitPosition) {
    if (bitPosition < (position)(sizeof(wordToModify->data1) * BITS_PER_BYTE)) {
        wordToModify->data1 |= ((unsigned char)SINGLE_BIT << bitPosition);
        return;
    }

    wordToModify->data2 |= ((unsigned char)SINGLE_BIT << (bitPosition - (position)(sizeof(wordToModify->data1) * BITS_PER_BYTE)));
}

void applyMask(word *wordToModify, mask bitMask, position from) {
    while (bitMask != (mask)EMPTY && from < (position)BITS_PER_MEMORY_CELL) {
        if (bitMask & (mask)SINGLE_BIT) {
            toggleBit(wordToModify, from);
        }

        bitMask >>= (mask)SINGLE_BIT;
        from++;
    }
}

void encodeString(word **data, char string[], unsigned *dataCount) {
    string++;

    while (*string != '\"') {
        (*dataCount)++;
        *data = addWord(*data);
        encodeData(*data, (short)*string);

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
        encodeData(*data, (short)atoi(token));

        free(token);
        numberList = skipCharacters(numberList);
        numberList = skipWhitespace(numberList);
    }
}

void encodeMetadata(word *wordToModify, char metadata) {
    switch (metadata) {
        case 'E':
            toggleBit(wordToModify, (position)FIRST_BIT);
            break;
        case 'R':
            toggleBit(wordToModify, (position)SECOND_BIT);
            break;
        case 'A':
            toggleBit(wordToModify, (position)THIRD_BIT);
            break;
        default:
            break;
    }
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
            encodeImmediate(wordToModify, (short)atoi(&operand[SECOND_INDEX]));
            break;
        default:
            break;
    }
}

void encodeOperand(word *wordToModify, char operand[], boolean isSource) {
    if (isSource) {
        toggleBit(wordToModify, (position)getOperandType(operand) + (position)STARTING_SOURCE_OPERAND_BIT);
        return;
    }

    toggleBit(wordToModify, (position)getOperandType(operand) + (position)STARTING_DESTINATION_OPERAND_BIT);
}

void encodeRegister(word *wordToModify, unsigned char registerNumber, boolean isSource) {
    applyMask(wordToModify, (mask)registerNumber, isSource ? (position)STARTING_SOURCE_REGISTER_BIT : (position)STARTING_DESTINATION_REGISTER_BIT);
}

void encodeOperation(word *wordToModify, char operation[]) {
    applyMask(wordToModify, (mask)getOperationIndex(operation), (position)STARTING_OPERATION_BIT);
}

void encodeImmediate(word *wordToModify, short immediate) {
    applyMask(wordToModify, (mask)immediate, (position)STARTING_NUMBER_BIT);
}

void encodeData(word *wordToModify, short data) {
    applyMask(wordToModify, (mask)data, (position)STARTING_DATA_BIT);
}

void encodeLabel(word *wordToModify, unsigned short labelAddress) {
    applyMask(wordToModify, (mask)labelAddress, (position)STARTING_LABEL_BIT);
}
