//
// Created by: Rhys Mahannah
// Date:       Sunday, March 26, 2023
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assign08.h"

IndexKey indexKey;

// Drives the program
int main(int argc, char *argv[]) {

    // Check number of arguments passed to main().
    if (argc != NUM_OF_ARGS) {
        printf("\nYou must pass 3 arguments. Terminating program.");
        exit(EXIT_FAILURE);
    }

    // Continue with program.
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

    // Open DAT file in read-only mode.
    if ((dataFilePtr = fopen(argv[1], "rb")) == NULL) {
        printf("\nERROR: File \"%s\" not found. Terminating program.\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Create IDX file to which data will be written.
    if ((indexAccountBalanceFilePtr = fopen(argv[2], "wb")) == NULL) {
        printf("\nERROR: File \"%s\" could not be created. Terminating program.", argv[2]);
        exit(EXIT_FAILURE);
    }

    dataRecordCount = fileSize(dataFilePtr) / sizeof(Customer); // Determines number of records in .dat file.

    // Initialize the number of indexRecords equal to the number of records we'll be reading (calculated above).
    indexRecords = (IndexRecord *) calloc(dataRecordCount, sizeof(IndexRecord));

    // Add metadata to index header.
    strcpy(indexHeader.AppName, APP_NAME);     // Cannot assign string; must copy string to destination.
    indexHeader.IndexKey = indexKey;
    indexHeader.RecordCount = dataRecordCount; // Number of records the file will contain.

    // Start writing data to index file.
    rewind(dataFilePtr);               // Ensure, for reading, we're at beginning of DAT file.
    filePosition = ftell(dataFilePtr); // Determine current position within DAT file.
    indexRecordCount = 0;

    // Read first data record; this will help confirm, below, that we're successfully reading data.
    Customer tempCustomer = {0, "", "", 0.0, 0.0};
    readCount = fread(&tempCustomer, sizeof(Customer), 1, dataFilePtr);

    // Loop runs if not end-of-file AND if first record, above, was read successfully.
    while (!feof(dataFilePtr) && readCount == 1) {
        indexRecords[indexRecordCount].Key.AccountBalance = tempCustomer.AccountBalance; // This is how records are org.
        indexRecords[indexRecordCount].FilePosition = filePosition;                      // Position of record in file.

        ++indexRecordCount;                // Move to the next record to be written.
        filePosition = ftell(dataFilePtr); // Save the position where next record will be written.

        readCount = fread(&tempCustomer, sizeof(Customer), 1, dataFilePtr); // Load next file to be written.
    }

    // Organize data in header according to AccountBalance descending, as per assignment instructions.
    // qsort() takes as its fourth argument a function pointer, so we must pass ADDRESS of compare function.
    qsort(indexRecords, indexRecordCount, sizeof(IndexRecord), &compare);

    printf("\nIndex Header Size: %lu\n"
           "Index Record Size: %lu\n\n",
           sizeof(IndexHeader),
           sizeof(IndexRecord));

    puts("Creating index file...\n");

    // Write Index Header to file -- this contains the metadata for the records it contains.
    fwrite(&indexHeader, sizeof(IndexHeader), 1, indexAccountBalanceFilePtr);

    // Write records to file -- this is the actual data being stored.
    writeCount = fwrite(indexRecords, sizeof(IndexRecord), indexRecordCount, indexAccountBalanceFilePtr);

    printf("%d records were processed.\n", writeCount);
    printf("Index file size: %ld\n\n", fileSize(indexAccountBalanceFilePtr));

    // Free memory dynamically allocated to create records.
    free(indexRecords);

    // Close both files.
    fclose(dataFilePtr);
    fclose(indexAccountBalanceFilePtr);
}

// Determines the size of the file.
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

    if (pLeft->Key.AccountBalance > pRight->Key.AccountBalance) {
        return -1; // Ascending order
    } else if (pLeft->Key.AccountBalance < pRight->Key.AccountBalance) {
        return 1;  // Descending order
    }
    return 0;      // Equivalent
}
