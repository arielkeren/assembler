/*
 * macroTable.c
 *
 * Contains functions to interface with a macro table, to search through it and
 * finally to free it.
 * The macro table contains all the macro definitions in the .as file, along
 * with their contents.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "macroTable.h"

#include <stdlib.h> /* free. */
#include <string.h> /* strcmp, strlen, strcpy, strcat. */

#include "globals.h" /* Constants and typedefs. */
#include "utils.h"   /* Allocating memory. */

/**
 * Adds a new macro to the given macro table with the given name.
 * IMPORTANT: The caller must free the new macro.
 *
 * Assumes that the given pointer to the macros is not NULL.
 * Assumes that the given macro name is not NULL and is null-terminated.
 *
 * @param macros The macro table to add the macro to.
 * @param macroName The name of the new macro.
 */
void addMacro(Macro **macros, char macroName[]) {
    Macro *newMacro; /* The new macro. */

    /* Allocate enough memory for the new macro. */
    newMacro = allocate(sizeof(Macro));

    /* Set the new macro's properties. */
    newMacro->name = macroName;
    newMacro->content = "";

    /* Add the new macro to the head of the list. */
    newMacro->next = *macros;
    *macros = newMacro;
}

/**
 * Appends the given content to the given macro's content.
 *
 * Assumes that the given macro pointer is not NULL.
 * Assumes that the given content is not NULL and is null-terminated.
 *
 * @param macro The macro to add the content to.
 * @param content The content to add.
 */
void addMacroContent(Macro *macro, char content[]) {
    char *newContent; /* The new combined content. */

    /* Allocate enough memory for the new combined content. */
    newContent = allocate(
        sizeof(char) * (strlen(macro->content) + strlen(content) + NULL_BYTE));

    /* Copy the old content. */
    strcpy(newContent, macro->content);
    /* Free the old content. */
    free(macro->content);
    /* Concatenate the given content. */
    strcat(newContent, content);

    /* Replace the old content with the new content. */
    macro->content = newContent;
}

/**
 * Searches for the given macro name in the given macro table.
 * If found, returns its content.
 * If not found, returns NULL.
 *
 * Assumes that the given macro name is not NULL and is null-terminated.
 *
 * @param macros The macro table.
 * @param macroName The macro name to search for.
 * @return The macro's content, or NULL if not found.
 */
char *getMacroContent(Macro *macros, char macroName[]) {
    /* Loop over the macros in the list and find the one with the given name. */
    while (macros != NULL) {
        /* Compare the macro names. */
        if (strcmp(macros->name, macroName) == EQUAL_STRINGS) {
            /* Return the desired content. */
            return macros->content;
        }

        /* Move on to the next macro. */
        macros = macros->next;
    }

    /* Not found. */
    return NULL;
}

/**
 * Frees a macro table.
 *
 * Assumes that the given macro table has not been freed yet.
 *
 * @param macros The macro table to free.
 */
void freeMacroTable(Macro *macros) {
    Macro *next; /* The next macro in the list. */

    /* Loop over the macros in the list and free them. */
    while (macros != NULL) {
        /* Store the next macro. */
        next = macros->next;
        /* Free the current macro. */
        freeMacro(macros);
        /* Move on to the next macro. */
        macros = next;
    }
}

/**
 * Frees a single macro.
 *
 * Assumes that the given macro is not NULL and has not been freed yet.
 *
 * @param macro The macro to free.
 */
void freeMacro(Macro *macro) {
    /* Free the name string. */
    free(macro->name);
    /* Free the content string. */
    free(macro->content);
    /* Free the macro. */
    free(macro);
}
