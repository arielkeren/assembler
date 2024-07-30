/*
 * encoder.c
 *
 * Contains functions to encode the different types of data into the memory cells.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "encoder.h"

#include <stdlib.h> /* free, atoi. */

#include "globals.h"                /* Constants and typedefs. */
#include "instructionInformation.h" /* Information about the different instructions. */
#include "utils.h"                  /* Parsing lines. */
#include "wordList.h"               /* Adding new words to the data list. */

void toggleBit(Word *word, Position bitPosition) {
    if (bitPosition < (sizeof(word->data1) * BITS_PER_BYTE)) {
        word->data1 |= (SINGLE_BIT << bitPosition);
        return;
    }

    word->data2 |= (SINGLE_BIT << (bitPosition - (sizeof(word->data1) * BITS_PER_BYTE)));
}

void applyMask(Word *word, Mask mask, Position from) {
    while (mask != EMPTY && from < BITS_PER_MEMORY_CELL) {
        if (mask & SINGLE_BIT) {
            toggleBit(word, from);
        }

        mask >>= SINGLE_BIT;
        from++;
    }
}

void encodeString(Word **data, char string[], WordCount *dataCount) {
    Length length;

    length = getStringLength(string);
    string++;

    while (length > EMPTY) {
        (*dataCount)++;
        *data = addWord(*data);
        encodeData(*data, (short)*string);

        string++;
        length--;
    }

    (*dataCount)++;
    *data = addWord(*data);
}

void encodeNumberList(Word **data, char numberList[], WordCount *dataCount) {
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
            toggleBit(word, FIRST_BIT);
            break;
        case 'R':
            toggleBit(word, SECOND_BIT);
            break;
        case 'A':
            toggleBit(word, THIRD_BIT);
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
        toggleBit(word, (Position)getOperandType(operand) + STARTING_SOURCE_OPERAND_BIT);
        return;
    }

    toggleBit(word, (Position)getOperandType(operand) + STARTING_DESTINATION_OPERAND_BIT);
}

void encodeRegister(Word *word, unsigned char registerNumber, Boolean isSource) {
    applyMask(word, (Mask)registerNumber, isSource ? STARTING_SOURCE_REGISTER_BIT : STARTING_DESTINATION_REGISTER_BIT);
}

void encodeOperation(Word *word, char operation[]) {
    applyMask(word, (Mask)getOperationIndex(operation), STARTING_OPERATION_BIT);
}

void encodeImmediate(Word *word, short immediate) {
    applyMask(word, (Mask)immediate, STARTING_NUMBER_BIT);
}

void encodeData(Word *word, short data) {
    applyMask(word, (Mask)data, STARTING_DATA_BIT);
}

void encodeLabel(Word *word, Address address) {
    applyMask(word, (Mask)address, STARTING_LABEL_BIT);
}
