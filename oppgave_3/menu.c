#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/menu.h"
#include "include/reservation.h"

// Read input and store it in a variable
// Returns 1 if the input was successfully read and stored, 0 otherwise
int readInput(char *pszInput, char *pszPrompt, void *pVar, const char *pcszFormat)
{
    while (1)
    {
        printf("%s", pszPrompt);
        fgets(pszInput, 256, stdin);
        // Remove the newline character at the end of the line
        pszInput[strcspn(pszInput, "\n")] = 0;

        if (sscanf(pszInput, pcszFormat, pVar) == 1)
            return 1;
        else
            printf("Invalid input. Please try again.\n");
    }
}

void printMenu()
{
    printf("\nPlease select an option from the menu below:\n");
    printf("1. Add reservation\n");
    printf("6. Print reservations\n");
    printf("0. Exit menu\n");
}

void addReservationOption()
{
    char pszInput[256]; // Large enough to hold multiple lines of input
    char pszName[32];
    char pszRoomNumber[8];
    int iDate;
    int iNumDays;
    float bPricePerDay;

    printf("Enter the following information:\n");

    int c = getchar(); // Consume any pending newline characters
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }

    readInput(pszInput, "Name: ", pszName, "%[^\n]");
    readInput(pszInput, "Room number: ", pszRoomNumber, "%[^\n]");
    readInput(pszInput, "Date: ", &iDate, "%d");
    readInput(pszInput, "Number of days: ", &iNumDays, "%d");
    readInput(pszInput, "Price per day: ", &bPricePerDay, "%f");

    addReservation(pszName, pszRoomNumber, iDate, iNumDays, bPricePerDay);
}
