#include "wordList.h"

#include <stdlib.h>

#include "globals.h"
#include "utils.h"

Word *addWord(Word *words) {
    Word *newWord;

    newWord = createWord();
    words->next = newWord;

    return newWord;
}

Word *createWord() {
    Word *newWord;

    newWord = allocate(sizeof(Word));

    newWord->data1 = (unsigned char)INITIAL_VALUE;
    newWord->data2 = (unsigned char)INITIAL_VALUE;
    newWord->next = (Word *)NULL;

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
