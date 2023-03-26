//
// Created by Rhys Mahannah on 2023-03-26.
//

#include <stdio.h>

#ifndef ASSIGN08_H
    #define ASSIGN08_H
    #define MAX_CHAR_LENGTH 20
    #define APP_NAME "Assign08"
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


// Function prototypes
long fileSize( FILE *input );

int compare( const void *left, const void *right );

void printNaturalOrder( char dataFile[] );

void printAccountBalanceInDescendingOrder( char dataFile[], char indexFile[] );
