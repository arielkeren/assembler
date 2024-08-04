/*
 * fileReading.h
 *
 * Contains the function prototypes for the functions in fileReading.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef FILE_READING_H
#define FILE_READING_H

#include <stdio.h> /* FILE. */

#include "globals.h" /* Typedefs. */

/**
 * Opens the .am file and starts reading it line by line.
 * Modifies the given arguments to contain the information read from the .am
 * file.
 * Returns whether or not no errors occurred.
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that all the pointers given as arguments, except macros, are not
 * NULL.
 *
 * @param fileName The name of the file to read.
 * @param macros The macro table.
 * @param code The code word list.
 * @param data The data word list.
 * @param entryLabels The list of entry labels.
 * @param externLabels The list of extern labels.
 * @param usedLabels The list of used labels.
 * @param foundLabels The list of found labels.
 * @param instructionCount The current instruction count to modify.
 * @param dataCount The current data count to modify.
 * @return TRUE if no errors occurred, FALSE otherwise or if the file could not
 * be opened.
 */
Boolean readFile(char fileName[], Macro *macros, Word *code, Word *data,
                 Label **entryLabels, Label **externLabels,
                 UsedLabel **usedLabels, FoundLabel **foundLabels,
                 WordCount *instructionCount, WordCount *dataCount);

/**
 * Reads the lines of the .am file line by line.
 * Modifies the given arguments to contain the information read from the .am
 * file.
 * Returns whether or not no errors occurred.
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that all the pointers given as arguments, except macros, are not
 * NULL.
 *
 * @param fileName The name of the file to read.
 * @param file The file to read.
 * @param macros The macro table.
 * @param code The code word list.
 * @param data The data word list.
 * @param entryLabels The list of entry labels.
 * @param externLabels The list of extern labels.
 * @param usedLabels The list of used labels.
 * @param foundLabels The list of found labels.
 * @param instructionCount The current instruction count to modify.
 * @param dataCount The current data count to modify.
 * @return TRUE if no errors occurred, FALSE otherwise.
 */
Boolean readLines(char fileName[], FILE *file, Macro *macros, Word *code,
                  Word *data, Label **entryLabels, Label **externLabels,
                  UsedLabel **usedLabels, FoundLabel **foundLabels,
                  WordCount *instructionCount, WordCount *dataCount);

/**
 * Handles a line of the .am file.
 * Modifies the given arguments to contain the information read from the current
 * line.
 * Returns whether or not no errors occurred.
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that the given line is not NULL and is null-terminated.
 * Assumes that all the pointers given as arguments, except macros, are not
 * NULL.
 *
 * @param fileName The name of the file to read.
 * @param line The line that has been extracted from the .am file.
 * @param lineNumber The current line's line number.
 * @param macros The macro table.
 * @param code The code word list.
 * @param data The data word list.
 * @param entryLabels The list of entry labels.
 * @param externLabels The list of extern labels.
 * @param usedLabels The list of used labels.
 * @param foundLabels The list of found labels.
 * @param instructionCount The current instruction count to modify.
 * @param dataCount The current data count to modify.
 * @return TRUE if no errors occurred, FALSE otherwise.
 */
Boolean handleLine(char fileName[], char line[], LineNumber lineNumber,
                   Macro *macros, Word **code, Word **data, Label **entryLabels,
                   Label **externLabels, UsedLabel **usedLabels,
                   FoundLabel **foundLabels, WordCount *instructionCount,
                   WordCount *dataCount);
Boolean handleLabel(char fileName[], char line[], LineNumber lineNumber,
                    Macro *macros, FoundLabel **foundLabels,
                    WordCount instructionCount, WordCount dataCount);

/**
 * Handles a potential label in the current line of the .am file.
 * Modifies the given arguments to contain the new information.
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that the given line is not NULL and is null-terminated.
 * Assumes that the given pointer to the found labels is not NULL.
 *
 * @param fileName The name of the file to read.
 * @param line The current line.
 * @param lineNumber The current line's line number.
 * @param macros The macro table.
 * @param foundLabels The list of found labels.
 * @param instructionCount The current instruction count.
 * @param dataCount The current data count.
 * @return TRUE if no errors occurred, FALSE otherwise.
 */
void handleOperation(char line[], LineNumber lineNumber, Word **code,
                     UsedLabel **usedLabels, WordCount *instructionCount);

#endif
