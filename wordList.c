#include "wordList.h"

#include <stdlib.h>

#include "globals.h"
#include "utils.h"

word *addWord(word *wordList) {
    word *newWord;

    newWord = createWord();
    wordList->next = newWord;

    return newWord;
}

word *createWord() {
    word *newWord;

    newWord = allocate(sizeof(word));

    newWord->data1 = INITIAL_VALUE;
    newWord->data2 = INITIAL_VALUE;
    newWord->next = NULL;

    return newWord;
}

void freeWordList(word *wordList) {
    word *next;

    while (wordList != NULL) {
        next = wordList->next;
        free(wordList);
        wordList = next;
    }
}
