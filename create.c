////
//// Created by Rhys Mahannah on 2023-03-18.
////
//#include <stdio.h>
//#include <stdarg.h>
//#include <string.h>
//#include <stdlib.h>
//
//#define MAX_CHAR_LENGTH  20
//#define INPUT_FILE_NAME  "accounts.csv"
//#define OUTPUT_FILE_NAME "accounts.dat"
//
//// Struct that represents a customer account.
//struct customerData
//{
//    int    AccountNumber;
//    char   FirstName[ MAX_CHAR_LENGTH ];
//    char   LastName[ MAX_CHAR_LENGTH ];
//    double AccountBalance;
//    double LastPaymentAmount;
//};
//
//typedef struct customerData CustomerData;
//
//
//// FUNCTION PROTOTYPE
//void readAndWriteCustomerDataToBinaryFile(int numOfArgs, char inputFileName[], char outputFileName[], ...);
//
//
//// DRIVES THE PROGRAM
//int main() {
//    readAndWriteCustomerDataToBinaryFile(2, INPUT_FILE_NAME, OUTPUT_FILE_NAME);
//}
//
//
//// FUNCTION DEFINITION
//void readAndWriteCustomerDataToBinaryFile(int numOfArgs, char inputFileName[], char outputFileName[], ...) {
//
//    // CONFIRM ARGUMENT COUNT
//    int count;
//    int total = 0;
//    va_list args;
//
//    va_start(args, outputFileName);
//    for (count = 0; count < numOfArgs; ++count)
//    {
//        total += va_arg(args, int);
//    }
//    va_end(args);
//
//    if (count != 2) {
//        puts("\nYou must input two arguments for this function:\n"
//             "(1) An input file name.\n"
//             "(2) the name of the output file.\n"
//             "Program terminating.\n");
//        exit(EXIT_FAILURE);
//    }
//
//    // RUN FUNCTION
//    FILE *CustomerDataInputFilePtr;  // Pointer to file to be read.
//    FILE *CustomerDataOutputFilePtr; // Pointer to file to be created/written to.
//
//    // (1) Attempt to read file. If it does exist, issue warning to user that file will be overwritten.
//    // If user accepts, then open file in write-mode.
//    if ((CustomerDataOutputFilePtr = fopen(outputFileName, "rb")) != NULL)
//    {
//        fclose(CustomerDataOutputFilePtr); // Immediately close file; it's confirmed to exist.
//
//        // Asks user if they'd like to overwrite file.
//        char userChoice;
//        printf("\nThe file \"%s\" already exists.\n"
//               "If you continue, the contents of the file will be overwritten.\n"
//               "Overwrite file? (y)es or (n)o\n"
//               ">>> ", outputFileName);
//
//        scanf("%c", &userChoice);
//
//        while (userChoice != 'y' && userChoice != 'n') {
//            puts("Please enter 'y' for yes or 'n' for no.\n");
//            scanf("%c", &userChoice);
//        }
//
//        if (userChoice == 'n') {
//            printf("\nThe file \"%s\" will NOT be overwritten. Terminating program.\n", outputFileName);
//            exit(EXIT_SUCCESS);
//        }
//
//        // User has chosen to overwrite file; check that file was successfully accessed.
//        if ((CustomerDataOutputFilePtr = fopen(outputFileName, "wb+")) == NULL) {
//            printf("WARNING: \"%s\" was not accessed. Terminating program\n", outputFileName);
//            exit(EXIT_FAILURE);
//        }
//
//        printf("\nOverwriting \"%s\"...", outputFileName);
//    }
//
//    // File did NOT exist, so attempt to create file; if that fails, issue warning and terminate program.
//    else
//    {
//        if ((CustomerDataOutputFilePtr = fopen(outputFileName, "ab+")) == NULL) {
//            printf("WARNING: \"%s\" was not created. Terminating program", outputFileName);
//            exit(EXIT_FAILURE);
//        }
//
//        printf("\nWriting customer data to \"%s\"...\n", outputFileName);
//    }
//
//    // (2) Open file for reading.
//    // If it doesn't exist, print warning, terminate program, and close output file; else, process data.
//    if ((CustomerDataInputFilePtr = fopen(inputFileName, "r")) == NULL)
//    {
//        printf("\nWARNING: \"%s\" not found. Terminating program.", inputFileName);
//        fclose(CustomerDataOutputFilePtr);
//        exit(EXIT_FAILURE);
//    }
//    else
//    {
//
//        CustomerData customer = {0, "", "", 0.0, 0.0};
//
//        // Loop through file and save records to struct until end-of-file.
//        while (!feof(CustomerDataInputFilePtr))
//        {
//            // Scan data and save it to appropriate variables in struct.
//            // For strings, use formatting to read all characters up to but NOT including commas.
//            // A space before and after the values means fscanf() will skip all whitespace characters.
//            fscanf(CustomerDataInputFilePtr, " %d , %19[^,] , %19[^,] , %lf , %lf \n",
//                   &customer.AccountNumber,
//                   customer.FirstName,
//                   customer.LastName,
//                   &customer.AccountBalance,
//                   &customer.LastPaymentAmount);
//
//            // Write struct to binary file.
//            fwrite( &customer, sizeof(CustomerData), 1, CustomerDataOutputFilePtr);
//        }
//    }
//    fclose(CustomerDataInputFilePtr);  // Close read-only file.
//    fclose(CustomerDataOutputFilePtr); // Close output file -- the file to which data was written.
//
//    puts("\nData successfully written.");
//}
