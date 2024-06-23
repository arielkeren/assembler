#ifndef GLOBALS_H
#define GLOBALS_H

typedef struct macroStruct {
    char *name;
    char *content;
    struct macroStruct *next;
} macro;

typedef struct wordStruct {
    unsigned char data1;
    unsigned char data2;
    struct wordStruct *next;
} word;

typedef struct labelStruct {
    char *name;
    unsigned address;
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
