/*
 * encoder.c
 *
 * Contains functions to encode the different types of data into the memory
 * cells.
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
    Position adjustedPosition; /* Used when the bit is in the data2 property. */

    /* Check if the bit is in the data1 property. */
    if (bitPosition < (sizeof(word->data1) * BITS_PER_BYTE)) {
        /* Toggle the bit in the data1 property. */
        word->data1 |= (SINGLE_BIT << bitPosition);
        return;
    }

    /* Adjust the position according to the data1's size in bits. */
    adjustedPosition = bitPosition - (sizeof(word->data1) * BITS_PER_BYTE);
    /* Toggle the bit in the data2 property. */
    word->data2 |= (SINGLE_BIT << adjustedPosition);
}

void applyMask(Word *word, Mask mask, Position from) {
    /* Constantly shift the mask and toggle each 1 bit. Limit the position. */
    while (mask != EMPTY && from < BITS_PER_MEMORY_CELL) {
        /* Check if the bit is a 1. */
        if (mask & SINGLE_BIT) {
            /* Toggle the bit. */
            toggleBit(word, from);
        }

        /* Shift the mask. */
        mask >>= SINGLE_BIT;
        /* Increment the position. */
        from++;
    }
}

void encodeString(Word **data, char string[], WordCount *dataCount) {
    Length length; /* The length of the string. */

    /* Compute the .string line's length. */
    length = getStringLength(string);
    /* Skip the opening double-quote character. */
    string++;

    /* Encode every character in the string. */
    while (length > EMPTY) {
        /* Increment the data count. */
        (*dataCount)++;
        /* Add another word to the data part. */
        *data = addWord(*data);
        /* Encode the character's ASCII value into the new word. */
        encodeData(*data, (short)*string);

        /* Move on to the next character. */
        string++;
        length--;
    }

    /* Increment the data count for the null character. */
    (*dataCount)++;
    /* Add an empty word, representing the null character. */
    *data = addWord(*data);
}

void encodeNumberList(Word **data, char numberList[], WordCount *dataCount) {
    char *token; /* The current token. */

    /* Encode every number in the number list. */
    while (*numberList != '\0') {
        /* Get the next token. */
        token = getNextToken(numberList);
        /* Remove the possible comma. */
        removeEnding(token, ',');
        /* Increment the data count. */
        (*dataCount)++;
        /* Add another word to the data part. */
        *data = addWord(*data);
        /* Encode the number's 2's complement representation. */
        encodeData(*data, (short)atoi(token));

        /* The current token is no longer used. */
        free(token);
        /* Move on to the next token. */
        numberList = skipCharacters(numberList);
        numberList = skipWhitespace(numberList);
    }
}

void encodeMetadata(Word *word, char metadata) {
    /* Determine which bit to toggle. */
    switch (metadata) {
        case 'E':
            /* E toggles bit 0. */
            toggleBit(word, FIRST_BIT);
            break;
        case 'R':
            /* R toggles bit 1. */
            toggleBit(word, SECOND_BIT);
            break;
        case 'A':
            /* A toggles bit 2. */
            toggleBit(word, THIRD_BIT);
            break;
        default:
            /* Should not happen. */
            break;
    }
}

void encodeExtraWord(Word *word, char operand[], Boolean isSource) {
    /* Determine the addressing mode of the operand before encoding it. */
    switch (getOperandType(operand)) {
        case DIRECT_REGISTER:
            /* Encode a direct register. */
            encodeMetadata(word, 'A');
            encodeRegister(word, convertDigitToNumber(operand[SECOND_INDEX]),
                           isSource);
            break;
        case INDIRECT_REGISTER:
            /* Encode an indirect register. */
            encodeMetadata(word, 'A');
            encodeRegister(word, convertDigitToNumber(operand[THIRD_INDEX]),
                           isSource);
            break;
        case IMMEDIATE:
            /* Encode an immediate value. */
            encodeMetadata(word, 'A');
            encodeImmediate(word, (short)atoi(&operand[SECOND_INDEX]));
            break;
        default:
            /* Direct operands (labels) will be encoded later. */
            break;
    }
}

void encodeOperand(Word *word, char operand[], Boolean isSource) {
    /* Encode the operand differently if it is the source or the destination. */
    if (isSource) {
        /* Encode the addressing mode of a source operand. */
        toggleBit(word, (Position)getOperandType(operand) +
                            STARTING_SOURCE_OPERAND_BIT);
        return;
    }

    /* Encode the addressing mode of a destination operand. */
    toggleBit(word, (Position)getOperandType(operand) +
                        STARTING_DESTINATION_OPERAND_BIT);
}

void encodeRegister(Word *word, unsigned char registerNumber,
                    Boolean isSource) {
    /* Encode the register's number into the given word. */
    applyMask(word, (Mask)registerNumber,
              isSource ? STARTING_SOURCE_REGISTER_BIT
                       : STARTING_DESTINATION_REGISTER_BIT);
}

void encodeOperation(Word *word, char operation[]) {
    /* Encode the operation's unique identifier number into the given word. */
    applyMask(word, (Mask)getOperationIndex(operation), STARTING_OPERATION_BIT);
}

void encodeImmediate(Word *word, short immediate) {
    /* Encode the immediate value's 2's complement representation. */
    applyMask(word, (Mask)immediate, STARTING_IMMEDIATE_BIT);
}

void encodeData(Word *word, short data) {
    /* Encode the data value's ASCII value or 2's complement representation. */
    applyMask(word, (Mask)data, STARTING_DATA_BIT);
}

void encodeLabel(Word *word, Address address) {
    /* Encode the label's address into the given word. */
    applyMask(word, (Mask)address, STARTING_LABEL_BIT);
}
