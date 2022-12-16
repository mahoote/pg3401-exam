#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/menu.h"
#include "include/reservation.h"

/* readInput() --------------------------------------
    Revision    : 1.0.0

    Comments:
    Read input and store it in a variable.
    Ends loop if the input was successfully read and stored.
    User must type in correct value.
   -------------------------------------------------- */
int readInput(char *pszInput, char *pszPrompt, void *pVar, const char *pcszFormat, int iMaxLength)
{
    while (1)
    {
        printf("%s", pszPrompt);
        fgets(pszInput, iMaxLength, stdin);

        // Remove the newline character at the end of the line
        pszInput[strcspn(pszInput, "\n")] = 0;

        if (sscanf(pszInput, pcszFormat, pVar) == 1)
            break;
        else
            printf("Invalid input. Please try again.\n");
    }
}

/* printMenu() --------------------------------------
    Revision    : 1.0.0
   -------------------------------------------------- */
void printMenu()
{
    printf("\nPlease select an option from the menu below:\n");
    printf("1. Add reservation\n");
    printf("2. Delete last reservation\n");
    printf("6. Print reservations\n");
    printf("0. Exit menu\n");
}

/* addReservationOption() ---------------------------
    Revision    : 1.0.0

    Comments:
    Reads the different reservation fields from the user.
   -------------------------------------------------- */
void addReservationOption()
{
    char pszInput[READ_INPUT_MAX_LENGTH];
    char pszName[32];
    char pszRoomNumber[8];
    int iDate;
    int iNumDays;
    float bPricePerDay;

    printf("Enter the following information:\n");

    // Consume any pending newline characters.
    int c = getchar();
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }

    readInput(pszInput, "Name: ", pszName, "%[^\n]", 32);
    readInput(pszInput, "Room number: ", pszRoomNumber, "%[^\n]", 8);
    readInput(pszInput, "Date: ", &iDate, "%d", READ_INPUT_MAX_LENGTH);
    readInput(pszInput, "Number of days: ", &iNumDays, "%d", READ_INPUT_MAX_LENGTH);
    readInput(pszInput, "Price per day: ", &bPricePerDay, "%f", READ_INPUT_MAX_LENGTH);

    addReservation(pszName, pszRoomNumber, iDate, iNumDays, bPricePerDay);
}

/* deleteLastElementOption() ------------------------
    Revision    : 1.0.0
   -------------------------------------------------- */
void deleteLastElementOption()
{
    deleteLastElement();
    printf("Last element deleted from list.\n");
}