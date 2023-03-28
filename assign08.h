//
// Created by Rhys Mahannah on 2023-03-26.
//

#include <stdio.h>

#ifndef ASSIGN08_H
    #define ASSIGN08_H
    #define APP_NAME          "Assign08"
    #define MAX_CHAR_LENGTH   20
    #define MAX_FUNC_PTR_ARGS 3
    #define NUM_OF_ARGS       3
#endif

typedef enum { ACCOUNT_BALANCE } IndexKey;

typedef union {
    double      AccountBalance;
} KeyType;

typedef struct {
    int         AccountNumber;
    char        FirstName[MAX_CHAR_LENGTH];
    char        LastName[MAX_CHAR_LENGTH];
    double      AccountBalance;
    double      LastPaymentAmount;
} Customer;

typedef struct {
    KeyType     Key;
    long        FilePosition;
} IndexRecord;

typedef struct {
    IndexKey    IndexKey;
    char        AppName[MAX_CHAR_LENGTH];
    int         RecordCount;
} IndexHeader;

typedef void(*FUNCTION_ARRAY_PTR[MAX_FUNC_PTR_ARGS]) (char fileName1[], char fileName2[]);


// Function prototypes
long fileSize( FILE *input );

int compare( const void *left, const void *right );

void printAccountBalanceNaturalOrder(char dataFile[], char indexFile[] );

void printAccountBalanceInDescendingOrder( char dataFile[], char indexFile[] );
