//
// Created by Rhys Mahannah on 2023-03-18.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CHAR_LENGTH  20
#define INPUT_FILE_NAME  "accounts.csv"
#define OUTPUT_FILE_NAME "accounts.dat"

typedef struct {
    int    AccountNumber;
    char   FirstName[MAX_CHAR_LENGTH];
    char   LastName[MAX_CHAR_LENGTH];
    double AccountBalance;
    double LastPaymentAmount;
} CustomerData;


// DRIVES THE PROGRAM
int main(int argc, char *argv[]) {

    FILE *inputCSVFilePtr;  // Pointer to CSV file to be read.
    FILE *outputDATFilePtr; // Pointer to DAT file to be created.

    // (1) Open CSV input file in read-only mode.
    if ((inputCSVFilePtr = fopen(INPUT_FILE_NAME, "rb")) == NULL) {
        printf("\nERROR: \"%s\" not found. Terminating operation.", INPUT_FILE_NAME);
        exit(EXIT_FAILURE);
    }

    // (2) Check if DAT output file already exists.
    if ((outputDATFilePtr = fopen(OUTPUT_FILE_NAME, "rb")) != NULL) {

        // DAT file confirmed to exist and is currently opened in read-only mode.
        // Immediately close file; if user wants to overwrite current file, it will be re-opened in write mode.
        fclose(outputDATFilePtr);

        // (3) If DAT file already exists, ask user if they want to overwrite it.
        char userChoice;
        printf("\n"
               "File \"%s\" already exists. Overwrite?\n"
               "y or n: ", OUTPUT_FILE_NAME);
        scanf("%c", &userChoice);

        while (userChoice != 'y' && userChoice != 'n') {
            puts("Please enter 'y' for yes or 'n' for no.\n");
            scanf("%c", &userChoice);
        }

        if (userChoice == 'n') {
            printf("\n"
                   "File \"%s\" will not be overwritten. Terminating program.\n", OUTPUT_FILE_NAME);
            exit(EXIT_SUCCESS);
        }
    }

    // (4) Open DAT file in write mode.
    if ((outputDATFilePtr = fopen(OUTPUT_FILE_NAME, "wb")) == NULL) {
        printf("\nERROR: \"%s\" not accessed. Terminating operation.", OUTPUT_FILE_NAME);
        exit(EXIT_FAILURE);
    }

    // (5) Write contents from CSV file into DAT file (binary).
    CustomerData tempCustomer = {0, "", "", 0.0, 0.0}; // Dummy data

    printf("\nWriting data from \"%s\" to \"%s\"...", INPUT_FILE_NAME, OUTPUT_FILE_NAME);

    // Loop through file and save records to struct until end-of-file.
    int recordCount = 0;
    while (!feof(inputCSVFilePtr)) {

        // Scan data; save it to appropriate variables in struct.
        // For strings, skip whitespaces and commas.
        fscanf(inputCSVFilePtr, " %d , %19[^,] , %19[^,] , %lf , %lf \n",
               &tempCustomer.AccountNumber,
               tempCustomer.FirstName,
               tempCustomer.LastName,
               &tempCustomer.AccountBalance,
               &tempCustomer.LastPaymentAmount);

        // Write struct to in binary to DAT file.
        fwrite(&tempCustomer, sizeof(CustomerData), 1, outputDATFilePtr);

        ++recordCount;
    }
    fclose(inputCSVFilePtr);
    fclose(outputDATFilePtr);

    printf("\n%d record(s) were successfully written to \"%s\"\n", recordCount, OUTPUT_FILE_NAME);
}

// Put into .h -- use them for both
// Struct definitions
// function prototypes
// typedefs

// Implement display.c menu using function pointers.