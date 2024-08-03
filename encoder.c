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
    if (bitPosition < (sizeof(word->data1) * BITS_PER_BYTE)) {
        word->data1 |= (SINGLE_BIT << bitPosition);
        return;
    }

    word->data2 |= (SINGLE_BIT << (bitPosition - (sizeof(word->data1) * BITS_PER_BYTE)));
}

/**
 * Applies the given mask to given word from the given position (continuing to the MSB).
 *
 * Assumes that the given word is not NULL.
 * Assumes that the given position is between 0 and 14, inclusive.
 *
 * @param word The word to apply the mask to.
 * @param mask The mask to apply.
 * @param from The position to start applying the mask from (continuing to the MSB).
 */
void applyMask(Word *word, Mask mask, Position from) {
    while (mask != EMPTY && from < BITS_PER_MEMORY_CELL) {
        if (mask & SINGLE_BIT) {
            toggleBit(word, from);
        }

        mask >>= SINGLE_BIT;
        from++;
    }
}

/**
 * Encodes the given string into words, that will be added in order to the data word list.
 * Increments the data count by the number of words that have been added.
 * Adds the ASCII values of the characters of the string to separate words.
 * Finally, adds another word with all zeros to represent the null terminator of the string.
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

/**
 * Encodes the given number list string into words, that will be added in order to the data word list.
 * Increments the data count by the number of words that have been added.
 * Adds the 2's complement binary representation of the numbers in the number list to separate words.
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

/**
 * Encodes the given addressing mode into the given word.
 * There are 3 options:
 * - E: toggles the bit at position 0, representing extern labels.
 * - R: toggles the bit at position 1, representing labels that are defined in the current file.
 * - A: toggles the bit at position 2, representing everything else that needs metadata.
 *
 * Assumes that the given word is not NULL.
 * Assumes that the given metadata is one of E, R or A.
 *
 * @param word The word to encode the addressing mode in.
 * @param metadata The addressing mode to encode - can be either E, R or A.
 */
void encodeAddressingMode(Word *word, char metadata) {
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

/**
 * Encodes the given operand into the given word, representing the word that follows the first word of the instruction.
 * Encodes according to the type of the given operand and according to whether it is the source or the destination.
 * Encodes the addressing mode A if the operand is not of type direct (potential labels).
 * Does not encode anything if the operand is of type direct.
 *
 * Assumes that the given word is not NULL.
 * Assumes that the given operand string is not NULL and is null-terminated.
 *
 * @param word The word to encode the operand in.
 * @param operand The operand to encode.
 * @param isSource Whether the operand is the source or the destination of the instruction.
 */
void encodeExtraWord(Word *word, char operand[], Boolean isSource) {
    switch (getOperandType(operand)) {
        case DIRECT_REGISTER:
            encodeAddressingMode(word, 'A');
            encodeRegister(word, convertDigitToNumber(operand[SECOND_INDEX]), isSource);
            break;
        case INDIRECT_REGISTER:
            encodeAddressingMode(word, 'A');
            encodeRegister(word, convertDigitToNumber(operand[THIRD_INDEX]), isSource);
            break;
        case IMMEDIATE:
            encodeAddressingMode(word, 'A');
            encodeImmediate(word, (short)atoi(&operand[SECOND_INDEX]));
            break;
        default:
            break;
    }
}

/**
 * Encodes the given operand into the given instruction word.
 * Encodes according to the type of the given operand and according to whether it is the source or the destination.
 *
 * Assumes that the given word is not NULL.
 * Assumes that the given operand string is not NULL and is null-terminated.
 *
 * @param word The word to encode the operand in.
 * @param operand The operand to encode.
 * @param isSource Whether the operand is the source or the destination of the instruction.
 */
void encodeOperand(Word *word, char operand[], Boolean isSource) {
    if (isSource) {
        toggleBit(word, (Position)getOperandType(operand) + STARTING_SOURCE_OPERAND_BIT);
        return;
    }

    toggleBit(word, (Position)getOperandType(operand) + STARTING_DESTINATION_OPERAND_BIT);
}

/**
 * Encodes the given register number into the given word.
 * Encodes according to whether the given register is the source or the destination.
 *
 * Assumes that the given word is not NULL.
 * Assumes that the given register number is between 0 and 7, inclusive.
 *
 * @param word The word to encode the register in.
 * @param registerNumber The register number to encode (0 to 7, inclusive).
 * @param isSource Whether the register is the source or the destination.
 */
void encodeRegister(Word *word, unsigned char registerNumber, Boolean isSource) {
    applyMask(word, (Mask)registerNumber, isSource ? STARTING_SOURCE_REGISTER_BIT : STARTING_DESTINATION_REGISTER_BIT);
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
    applyMask(word, (Mask)immediate, STARTING_IMMEDIATE_BIT);
}

/**
 * Encodes the given data value into the given word.
 * If the data value represents a number (-2048 to 2047, inclusive), encodes the 2's complement binary representation.
 * If the data value represents a character, encodes the ASCII value of the character.
 *
 * Assumes that the given word is not NULL.
 * Assumes that the given data value is between -2048 and 2047, inclusive, or is a character value.
 *
 * @param word The word to encode the data value in.
 * @param data The data value to encode.
 */
void encodeData(Word *word, short data) {
    applyMask(word, (Mask)data, STARTING_DATA_BIT);
}

/**
 * Encodes the given label address into the given word.
 *
 * Assumes that the given word is not NULL.
 * Assumes that the given address is between 100 and 4095, inclusive (to fit in the made-up memory).
 *
 * @param word The word to encode the label address in.
 * @param address The label address to encode.
 */
void encodeLabel(Word *word, Address address) {
    applyMask(word, (Mask)address, STARTING_LABEL_BIT);
}
