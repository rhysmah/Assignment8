//
// Created by Rhys Mahannah on 2023-03-23.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define APP_NAME "Assignment08"
#define DAT_FILE "accounts.dat"
#define IDX_NAME "accounts.idx"

#define MAX_CHAR_LENGTH 20

// How we'll sort data. Created enum, because we may want to add more sorting fields.
typedef enum {
    ACCOUNT_BALANCE
} IndexKey;

// Union not needed for one sorting option, but if we want to sort by different data types,
// in the future, which is a reasonable assumption, it'd be efficient to share memory space.
typedef union {
    double AccountBalance;
} KeyType;

typedef struct {
    int AccountNumber;
    char FirstName[MAX_CHAR_LENGTH];
    char LastName[MAX_CHAR_LENGTH];
    double AccountBalance;
    double LastPaymentAmount;
} Customer;

typedef struct {
    KeyType Key;
    long FilePosition;
} IndexRecord;

typedef struct {                   // Index file header; contains the metadata for records.
    IndexKey IndexKey;             // The field by which the data is sorted.
    char AppName[MAX_CHAR_LENGTH]; // The name of the index file.
    int RecordCount;               // The number of records in the index.
} IndexHeader;


// FUNCTION PROTOTYPES
long fileSize(FILE *input);

int compare(const void *left, const void *right);


IndexKey indexKey;

// DRIVES THE PROGRAM
int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("\nYou must pass 3 arguments. Terminating program.");
        exit(EXIT_FAILURE);
    }

    FILE *dataFilePtr;                // Will point to the .dat file from which we'll be reading.
    FILE *indexAccountBalanceFilePtr; // Will point to the .idx file we'll be creating.

    int readCount;
    int writeCount;
    int dataRecordCount;
    int indexRecordCount;
    long filePosition;

    IndexHeader indexHeader;
    IndexRecord *indexRecords;

    indexKey = ACCOUNT_BALANCE;

    // (1) Open DAT file in read-only mode.
    if ((dataFilePtr = fopen(DAT_FILE, "rb")) == NULL) {
        printf("\nERROR: File \"%s\" not found. Terminating program.\n", DAT_FILE);
        exit(EXIT_FAILURE);
    }

    // (2) Create IDX file to which data will be written.
    if ((indexAccountBalanceFilePtr = fopen(IDX_NAME, "wb")) == NULL) {
        printf("\nERROR: File \"%s\" could not be created. Terminating program.", IDX_NAME);
        exit(EXIT_FAILURE);
    }

    // If (1) and (2) successful, start processing data.
    dataRecordCount = fileSize(dataFilePtr) / sizeof(Customer); // Determines number of records in .dat file.

    // Initialize the number of indexRecords equal to the number of records we'll be reading (calculated above).
    indexRecords = (IndexRecord *) calloc(dataRecordCount, sizeof(IndexRecord));

    // (3) Add metadata to index header.
    strcpy(indexHeader.AppName, APP_NAME);     // Cannot assign string; must copy string to destination.
    indexHeader.IndexKey = indexKey;
    indexHeader.RecordCount = dataRecordCount; // Number of records the file will contain.

    // (4) Start writing data to index file.
    rewind(dataFilePtr);               // Ensure, for reading, we're at beginning of DAT file.
    filePosition = ftell(dataFilePtr); // Determine current position within DAT file.
    indexRecordCount = 0;

    // (5) Read first data record; this will help confirm, below, that we're successfully reading data.
    Customer tempCustomer = {0, "", "", 0.0, 0.0};
    readCount = fread(&tempCustomer, sizeof(Customer), 1, dataFilePtr);

    // (6) Loop runs if not EOF AND if first record, above, was read successfully.
    while (!feof(dataFilePtr) && readCount == 1) {
        indexRecords[indexRecordCount].Key.AccountBalance = tempCustomer.AccountBalance; // This is how records are org.
        indexRecords[indexRecordCount].FilePosition = filePosition;                      // Position of record in file.

        ++indexRecordCount;                // Move to the next record to be written.
        filePosition = ftell(dataFilePtr); // Save the position where next record will be written.

        readCount = fread(&tempCustomer, sizeof(Customer), 1, dataFilePtr); // Load next file to be written.
    }

    // (&) Organize data in header according to AccountBalance descending, as per assignment instructions.
    // qsort() takes as its fourth argument a function pointer, so we must pass ADDRESS of compare function.
    qsort(indexRecords, indexRecordCount, sizeof(IndexRecord), &compare);

    printf("Index Header Size: %lu\n"
           "Index Record Size: %lu\n"
           "\n",
           sizeof(IndexHeader),
           sizeof(IndexRecord));

    puts("Creating index file...\n");

    // Write Index Header to file -- this contains the metadata for the records it contains.
    fwrite(&indexHeader, sizeof(IndexHeader), 1, indexAccountBalanceFilePtr);

    // Write records to file -- this is the actual data being stored.
    writeCount = fwrite(indexRecords, sizeof(IndexRecord), indexRecordCount, indexAccountBalanceFilePtr);

    printf("%d records were processed.\n", writeCount);
    printf("Index file size: %ld\n", fileSize(indexAccountBalanceFilePtr));

    // Free memory dynamically allocated to create records.
    free(indexRecords);

    // Close both files.
    fclose(dataFilePtr);
    fclose(indexAccountBalanceFilePtr);
}


// FUNCTION DEFINITIONS

// Determines the size of the file based on the size of one record and the total number of records.
long fileSize(FILE *input) {
    long orgPos;
    long startPos;
    long endPos;

    orgPos = ftell(input); // Save original file position
    rewind(input);

    startPos = ftell(input);
    fseek(input, 0, SEEK_END);

    endPos = ftell(input);
    fseek(input, orgPos, SEEK_SET); // Restore original file position

    return (endPos - startPos);
}

// Compares left with right; returns 1 if descending order.
int compare(const void *left, const void *right) {
    // Cast pointer to parameters to IndexRecord.
    IndexRecord *pLeft = (IndexRecord *) left;
    IndexRecord *pRight = (IndexRecord *) right;

    // Return result based on size of Account Balance.
    if (pLeft->Key.AccountBalance > pRight->Key.AccountBalance) {
        return -1; // This would be ascending order, which we don't want.
    } else if (pLeft->Key.AccountBalance < pRight->Key.AccountBalance) {
        return 1;  // This allows us to sort Account Balance in descending order.
    }
    return 0; // If the Account Balances are the same,
}
