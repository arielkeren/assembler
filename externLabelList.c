#include "externLabelList.h"

#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "utils.h"

void addExternLabel(externLabel **labelList, char labelName[]) {
    externLabel *newLabel;

    newLabel = allocate(sizeof(externLabel));
    newLabel->name = labelName;
    newLabel->useList = NULL;
    newLabel->next = *labelList;
    *labelList = newLabel;
}

void addUse(externLabel *labelToModify, unsigned address) {
    use *newUse;

    newUse = allocate(sizeof(use));
    newUse->address = address;
    newUse->next = labelToModify->useList;
    labelToModify->useList = newUse;
}

boolean containsExternLabel(externLabel *labelList, char labelName[]) {
    while (labelList != NULL) {
        if (strcmp(labelList->name, labelName) == 0) {
            return TRUE;
        }

        labelList = labelList->next;
    }

    return FALSE;
}

void freeExternLabelList(externLabel *labelList) {
    externLabel *next;

    while (labelList != NULL) {
        next = labelList->next;
        freeExternLabel(labelList);
        labelList = next;
    }
}

void freeExternLabel(externLabel *labelToFree) {
    free(labelToFree->name);
    freeUseList(labelToFree->useList);
    free(labelToFree);
}

void freeUseList(use *useList) {
    use *next;

    while (useList != NULL) {
        next = useList->next;
        free(useList);
        useList = next;
    }
}
