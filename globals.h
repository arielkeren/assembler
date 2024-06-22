#ifndef GLOBALS_H
#define GLOBALS_H

typedef struct macroStruct {
    char *name;
    char *content;
    struct macroStruct *next;
} macro;

typedef struct labelStruct {
    char *name;
    struct labelStruct *next;
} label;

typedef struct usedLabelStruct {
    char *name;
    word *wordPointer;
    struct usedLabelStruct *next;
} usedLabel;

extern const char *OPERATIONS[16];

typedef enum { FALSE,
               TRUE } boolean;
typedef enum { IMMEDIATE,
               DIRECT,
               INDIRECT_REGISTER,
               DIRECT_REGISTER } operandType;

#endif
