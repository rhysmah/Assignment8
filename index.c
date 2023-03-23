//
// Created by Rhys Mahannah on 2023-03-23.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define APP_NAME        "Assignment08"
#define DAT_FILE        "accounts.dat"
#define IDX_NAME        "accounts.idx"

#define MAX_CHAR_LENGTH 20

typedef enum {
    ACCOUNT,
    FIRST,
    LAST,
    ACCOUNT_BALANCE
} IndexKey;

typedef union {
    int Account;
    char First[MAX_CHAR_LENGTH];
    char Last[MAX_CHAR_LENGTH];
    double AccountBalance;
} KeyType;

typedef struct { // The struct whose data we'll be accessing in the .dat file.
    int AccountNumber;
    char FirstName[MAX_CHAR_LENGTH];
    char LastName[MAX_CHAR_LENGTH];
    double AccountBalance;
    double LastPaymentAmount;
} Customer;

typedef struct {                   // Index file header; contains the metadata for records.
    IndexKey IndexKey;             // The field by which the data is sorted.
    char AppName[MAX_CHAR_LENGTH]; // The name of the index file.
    int RecordCount;               // The number of records in the index.
} IndexHeader;

typedef struct {       // The records themselves
    KeyType Key;       // Key for records will be AccountBalance, as per assignment instructions.
    long FilePosition; // The position of the record within the records file.
} IndexRecord;


// FUNCTION PROTOTYPES
long fileSize(FILE *input);
int compare(const void *left, const void *right);

IndexKey indexKey;

// DRIVES THE PROGRAM
int main() {

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

    // (1) Open the .dat binary file in read-only mode.
    // Check that it exists; if not, print error message and terminate program, else continue.
    if ((dataFilePtr = fopen(DAT_FILE, "rb")) == NULL) {
        printf("\nERROR: File \"%s\" not found. Terminating program.\n", DAT_FILE);
        exit(EXIT_FAILURE);
    }

    // (2) Create an index file to which metadata will be written.
    // Check that it can be created; if not, print error message and terminate program, else continue.
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
    indexHeader.IndexKey = indexKey;           // Cannot assign string; must copy string to destination.
    indexHeader.RecordCount = dataRecordCount; // Number of records the file will contain.

    // (4) Start writing data to index file.
    rewind(dataFilePtr);               // Ensure, for reading, that we're at the beginning of the .dat file.
    filePosition = ftell(dataFilePtr); // Determine the current position within the .dat file.
    indexRecordCount = 0;              // Initialize record count to 0;

    // Read first data record; this will help confirm, below, that we're successfully reading data.
    // Reading ONE piece of data, the size of Customer, and reading that INTO our dummy data, which we
    // can then write to our .idx file.
    Customer tempCustomer = {0, "", "", 0.0, 0.0}; // Dummy data to which we'll be reading data from .dat file.

    readCount = fread(&tempCustomer, sizeof(Customer), 1, dataFilePtr);

    // Set loop that runs if not at EOF and first record, above, was read successfully.
    while (!feof(dataFilePtr) && readCount == 1) {
        indexRecords[indexRecordCount].Key.AccountBalance = tempCustomer.AccountBalance; // This is how records are org.
        indexRecords[indexRecordCount].FilePosition = filePosition;            // Position of record in file.

        ++indexRecordCount;                // Move to the next record to be written.
        filePosition = ftell(dataFilePtr); // Save the position where next record will be written.

        readCount = fread(&tempCustomer, sizeof(Customer), 1, dataFilePtr); // Load next file to be written.
    }

    // (5) Organize data in header according to AccountBalance descending, as per assignment instructions.
    qsort(indexRecords, indexRecordCount, sizeof(IndexRecord), compare);

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

    // Free memory dynamically allocated to creating the records.
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
    return pRight->Key.AccountBalance > pLeft->Key.AccountBalance;
}
