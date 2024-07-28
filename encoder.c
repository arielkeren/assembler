#include "encoder.h"

#include <stdlib.h>

#include "globals.h"
#include "instructionInformation.h"
#include "utils.h"
#include "wordList.h"

void toggleBit(Word *word, Position bitPosition) {
    if (bitPosition < (Position)(sizeof(word->data1) * BITS_PER_BYTE)) {
        word->data1 |= ((unsigned char)SINGLE_BIT << bitPosition);
        return;
    }

    word->data2 |= ((unsigned char)SINGLE_BIT << (bitPosition - (Position)(sizeof(word->data1) * BITS_PER_BYTE)));
}

void applyMask(Word *word, Mask bitMask, Position from) {
    while (bitMask != (Mask)EMPTY && from < (Position)BITS_PER_MEMORY_CELL) {
        if (bitMask & (Mask)SINGLE_BIT) {
            toggleBit(word, from);
        }

        bitMask >>= (Mask)SINGLE_BIT;
        from++;
    }
}

void encodeString(Word **data, char string[], unsigned *dataCount) {
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

void encodeNumberList(Word **data, char numberList[], unsigned *dataCount) {
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

void encodeMetadata(Word *word, char metadata) {
    switch (metadata) {
        case 'E':
            toggleBit(word, (Position)FIRST_BIT);
            break;
        case 'R':
            toggleBit(word, (Position)SECOND_BIT);
            break;
        case 'A':
            toggleBit(word, (Position)THIRD_BIT);
            break;
        default:
            break;
    }
}

void encodeExtraWord(Word *word, char operand[], Boolean isSource) {
    switch (getOperandType(operand)) {
        case DIRECT_REGISTER:
            encodeMetadata(word, 'A');
            encodeRegister(word, convertDigitToNumber(operand[SECOND_INDEX]), isSource);
            break;
        case INDIRECT_REGISTER:
            encodeMetadata(word, 'A');
            encodeRegister(word, convertDigitToNumber(operand[THIRD_INDEX]), isSource);
            break;
        case IMMEDIATE:
            encodeMetadata(word, 'A');
            encodeImmediate(word, (short)atoi(&operand[SECOND_INDEX]));
            break;
        default:
            break;
    }
}

void encodeOperand(Word *word, char operand[], Boolean isSource) {
    if (isSource) {
        toggleBit(word, (Position)getOperandType(operand) + (Position)STARTING_SOURCE_OPERAND_BIT);
        return;
    }

    toggleBit(word, (Position)getOperandType(operand) + (Position)STARTING_DESTINATION_OPERAND_BIT);
}

void encodeRegister(Word *word, unsigned char registerNumber, Boolean isSource) {
    applyMask(word, (Mask)registerNumber, isSource ? (Position)STARTING_SOURCE_REGISTER_BIT : (Position)STARTING_DESTINATION_REGISTER_BIT);
}

void encodeOperation(Word *word, char operation[]) {
    applyMask(word, (Mask)getOperationIndex(operation), (Position)STARTING_OPERATION_BIT);
}

void encodeImmediate(Word *word, short immediate) {
    applyMask(word, (Mask)immediate, (Position)STARTING_NUMBER_BIT);
}

void encodeData(Word *word, short data) {
    applyMask(word, (Mask)data, (Position)STARTING_DATA_BIT);
}

void encodeLabel(Word *word, Address address) {
    applyMask(word, (Mask)address, (Position)STARTING_LABEL_BIT);
}
