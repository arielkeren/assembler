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

#include "globals.h" /* Typedefs. */

/**
 * Adds a new word to the given word list.
 * Returns a pointer to the new word.
 * IMPORTANT: The caller must free the new word.
 *
 * Assumes that the given word pointer is not NULL.
 *
 * @param words The word list to add the new word to.
 * @return A pointer to the new word.
 */
Word *addWord(Word *wordList);

/**
 * Creates a new word and returns a pointer to it.
 * Initializes the fields of the word to zeros.
 * IMPORTANT: The caller must free the new word.
 *
 * @return A pointer to the new word.
 */
Word *createWord();

/**
 * Frees a word list.
 *
 * Assumes that the given words have not been freed yet.
 *
 * @param words The word list to free.
 */
void freeWordList(Word *wordList);

#endif
