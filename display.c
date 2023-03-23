////
//// Created by Rhys Mahannah on 2023-03-19.
////
//#include <stdarg.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//#define APP_NAME        "Assign08"
//#define MAX_CHAR_LENGTH 20
//#define INDEX_FILE      "accounts.idx" // For accounts balance descending order
//#define DATA_FILE       "accounts.dat" // For natural order
//
//// Struct that represents customer account
//typedef struct customerData {
//    int AccountNumber;
//    char FirstName[MAX_CHAR_LENGTH];
//    char LastName[MAX_CHAR_LENGTH];
//    double AccountBalance;
//    double LastPaymentAmount;
//} Customer;
//
//typedef struct indexHeader {        // Index file header; contains the metadata for records.
//    char Key[MAX_CHAR_LENGTH]; // The field by which the data is sorted.
//    char AppName[MAX_CHAR_LENGTH];  // The name of the index file.
//    int RecordCount;                // The number of records in the index.
//} IndexHeader;
//
//typedef struct indexRecord { // The records themselves
//    double Key;         // Key for records will be AccountBalance, as per assignment instructions.
//    long FilePosition;       // The position of the record within the records file.
//} IndexRecord;
//
//
//// FUNCTION PROTOTYPES
//long fileSize(FILE *input);
//void printNaturalOrder(int numOfArgs, char inputFileName[], ...);
//void printAccountBalanceDescendingOrder(int numOfArgs, char inputFileName[], ...);
//
//
//// DRIVES THE PROGRAM
//int main() {
//
//    int EXIT_VALUE = 3;
//    int userChoice;
//
//    printf("------------------\n"
//           "-- FILE PRINTER --\n"
//           "------------------\n"
//           "Select an option:\n"
//           "  (1) Account Balance: Natural Order\n"
//           "  (2) Account Balance: Descending Order\n"
//           "  (3) Exit Program\n"
//           ">>> ");
//    scanf("%d", &userChoice);
//
//    while (userChoice != EXIT_VALUE) {
//
//        switch (userChoice) {
//            case 1: {
//                printNaturalOrder(1, DATA_FILE);
//                break;
//            }
//            case 2: {
//                printAccountBalanceDescendingOrder(1, INDEX_FILE);
//                break;
//            }
//            default: {
//                printf("Invalid Entry. Try again.\n"
//                       ">>> ");
//            }
//        }
//
//        printf("\nSelect another option.\n"
//               "  (1) Account Balance: Natural Order\n"
//               "  (2) Account Balance: Descending Order\n"
//               "  (3) Exit Program\n"
//               ">>> ");
//        scanf("%d", &userChoice);
//    }
//    puts("Goodbye!");
//}
//
//
//// FUNCTION DEFINITIONS
//
//// Prints records in natural order.
//void printNaturalOrder(int numOfArgs, char inputFileName[], ...) {
//
//    // Confirm function was passed correct number of arguments.
//    int count;
//    int total = 0;
//    va_list args;
//
//    va_start(args, inputFileName);
//    for (count = 0; count < numOfArgs; ++count) {
//        total += va_arg(args, int);
//    }
//    va_end(args);
//
//    if (count != 1) {
//        puts("\nYou must input ONE argument for this function: An input file name.\n"
//             "Program terminating.\n");
//        exit(EXIT_FAILURE);
//    }
//
//    FILE *dataFilePtr; // Will point to the file from which data is read.
//
//    // (1) Check if file is found; if not, print error message and terminate program.
//    // Else, continue processing.
//    if ((dataFilePtr = fopen(inputFileName, "rb")) == NULL) {
//        printf("ERROR: File \"%s\" not found. Terminating program.", inputFileName);
//        exit(EXIT_FAILURE);
//    }
//
//    // Print data headers
//    printf("\n-----------------------------------------------------------------\n"
//           "NATURAL ORDER\n"
//           "-----------------------------------------------------------------\n"
//           "%7s %8s %11s %15s %18s\n"
//           "-----------------------------------------------------------------\n",
//           "Account #", "First", "Last", "Balance", "Last Payment");
//
//    // Read and print data
//    Customer tempCustomer = {0, "", "", 0.0, 0.0};
//
//    // Read in first Customer data
//    fread(&tempCustomer, sizeof(Customer), 1, dataFilePtr);
//
//    while (!feof(dataFilePtr)) {
//
//        printf("%-12d %-12s %-12s $%-12.2lf $%.2lf\n",
//               tempCustomer.AccountNumber,
//               tempCustomer.FirstName,
//               tempCustomer.LastName,
//               tempCustomer.AccountBalance,
//               tempCustomer.LastPaymentAmount);
//
//        fread(&tempCustomer, sizeof(Customer), 1, dataFilePtr);
//    }
//    fclose(dataFilePtr);
//}
//
//// Prints records based on account balance, in descending order.
//void printAccountBalanceDescendingOrder(int numOfArgs, char inputFileName[], ...) {
//
//    // Confirm number of arguments is correct.
//
//    FILE *dataFilePtr;
//    FILE *indexAccountBalanceFilePtr;
//
//    int readCount;
//    int recordCount;
//
//    IndexHeader indexHeader;
//    IndexRecord indexTemp;
//
//    // (1) Open .dat file in read-only mode. If not found, print error and terminate; else, continue.
//    if ((dataFilePtr = fopen(DATA_FILE, "rb")) == NULL) {
//        printf("ERROR: File \"%s\" not found. Terminating program.\n", DATA_FILE);
//        exit(EXIT_FAILURE);
//    }
//
//    // (2) Open .idx file in read-only mode. If not found, print error and terminate; else, continue.
//    if ((indexAccountBalanceFilePtr = fopen(INDEX_FILE, "rb")) == NULL) {
//        printf("ERROR: File \"%s\" not found. Terminating program.\n", INDEX_FILE);
//        exit(EXIT_FAILURE);
//    }
//
//    // (3) Calculate # records in .dat file
//    recordCount = fileSize(dataFilePtr) / sizeof(Customer);
//
//    // (4) Read meta data in Index Header
//    readCount = fread(&indexHeader, sizeof(IndexHeader), 1, indexAccountBalanceFilePtr);
//
//    // (5) Confirm that .dat and .idx files are the correct pair.
//    // (a) First, check that the data file names match.
//    if (strcmp(indexHeader.AppName, APP_NAME) != 0) {
//        puts("ERROR: Data File and Index File are different versions. Terminating program.");
//
//        fclose(dataFilePtr);
//        fclose(indexAccountBalanceFilePtr);
//
//        exit(EXIT_FAILURE);
//    }
//
//    // (b) Next, check that both files have the same number of records.
//    if (indexHeader.RecordCount != recordCount) {
//        puts("ERROR: Data File and Index File have different number of records. Terminating program.");
//
//        fclose(dataFilePtr);
//        fclose(indexAccountBalanceFilePtr);
//
//        exit(EXIT_FAILURE);
//    }
//
//    // (6) Print out records in specified order.
//    printf("\n-----------------------------------------------------------------\n"
//           "Account Balance (Descending Order)\n"
//           "-----------------------------------------------------------------\n"
//           "%7s %8s %11s %15s %18s\n"
//           "-----------------------------------------------------------------\n",
//           "Account #", "First", "Last", "Balance", "Last Payment");
//
//    // Read first index record
//    Customer customerTemp = {0, "", "", 0.0, 0.0};
//
//    readCount = fread(&indexTemp, sizeof(IndexRecord), 1, indexAccountBalanceFilePtr);
//
//    // Enter loop if not at EOF and the first index record, above, was properly read.
//    while ((!feof(indexAccountBalanceFilePtr)) && (readCount == 1)) {
//
//        // Check that the next record, based on the file position of the
//        // previously read index record, is the correct one; if not, print
//        // error message and terminate program.
//        if (fseek(dataFilePtr, indexTemp.FilePosition, SEEK_SET) != 0) {
//            printf("Seek Error. Terminating program.");
//
//            fclose(dataFilePtr);
//            fclose(indexAccountBalanceFilePtr);
//
//            exit(EXIT_FAILURE);
//        }
//
//        // If no errors, read the data record and print it.
//        fread(&customerTemp, sizeof(Customer), 1, indexAccountBalanceFilePtr);
//        printf("%-12d %-12s %-12s $%-12.2lf $%.2lf\n",
//               customerTemp.AccountNumber,
//               customerTemp.FirstName,
//               customerTemp.LastName,
//               customerTemp.AccountBalance,
//               customerTemp.LastPaymentAmount);
//
//        // Read next index record
//        readCount = fread(&indexTemp, sizeof(IndexRecord), 1, indexAccountBalanceFilePtr);
//    }
//
//    // When done, close files.
//    fclose(dataFilePtr);
//    fclose(indexAccountBalanceFilePtr);
//}
//
//// Determines the size of the file based on the size of one record and the total number of records.
//long fileSize(FILE *input) {
//    long orgPos;
//    long startPos;
//    long endPos;
//
//    orgPos = ftell(input); // Save original file position
//    rewind(input);
//
//    startPos = ftell(input);
//    fseek(input, 0, SEEK_END);
//
//    endPos = ftell(input);
//    fseek(input, orgPos, SEEK_SET); // Restore original file position
//
//    return (endPos - startPos);
//}
