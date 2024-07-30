/*
 * wordList.c
 *
 * Contains functions to interface with a word list and to free it.
 * Also, contains a function to create a new word, which can be used as a dummy node.
 * The 2 word lists contain all the words in the code part and in the data part of the .am file.
 * They will eventually be used to generate the .ob file.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "wordList.h"

#include <stdlib.h> /* free. */

#include "globals.h" /* Constants and typedefs. */
#include "utils.h"   /* Allocating memory. */

Word *addWord(Word *words) {
    Word *newWord;

    newWord = createWord();
    words->next = newWord;

    return newWord;
}

Word *createWord() {
    Word *newWord;

    newWord = allocate(sizeof(Word));

    newWord->data1 = INITIAL_VALUE;
    newWord->data2 = INITIAL_VALUE;
    newWord->next = NULL;

    return newWord;
}

void freeWordList(Word *words) {
    Word *next;

    while (words != NULL) {
        next = words->next;
        free(words);
        words = next;
    }
}
