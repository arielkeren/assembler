/*
 * globals.h
 *
 * Contains all the typedefs, structs, enums and constants used in the program.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef GLOBALS_H
#define GLOBALS_H

typedef unsigned char Length;     /* Used for strings instead of size_t because a line is maximum 80 characters. */
typedef unsigned char Position;   /* Used for positions of bits in a word (0-14, inclusive). */
typedef unsigned char Index;      /* Used for indices of short arrays. */
typedef unsigned short Mask;      /* Used for masks being applied to a word. */
typedef unsigned short Address;   /* Used for addresses of words. */
typedef unsigned short WordCount; /* Used for counters of words, such as the instruction and data counters. */
typedef unsigned long LineNumber; /* Used for line numbers in a file. */

/* Booleans. */
typedef enum { FALSE,
               TRUE } Boolean;

/* Type (addressing mode) of an operand. */
typedef enum { IMMEDIATE,
               DIRECT,
               INDIRECT_REGISTER,
               DIRECT_REGISTER } OperandType;

/* Number of operands that should appear right after a specific operation. */
typedef enum { NO_OPERANDS,
               ONE_OPERAND,
               TWO_OPERANDS } OperandCount;

/* Macro linked list node. */
typedef struct MacroNode {
    char *name;             /* The name of the macro (identifier). */
    char *content;          /* The content of the macro (what the macro expands to). */
    struct MacroNode *next; /* The next node in the list (NULL if this is the last one). */
} Macro;

/* Word linked list node. */
typedef struct WordNode {
    unsigned char data1;   /* The first 8 bits in a word (bits 0-7). */
    unsigned char data2;   /* The last 7 bits in a word (bits 8-14). The last bit is ignored. */
    struct WordNode *next; /* The next node in the list (NULL if this is the last one). */
} Word;

/* Label linked list node. */
typedef struct LabelNode {
    char *name;             /* The name of the label (identifier). */
    LineNumber lineNumber;  /* The line number of the first .entry/.extern declaration of this label (for errors). */
    struct LabelNode *next; /* The next node in the list (NULL if this is the last one). */
} Label;

/* Used label linked list node. */
typedef struct UsedLabelNode {
    char *name;                 /* The name of the used label (identifier). */
    Address address;            /* The address of the word that the label's address has to appear in. */
    LineNumber lineNumber;      /* The line number of this specific used label instance (for errors). */
    Word *wordPointer;          /* A pointer to the word that needs to be modified to include the label's address. */
    struct UsedLabelNode *next; /* The next node in the list (NULL if this is the last one). */
} UsedLabel;

/* Found label linked list node. */
typedef struct FoundLabelNode {
    char *name;                  /* The name of the found label (identifier). */
    Address address;             /* The address of the word marked with this label. */
    Boolean isData;              /* Whether or not this label marks a word in the data part or in the code part. */
    struct FoundLabelNode *next; /* The next node in the list (NULL if this is the last one). */
} FoundLabel;

/* Struct that contains pointers to all the linked lists used in the program, in case of an allocation failure. */
typedef struct {
    Word **code;              /* The code word list. */
    Word **data;              /* The data word list. */
    Macro **macros;           /* The macro table. */
    Label **entryLabels;      /* The list of entry labels. */
    Label **externLabels;     /* The list of extern labels. */
    UsedLabel **usedLabels;   /* The list of used labels. */
    FoundLabel **foundLabels; /* The list of found labels. */
} ToFree;

/* All the operations in the language. */
enum { MOV,
       CMP,
       ADD,
       SUB,
       LEA,
       CLR,
       NOT,
       INC,
       DEC,
       JMP,
       BNE,
       RED,
       PRN,
       JSR,
       RTS,
       STOP };

/* Indices that are used explicitly. */
enum { FIRST_INDEX,
       SECOND_INDEX,
       THIRD_INDEX };

/* Bit positions that are used explicitly. */
enum { FIRST_BIT,
       SECOND_BIT,
       THIRD_BIT };

/* Exit codes. */
#define SUCCESS 0 /* Success exit code. */
#define ERROR 1   /* Error exit code. */

/* Command line arguments. */
#define ARGS_DIFF 1 /* Used when accounting for the program's name in the command line arguments. */
#define NO_FILES 0  /* Used to check if no files have been provided in the command line arguments. */

/* Made-up language and environment information. */
#define TOTAL_MEMORY_CELLS 4096     /* The number of 15-bit cells in the made-up memory. */
#define BITS_PER_MEMORY_CELL 15     /* The number of bits in each 15-bit cell. */
#define STARTING_MEMORY_ADDRESS 100 /* The starting memory address to load a word to (0-99 should not be used by the program). */
#define OPERATION_COUNT 16          /* The number of different operations in the language. */
#define MAX_LINE_LENGTH 80          /* The maximum number of characters in a line (not including any newline and null characters). */
#define MAX_NAME_LENGTH 31          /* The maximum number of characters in a label's or macro's name (they follow the same rules). */
#define MAX_IMMEDIATE 2047          /* The maximum value an immediate value (number operand of some instruction) can take. */
#define MIN_IMMEDIATE (-2048)       /* The minimum value an immediate value (number operand of some instruction) can take. */
#define MAX_NUMBER 16383            /* The maximum value a number in a .data line can take. */
#define MIN_NUMBER (-16384)         /* The minimum value a number in a .data line can take. */

/* Information about where to put different types of data inside words. */
#define STARTING_DATA_BIT 0                 /* The first bit for data in .string and .data lines. */
#define STARTING_IMMEDIATE_BIT 3            /* The first bit of an immediate value. */
#define STARTING_LABEL_BIT 3                /* The first bit of a label's address. */
#define STARTING_OPERATION_BIT 11           /* The first bit of an operation's identifier number in the first word of an instruction. */
#define STARTING_SOURCE_OPERAND_BIT 7       /* The first bit of a source operand in the first word of an instruction. */
#define STARTING_DESTINATION_OPERAND_BIT 3  /* The first bit of a destination operand in the first word of an instruction. */
#define STARTING_SOURCE_REGISTER_BIT 6      /* The first bit of a source register's number. */
#define STARTING_DESTINATION_REGISTER_BIT 3 /* The first bit of a destination register's number. */

/* Basic general constants. */
#define NULL ((void *)0)  /* The NULL pointer. */
#define EMPTY 0           /* A value representing that something is empty. */
#define INITIAL_VALUE 0   /* The initial value of many variables. */
#define EQUAL_STRINGS 0   /* Used to check if two strings are equal with strcmp. */
#define LAST_INDEX_DIFF 1 /* Used to acccess the last character in a string. */

/* Invalid values that can be returned from some functions. */
#define INVALID_STRING_LENGTH 0 /* Return value of strings in .string lines that are invalid, when trying to calculate their lengths. */
#define INVALID_DIGIT 0         /* Return value of characters that are not digits, when trying to convert them to numbers. */
#define INVALID_OPERATION 16    /* Return value of strings that are not operation names, when trying to find their corresponding operations' identifiers. */

/* Extra information that is only relevant because of the ordering of the operations. */
#define LAST_OPERATION_WITH_TWO_OPERANDS 4 /* The last operation that expects 2 operands in the defined order. */
#define LAST_OPERATION_WITH_ONE_OPERAND 13 /* The last operation that expects 1 operand (after the ones that expect 2) in the defined order. */

/* Information relating to bits and bytes that is not affected by the made-up language. */
#define BITS_PER_BYTE 8 /* The number of bits in a single byte. */
#define SINGLE_BIT 1    /* A single bit to use for masking. */
#define NULL_BYTE 1     /* The byte taken by a null character. */
#define NEWLINE_BYTE 1  /* The byte taken by a newline character. */
#define DOT_BYTE 1      /* The byte taken by a dot character. */

#endif
