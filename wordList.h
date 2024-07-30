/*
 * wordList.h
 *
 * Contains the function prototypes for the functions in wordList.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef WORD_LIST_H
#define WORD_LIST_H

#include "globals.h"

Word *addWord(Word *wordList);
Word *createWord();
void freeWordList(Word *wordList);

#endif
