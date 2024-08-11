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

/**
 * Toggles the bit at the given position in the given word.
 *
 * Assumes that the given word is not NULL.
 * Assumes that the given position is between 0 and 14, inclusive.
 *
 * @param word The word to toggle the bit in.
 * @param bitPosition The position of the bit to toggle (0 is the LSB).
 */
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

/**
 * Applies the given mask to given word from the given position (continuing to
 * the MSB).
 *
 * Assumes that the given word is not NULL.
 * Assumes that the given position is between 0 and 14, inclusive.
 *
 * @param word The word to apply the mask to.
 * @param mask The mask to apply.
 * @param from The position to start applying the mask from (continuing to the
 * MSB).
 */
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

/**
 * Encodes the given string into words, that will be added in order to the data
 * word list.
 * Increments the data count by the number of words that have been added.
 * Adds the ASCII values of the characters of the string to separate words.
 * Finally, adds another word with all zeros to represent the null terminator of
 * the string.
 *
 * Assumes that the given data is not NULL.
 * Assumes that the given string is not NULL and is null-terminated.
 * Assumes that the given pointer to the data count is not NULL.
 *
 * @param data The data word list to add the words to.
 * @param string The string to encode.
 * @param dataCount The data count to increment.
 */
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

/**
 * Encodes the given number list string into words, that will be added in order
 * to the data word list.
 * Increments the data count by the number of words that have been added.
 * Adds the 2's complement binary representation of the numbers in the number
 * list to separate words.
 *
 * Assumes that the given data is not NULL.
 * Assumes that the given number list string is not NULL and is null-terminated.
 * Assumes that the given pointer to the data count is not NULL.
 *
 * @param data The data word list to add the words to.
 * @param numberList The number list string, separated by commas, to encode.
 * @param dataCount The data count to increment.
 */
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

/**
 * Encodes the given addressing mode into the given word.
 * There are 3 options:
 * - E: toggles the bit at position 0, representing extern labels.
 * - R: toggles the bit at position 1, representing labels that are defined in
 * the current file.
 * - A: toggles the bit at position 2, representing everything else that needs
 * metadata.
 *
 * Assumes that the given word is not NULL.
 * Assumes that the given metadata is one of E, R or A.
 *
 * @param word The word to encode the addressing mode in.
 * @param metadata The addressing mode to encode - can be either E, R or A.
 */
void encodeAddressingMode(Word *word, char metadata) {
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

/**
 * Encodes the given operand into the given word, representing the word that
 * follows the first word of the instruction.
 * Encodes according to the type of the given operand and according to whether
 * it is the source or the destination.
 * Encodes the addressing mode A if the operand is not of type direct (potential
 * labels).
 * Does not encode anything if the operand is of type direct.
 *
 * Assumes that the given word is not NULL.
 * Assumes that the given operand string is not NULL and is null-terminated.
 *
 * @param word The word to encode the operand in.
 * @param operand The operand to encode.
 * @param isSource Whether the operand is the source or the destination of the
 * instruction.
 */
void encodeExtraWord(Word *word, char operand[], Boolean isSource) {
    /* Determine the addressing mode of the operand before encoding it. */
    switch (getOperandType(operand)) {
        case DIRECT_REGISTER:
            /* Encode a direct register. */
            encodeAddressingMode(word, 'A');
            encodeRegister(word, convertDigitToNumber(operand[SECOND_INDEX]),
                           isSource);
            break;
        case INDIRECT_REGISTER:
            /* Encode an indirect register. */
            encodeAddressingMode(word, 'A');
            encodeRegister(word, convertDigitToNumber(operand[THIRD_INDEX]),
                           isSource);
            break;
        case IMMEDIATE:
            /* Encode an immediate value. */
            encodeAddressingMode(word, 'A');
            encodeImmediate(word, (short)atoi(&operand[SECOND_INDEX]));
            break;
        default:
            /* Direct operands (labels) will be encoded later. */
            break;
    }
}

/**
 * Encodes the given operand into the given instruction word.
 * Encodes according to the type of the given operand and according to whether
 * it is the source or the destination.
 *
 * Assumes that the given word is not NULL.
 * Assumes that the given operand string is not NULL and is null-terminated.
 *
 * @param word The word to encode the operand in.
 * @param operand The operand to encode.
 * @param isSource Whether the operand is the source or the destination of the
 * instruction.
 */
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

/**
 * Encodes the given register number into the given word.
 * Encodes according to whether the given register is the source or the
 * destination.
 *
 * Assumes that the given word is not NULL.
 * Assumes that the given register number is between 0 and 7, inclusive.
 *
 * @param word The word to encode the register in.
 * @param registerNumber The register number to encode (0 to 7, inclusive).
 * @param isSource Whether the register is the source or the destination.
 */
void encodeRegister(Word *word, unsigned char registerNumber,
                    Boolean isSource) {
    /* Encode the register's number into the given word. */
    applyMask(word, (Mask)registerNumber,
              isSource ? STARTING_SOURCE_REGISTER_BIT
                       : STARTING_DESTINATION_REGISTER_BIT);
}

/**
 * Encodes the given operation into the given word.
 * Encodes the number of the operation.
 *
 * Assumes that the given word is not NULL.
 * Assumes that the given operation string is not NULL and is null-terminated.
 *
 * @param word The word to encode the operation in.
 * @param operation The operation to encode.
 */
void encodeOperation(Word *word, char operation[]) {
    /* Encode the operation's unique identifier number into the given word. */
    applyMask(word, (Mask)getOperationIndex(operation), STARTING_OPERATION_BIT);
}

/**
 * Encodes the given immediate value into the given word.
 * Encodes the 2's complement binary representation of the immediate value.
 *
 * Assumes that the given word is not NULL.
 * Assumes that the given immediate value is between -2048 and 2047, inclusive.
 *
 * @param word The word to encode the immediate value in.
 * @param immediate The immediate value to encode (-2048 to 2047, inclusive).
 */
void encodeImmediate(Word *word, short immediate) {
    /* Encode the immediate value's 2's complement representation. */
    applyMask(word, (Mask)immediate, STARTING_IMMEDIATE_BIT);
}

/**
 * Encodes the given data value into the given word.
 * If the data value represents a number (-2048 to 2047, inclusive), encodes the
 * 2's complement binary representation.
 * If the data value represents a character, encodes the ASCII value of the
 * character.
 *
 * Assumes that the given word is not NULL.
 * Assumes that the given data value is between -2048 and 2047, inclusive, or is
 * a character value.
 *
 * @param word The word to encode the data value in.
 * @param data The data value to encode.
 */
void encodeData(Word *word, short data) {
    /* Encode the data value's ASCII value or 2's complement representation. */
    applyMask(word, (Mask)data, STARTING_DATA_BIT);
}

/**
 * Encodes the given label address into the given word.
 *
 * Assumes that the given word is not NULL.
 * Assumes that the given address is between 100 and 4095, inclusive (to fit in
 * the made-up memory).
 *
 * @param word The word to encode the label address in.
 * @param address The label address to encode.
 */
void encodeLabel(Word *word, Address address) {
    /* Encode the label's address into the given word. */
    applyMask(word, (Mask)address, STARTING_LABEL_BIT);
}
