#ifndef WORD_LIST_H
#define WORD_LIST_H

#include "globals.h"

Word *addWord(Word *wordList);
Word *createWord();
void freeWordList(Word *wordList);

#endif
