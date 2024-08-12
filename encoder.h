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

/**
 * Toggles the bit at the given position in the given word.
 *
 * Assumes that the given word is not NULL.
 * Assumes that the given position is between 0 and 14, inclusive.
 *
 * @param word The word to toggle the bit in.
 * @param bitPosition The position of the bit to toggle (0 is the LSB).
 */
void toggleBit(Word *word, Position bitPosition);

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
void applyMask(Word *word, Mask mask, Position from);

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
void encodeString(Word **data, char string[], WordCount *dataCount);

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
void encodeNumberList(Word **data, char numberList[], WordCount *dataCount);

/**
 * Encodes the given metadata character into the given word.
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
 * @param word The word to encode the metadata in.
 * @param metadata The metadata to encode - can be either E, R or A.
 */
void encodeMetadata(Word *word, char metadata);

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
void encodeExtraWord(Word *word, char operand[], Boolean isSource);

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
void encodeOperand(Word *word, char operand[], Boolean isSource);

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
void encodeRegister(Word *word, unsigned char registerNumber, Boolean isSource);

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
void encodeOperation(Word *word, char operation[]);

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
void encodeImmediate(Word *word, short number);

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
void encodeData(Word *word, short data);

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
void encodeLabel(Word *word, Address address);

#endif
