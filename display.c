////
//// Created by Rhys Mahannah on 2023-03-19.
////
//#include <stdarg.h>
//#include <stdio.h>
//#include <stdlib.h>
//
//#define MAX_NAME_LENGTH 20
//#define FILE_NAME       "accounts.dat"
//
//// Struct that represents customer account
//struct customerData
//{
//    int    AccountNumber;
//    char   FirstName[ MAX_NAME_LENGTH ];
//    char   LastName[ MAX_NAME_LENGTH ];
//    double AccountBalance;
//    double LastPaymentAmount;
//};
//
//typedef struct customerData CustomerData;
//
//// FUNCTION PROTOTYPE
//void printCustomerDataFromBinaryFile(int numOfArgs, char inputFileName[], ...);
//
//
//// DRIVES THE PROGRAM
//int main() {
//    printCustomerDataFromBinaryFile(1, FILE_NAME);
//}
//
//
//// FUNCTION DEFINITION
//// Prints to screen customer data from binary file.
//void printCustomerDataFromBinaryFile(int numOfArgs, char inputFileName[], ...) {
//
//    // Confirm function was passed correct number of arguments (two).
//    int count;
//    int total = 0;
//    va_list args;
//
//    va_start(args, inputFileName);
//    for (count = 0; count < numOfArgs; ++count)
//    {
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
//    // Run function
//    FILE *CustomerDataInputFilePtr; // Pointer to file to be read.
//
//    // Open binary file for reading; if it doesn't exist, issue warning and terminate program.
//    if ((CustomerDataInputFilePtr = fopen( FILE_NAME, "rb")) == NULL)
//    {
//        printf("WARNING: \"%s\" not found.", FILE_NAME);
//    }
//    else // If file opens fine, process data for printing.
//    {
//        CustomerData customer = {0, "", "", 0.0, 0.0};
//
//        // Print headers
//        printf("-----------------------------------------------------------------\n"
//               "%7s %8s %11s %15s %18s\n"
//               "-----------------------------------------------------------------\n",
//               "Account #", "First", "Last", "Balance", "Last Payment");
//
//        // While loop checks if fread() returns 1 (True); if so, it contains data and will print to console.
//        while (fread(&customer, sizeof(CustomerData), 1, CustomerDataInputFilePtr) == 1) {
//
//            // Print customer data under headings
//            printf("%-12d %-12s %-12s $%-12.2lf $%.2lf\n",
//                   customer.AccountNumber,
//                   customer.FirstName,
//                   customer.LastName,
//                   customer.AccountBalance,
//                   customer.LastPaymentAmount);
//        }
//        fclose(CustomerDataInputFilePtr); // Close file when done.
//    }
//}
