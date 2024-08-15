/*
 * fileGeneration.h
 *
 * Contains the function prototypes for the functions in fileGeneration.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef FILE_GENERATION_H
#define FILE_GENERATION_H

#include <stdio.h> /* FILE. */

#include "globals.h" /* Typedefs. */

/**
 * Generates the <fileName>.ob file in the following format:
 * Addresses are decimal and padded to be 4 digits long with zeros to the left.
 * Words are octal and padded to be 5 digits long with zeros to the left.
 * NOTE: The .ob file does not include a title before each part.
 *
 * <Instruction count> <Data count>
 * --- Code Part --- (Taken from the code list)
 * <Address> <Word>
 * <Address> <Word>
 * ...
 * --- Data Part --- (Taken from the data list)
 * <Address> <Word>
 * <Address> <Word>
 * ...
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param fileName The name of the .ob file to generate.
 * @param code List that represents the words in the code part.
 * @param data List that represents the words in the data part.
 * @param instructionCount The number of words in the code part.
 * @param dataCount The number of words in the data part.
 */
void generateObFile(char fileName[], Word *code, Word *data,
                    WordCount instructionCount, WordCount dataCount);

/**
 * Returns whether or not more files should be generated after this one (an
 * error occured).
 *
 * The labels are padded to the number of characters of the longest label with
 * spaces to the right.
 *
 * The addresses are decimal and padded to be 4 digits long with zeros to the
 * left.
 *
 * Generates the <fileName>.ent file in the following format:
 * <Label> <Address>
 * <Label> <Address>
 * ...
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param fileName The name of the .ent file to generate.
 * @param entryLabels The list of labels marked as entry.
 * @param foundLabels The list of found labels to check for missing definitions.
 * @param instructionCount The number of words in the code part.
 * @param shouldGenerate Whether the .ent file should be generated. If not, only
 * checks for missing definitions.
 * @return Whether or not more files should be generated after the .ent file.
 */
Boolean generateEntFile(char fileName[], Label *entryLabels,
                        FoundLabel *foundLabels, WordCount instructionCount,
                        Boolean shouldGenerate);

/**
 * Returns whether or not more files should be generated after this one (an
 * error occured).
 * The labels are padded to the number of characters of the longest label with
 * spaces to the right.
 * The addresses are decimal and padded to be 4 digits long with zeros to the
 * left.
 * Generates the <fileName>.ext file in the following format:
 * <Label> <Address>
 * <Label> <Address>
 * ...
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param fileName The name of the .ext file to generate.
 * @param externLabels The list of labels marked as extern.
 * @param usedLabels The list of used labels to check for every instance of the
 * labels.
 * @param foundLabels The list of found labels to check for labels that are
 * extern and also defined (invalid).
 * @param shouldGenerate Whether the .ext file should be generated. If not, only
 * checks for errors.
 * @return Whether or not more files should be generated after the .ext file.
 */
Boolean generateExtFile(char fileName[], Label *externLabels,
                        UsedLabel *usedLabels, FoundLabel *foundLabels,
                        Boolean shouldGenerate);

/**
 * Inserts the given list of words into the given file.
 * The address of the first word will be the given starting address.
 * The following addresses will be automatically incremented by one each
 * time.
 *
 * Assumes that the given file pointer is not NULL.
 * Assumes that the starting address is valid, so that all the words will
 * fit in the made-up memory until address 4095.
 *
 * @param file The file to insert the words into.
 * @param words The list of words to insert.
 * @param startingAddress The address of the first word in the list.
 */
void insertWords(FILE *file, Word *words, Address startingAddress);

/**
 * Inserts all the uses of the given extern label into the given file.
 *
 * Assumes that the given pointer to the file pointer is not NULL.
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that the given label name string is not NULL and is null-terminated.
 * Assumes that the given isFirst boolean pointer is not NULL.
 *
 * @param file The file to insert the uses into.
 * @param fileName The name of the file to insert the uses into.
 * @param externLabel The extern label to insert.
 * @param usedLabels The list of used labels to scan.
 * @param longest The number of characters in the longest label.
 * @param isFirst Whether this is the first label inserted.
 */
Boolean insertUses(FILE **file, char fileName[], Label *externLabel,
                   UsedLabel *usedLabels, Length longest, Boolean *isFirst);

/**
 * Inserts the given label definition into the given file.
 *
 * Assumes that the given file pointer is not NULL.
 * Assumes that the given label name string is not NULL and is null-terminated.
 * Assumes that the given found label is not NULL.
 *
 * @param file The file to insert the label definition into.
 * @param labelName The name of the label to insert.
 * @param definition The found label that defines the target label.
 * @param instructionCount The final instruction count.
 * @param longest The number of characters in the longest label.
 * @param isFirst Whether this is the first label inserted.
 */
void insertLabelDefinition(FILE *file, char labelName[], FoundLabel *definition,
                           WordCount instructionCount, Length longest,
                           Boolean isFirst);

/**
 * Inserts the given label into the given file, along with an address.
 * Pads the label with spaces to the right using the longest label's length, so
 * that all the labels line up properly.
 * Pads the address with zeros to the left, so that every address is 4 digits
 * long.
 *
 * Assumes that the given file pointer is not NULL.
 * Assumes that the given label name string is not NULL and is
 * null-terminated. Assumes that the given address is between 100 and 4095,
 * inclusive (to fit in the made-up memory). Assumes that the given longest
 * label's length is the number of characters in the longest label.
 *
 * @param file The file to insert the label into.
 * @param labelName The name of the label to insert.
 * @param address The address associated with the label.
 * @param longest The number of characters in the longest label.
 * @param isFirst Whether this is the first label to be inserted into some
 * file.
 */
void insertLabel(FILE *file, char labelName[], Address address, Length longest,
                 Boolean isFirst);

/**
 * Computes and returns the length of the longest label in the given label list.
 * If the labels are extern, the calculation does not include the extern labels
 * that are unused.
 * Uses the given list of used labels to check for unused extern labels.
 * If an extern label is unused, prints a warning to stdout.
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 *
 * @param fileName The name of the current file (for warnings).
 * @param labels The label list to compute the length of the longest label in.
 * @param isExtern Whether the labels are extern or not (entry).
 * @param usedLabels The list of used labels to use to check for unused extern
 * labels.
 * @return The length of the longest label in the given label list.
 */
Length getLongestLabel(char fileName[], Label *labels, Boolean isExtern,
                       UsedLabel *usedLabels);

#endif
