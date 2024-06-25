#ifndef GLOBALS_H
#define GLOBALS_H

typedef enum { FALSE,
               TRUE } boolean;
typedef enum { IMMEDIATE,
               DIRECT,
               INDIRECT_REGISTER,
               DIRECT_REGISTER } operandType;

typedef struct useStruct {
    unsigned address;
    struct useStruct *next;
} use;

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

typedef struct externLabelStruct {
    char *name;
    use *useList;
    struct externLabelStruct *next;
} externLabel;

typedef struct usedLabelStruct {
    char *name;
    unsigned address;
    word *wordPointer;
    struct usedLabelStruct *next;
} usedLabel;

typedef struct foundLabelStruct {
    char *name;
    unsigned address;
    boolean isData;
    struct foundLabelStruct *next;
} foundLabel;

extern const char *OPERATIONS[16];

#endif
