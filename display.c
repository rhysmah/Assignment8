//
// Created by Rhys Mahannah on 2023-03-19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assign08.h"

// Drives the program
int main(int argc, char *argv[]) {

    // Check number of arguments passed to main().
    if (argc != 3) {
        printf("\nThis program requires 3 arguments. Terminating program.");
        exit(EXIT_FAILURE);
    }

    // Check files
    FILE *dataFilePtr;
    FILE *indexFilePtr;

    // Open .dat file in read-only mode. If not found, print error and terminate; else, continue.
    if ((dataFilePtr = fopen(argv[1], "rb")) == NULL) {
        printf("ERROR: File \"%s\" not found. Terminating program.\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Open .idx file in read-only mode. If not found, print error and terminate; else, continue.
    if ((indexFilePtr = fopen(argv[2], "rb")) == NULL) {
        printf("ERROR: File \"%s\" not found. Terminating program.\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    // Function pointer
    FUNCTION_ARRAY_PTR myFunctions;
    myFunctions[1] = &printAccountBalanceNaturalOrder;
    myFunctions[2] = &printAccountBalanceInDescendingOrder;

    // Continue with program.
    int EXIT_VALUE = 3;
    int userChoice;

    printf("------------------\n"
           "-  FILE PRINTER  -\n"
           "------------------\n"
           "Select an option:\n"
           "  (1) Account Balance: Natural Order\n"
           "  (2) Account Balance: Descending Order\n"
           "  (3) Exit Program\n"
           ">>> ");
    scanf("%d", &userChoice);

    while (userChoice != EXIT_VALUE) {

        switch (userChoice) {
            case 1: {
                myFunctions[1](dataFilePtr, NULL); // Print natural order.
                break;
            }
            case 2: {
                myFunctions[2](dataFilePtr, indexFilePtr); // Print descending order.
                break;
            }
            default: {
                printf("Invalid Entry. Try again.\n"
                       ">>> ");
            }
        }
        printf("\nSelect another option:\n"
               "  (1) Account Balance: Natural Order\n"
               "  (2) Account Balance: Descending Order\n"
               "  (3) Exit Program\n"
               ">>> ");
        scanf("%d", &userChoice);
    }
    puts("Goodbye!\n");
}

// Prints records in natural order.
void printAccountBalanceNaturalOrder(FILE* dataFilePtr, FILE* null) {

    // Print data headers
    printf("\nNATURAL ORDER\n"
           "-----------------------------------------------------------------\n"
           "%7s %8s %11s %15s %18s\n"
           "-----------------------------------------------------------------\n",
           "Account #", "First", "Last", "Balance", "Last Payment");

    // Read and print data
    Customer tempCustomer = {0, "", "", 0.0, 0.0};

    // Read in first Customer data
    fread(&tempCustomer, sizeof(Customer), 1, dataFilePtr);

    while (!feof(dataFilePtr)) {

        printf("%-12d %-12s %-12s $%-12.2lf $%.2lf\n",
               tempCustomer.AccountNumber,
               tempCustomer.FirstName,
               tempCustomer.LastName,
               tempCustomer.AccountBalance,
               tempCustomer.LastPaymentAmount);

        fread(&tempCustomer, sizeof(Customer), 1, dataFilePtr);
    }
    fclose(dataFilePtr);
}

// Prints records based on account balance, in descending order.
void printAccountBalanceInDescendingOrder(FILE* dataFilePtr, FILE* indexFilePtr) {

    int readCount;
    int recordCount;

    IndexHeader indexHeader;
    IndexRecord indexTemp;

    // Calculate # records in .dat file
    recordCount = fileSize(dataFilePtr) / sizeof(Customer);

    // Read meta data in Index Header
    fread(&indexHeader, sizeof(IndexHeader), 1, indexFilePtr);

    // Confirm that .dat and .idx files are the correct pair.
    // First, check that the data file names match.
    if (strcmp(indexHeader.AppName, APP_NAME) != 0) {
        puts("ERROR: Data File and Index File are different versions. Terminating program.");

        fclose(dataFilePtr);
        fclose(indexFilePtr);

        exit(EXIT_FAILURE);
    }

    // Next, check that both files have the same number of records.
    if (indexHeader.RecordCount != recordCount) {
        puts("ERROR: Data File and Index File have different number of records. Terminating program.");

        fclose(dataFilePtr);
        fclose(indexFilePtr);

        exit(EXIT_FAILURE);
    }

    // Print out records in specified order.
    printf("\nAccount Balance (Descending Order)\n"
           "-----------------------------------------------------------------\n"
           "%7s %8s %11s %15s %18s\n"
           "-----------------------------------------------------------------\n",
           "Account #", "First", "Last", "Balance", "Last Payment");

    // Read first index record
    Customer customerTemp = {0, "", "", 0.0, 0.0};
    readCount = fread(&indexTemp, sizeof(IndexRecord), 1, indexFilePtr);

    // Enter loop if not at EOF and the first index record, above, was properly read.
    while ((!feof(indexFilePtr)) && (readCount == 1)) {

        // Check that the next record, based on the file position of the
        // previously read index record, is the correct one; if not, print
        // error message and terminate program.
        if (fseek(dataFilePtr, indexTemp.FilePosition, SEEK_SET) != 0) {
            printf("Seek Error. Terminating program.");

            fclose(dataFilePtr);
            fclose(indexFilePtr);

            exit(EXIT_FAILURE);
        }

        // If no errors, read the data record and print it.
        fread(&customerTemp, sizeof(Customer), 1, dataFilePtr);
        printf("%-12d %-12s %-12s $%-12.2lf $%.2lf\n",
               customerTemp.AccountNumber,
               customerTemp.FirstName,
               customerTemp.LastName,
               customerTemp.AccountBalance,
               customerTemp.LastPaymentAmount);

        // Read next index record
        readCount = fread(&indexTemp, sizeof(IndexRecord), 1, indexFilePtr);
    }

    // When done, close files.
    fclose(dataFilePtr);
    fclose(indexFilePtr);
}

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
