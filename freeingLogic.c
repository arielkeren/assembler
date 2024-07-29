#include "freeingLogic.h"

#include "foundLabelList.h"
#include "globals.h"
#include "labelList.h"
#include "macroTable.h"
#include "usedLabelList.h"
#include "wordList.h"

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
