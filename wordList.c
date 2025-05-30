/*
 * wordList.c
 *
 * Contains functions to interface with a word list and to free it.
 * Also, contains a function to create a new word, which can be used as a dummy
 * node.
 * The 2 word lists contain all the words in the code part and in the data part
 * of the .am file.
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
    Word *newWord; /* The new word. */

    /* Create a new word. */
    newWord = createWord();
    /* Add the new word to the list. */
    words->next = newWord;

    return newWord;
}

Word *createWord() {
    Word *newWord; /* The new word. */

    /* Allocate enough memory for the new word. */
    newWord = allocate(sizeof(Word));

    /* Initialize the fields of the new word. */
    newWord->data1 = INITIAL_VALUE;
    newWord->data2 = INITIAL_VALUE;
    newWord->next = NULL;

    return newWord;
}

void freeWordList(Word *words) {
    Word *next; /* The next word in the list. */

    /* Loop over each word in the list and free it. */
    while (words != NULL) {
        /* Store the next word. */
        next = words->next;
        /* Free the current word. */
        free(words);
        /* Move on to the next word. */
        words = next;
    }
}
