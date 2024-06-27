#include "wordList.h"

#include <stdlib.h>

#include "globals.h"
#include "utils.h"

void addWord(word **wordList) {
    word *newWord;

    newWord = allocate(sizeof(word));

    newWord->data1 = 0;
    newWord->data2 = 0;
    newWord->next = *wordList;
    *wordList = newWord;
}

void freeWordList(word *wordList) {
    word *next;

    while (wordList != NULL) {
        next = wordList->next;
        free(wordList);
        wordList = next;
    }
}
