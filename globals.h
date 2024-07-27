#ifndef GLOBALS_H
#define GLOBALS_H

typedef unsigned char position;
typedef unsigned short mask;

typedef enum { FALSE,
               TRUE } boolean;
typedef enum { IMMEDIATE,
               DIRECT,
               INDIRECT_REGISTER,
               DIRECT_REGISTER } operandType;

typedef enum { NO_OPERANDS,
               ONE_OPERAND,
               TWO_OPERANDS } operandCount;

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
    unsigned lineNumber;
    struct labelStruct *next;
} label;

typedef struct usedLabelStruct {
    char *name;
    unsigned address;
    unsigned lineNumber;
    word *wordPointer;
    struct usedLabelStruct *next;
} usedLabel;

typedef struct foundLabelStruct {
    char *name;
    unsigned address;
    boolean isData;
    struct foundLabelStruct *next;
} foundLabel;

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

enum { FIRST_INDEX,
       SECOND_INDEX,
       THIRD_INDEX };

enum { FIRST_BIT,
       SECOND_BIT,
       THIRD_BIT };

#define SUCCESS 0
#define ERROR 1

#define ARGS_DIFF 1
#define NO_FILES 0

#define TOTAL_MEMORY_CELLS 4096
#define BITS_PER_MEMORY_CELL 15
#define STARTING_MEMORY_ADDRESS 100
#define OPERATION_COUNT 16
#define MAX_LINE_LENGTH 80
#define MAX_NAME_LENGTH 31
#define MAX_IMMEDIATE 2047
#define MIN_IMMEDIATE (-2048)
#define MAX_NUMBER 16383
#define MIN_NUMBER (-16384)

#define STARTING_DATA_BIT 0
#define STARTING_NUMBER_BIT 3
#define STARTING_LABEL_BIT 3
#define STARTING_OPERATION_BIT 11
#define STARTING_SOURCE_OPERAND_BIT 7
#define STARTING_DESTINATION_OPERAND_BIT 3
#define STARTING_SOURCE_REGISTER_BIT 6
#define STARTING_DESTINATION_REGISTER_BIT 3

#define EMPTY 0
#define INITIAL_VALUE 0
#define EQUAL_STRINGS 0
#define LAST_INDEX_DIFF 1

#define ERROR_MESSAGE_LENGTH 53

#define NO_TOKEN 0

#define INVALID_DIGIT 0

#define INVALID_OPERATION 16
#define LAST_OPERATION_WITH_TWO_OPERANDS 4
#define LAST_OPERATION_WITH_ONE_OPERAND 13

#define SINGLE_BIT 1
#define BITS_PER_BYTE 8
#define NULL_BYTE 1
#define NEWLINE_BYTE 1
#define DOT_BYTE 1

#endif
