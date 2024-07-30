/*
 * freeingLogic.c
 *
 * Contains functions to handle sudden freeing of the memory used by the program.
 * Only executes when an allocation failure occurs.
 * Needs to get the pointers to each linked list in memory to free it, in case of an allocation failure.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "freeingLogic.h"

#include "foundLabelList.h" /* Freeing the found label list. */
#include "globals.h"        /* Constants and typedefs. */
#include "labelList.h"      /* Freeing the 2 label lists. */
#include "macroTable.h"     /* Freeing the macro table. */
#include "usedLabelList.h"  /* Freeing the used label list. */
#include "wordList.h"       /* Freeing the 2 word lists. */

void freeAll() {
    freeingLogic(TRUE, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void setToFree(Word **code, Word **data, Macro **macros, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels) {
    freeingLogic(FALSE, code, data, macros, entryLabels, externLabels, usedLabels, foundLabels);
}

void freeingLogic(Boolean shouldFree, Word **code, Word **data, Macro **macros, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels) {
    static ToFree toFree;

    if (shouldFree) {
        freeLists(&toFree);
        return;
    }

    toFree.code = code;
    toFree.data = data;
    toFree.macros = macros;
    toFree.entryLabels = entryLabels;
    toFree.externLabels = externLabels;
    toFree.usedLabels = usedLabels;
    toFree.foundLabels = foundLabels;
}

void freeLists(ToFree *toFree) {
    freeWordList(*toFree->code);
    freeWordList(*toFree->data);
    freeMacroTable(*toFree->macros);
    freeLabelList(*toFree->entryLabels);
    freeLabelList(*toFree->externLabels);
    freeUsedLabelList(*toFree->usedLabels);
    freeFoundLabelList(*toFree->foundLabels);
}
