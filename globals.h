#ifndef CONSTANTS_H
#define CONSTANTS_H

extern const char *OPERATIONS[16];

typedef enum { FALSE,
               TRUE } boolean;
typedef enum { IMMEDIATE,
               DIRECT,
               INDIRECT_REGISTER,
               DIRECT_REGISTER } operandType;

#endif
