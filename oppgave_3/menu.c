#include <stdio.h>
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
int readInput(char *_pszInput, char *_pszPrompt, void *_pVar, const char *_pcszFormat, int _iMaxLength)
{
    while (1)
    {
        printf("%s", _pszPrompt);
        fgets(_pszInput, _iMaxLength, stdin);

        // Remove the newline character at the end of the line
        _pszInput[strcspn(_pszInput, "\n")] = 0;

        if (sscanf(_pszInput, _pcszFormat, _pVar) == 1)
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
    printf("3. Delete expired reservations\n");
    printf("4. Get reservation by guest name\n");
    printf("5. Sum booking prices based on date\n");
    printf("6. Print reservations\n");
    printf("0. Exit menu\n");
}

/* exitMenu() ---------------------------------------
    Revision    : 1.0.0

    Comments:
    Frees the reservations memory.
   -------------------------------------------------- */
void exitMenu()
{
    deleteAllReservations();
    printf("\nExiting menu...\n");
}

/* printAllReservationsOption() ---------------------
    Revision    : 1.0.0
   -------------------------------------------------- */
void printAllReservationsOption()
{
    printf("Reservations: \n\n");

    int iVal = printAllReservations();

    if (iVal == 1)
    {
        printf("List empty.\n");
    }
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

    printf("\nReservation added.\n");
}

/* deleteLastElementOption() ------------------------
    Revision    : 1.0.0
   -------------------------------------------------- */
void deleteLastElementOption()
{
    deleteLastElement();
    printf("Last reservation deleted.\n");
}

/* deleteExpiredReservationsOption() ----------------
    Revision    : 1.0.0
   -------------------------------------------------- */
void deleteExpiredReservationsOption()
{
    deleteExpiredReservations();
    printf("Expired reservations deleted.\n");
}

/* getReservationByNameOption -----------------------
    Revision    : 1.0.0

    Comments:
    Get input name from user and run get method.
   -------------------------------------------------- */
void getReservationByNameOption()
{
    char pszInput[READ_INPUT_MAX_LENGTH];
    char pszName[32];

    Reservation *reservation = NULL;

    // Consume any pending newline characters.
    int c = getchar();
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }

    printf("Enter the guest ");
    readInput(pszInput, "name: ", pszName, "%[^\n]", 32);

    int iVal = getReservationByName(pszName, &reservation);

    if (iVal == 0)
    {
        printf("Reservation found:\n\n");
        printReservation(reservation);
    }
    else if (iVal == 1)
    {
        printf("No reservations exist.\n");
    }
    else if (iVal == 2)
    {
        printf("The guest does not have a reservation.\n");
    }
}

void sumBookingPricesByDateOption()
{
    char pszInput[READ_INPUT_MAX_LENGTH];
    int iDate;
    int iSum;

    printf("Enter ");

    // Consume any pending newline characters.
    int c = getchar();
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }

    readInput(pszInput, "date: ", &iDate, "%d", READ_INPUT_MAX_LENGTH);

    int iVal = sumBookingPricesByDate(iDate, &iSum);

    if (iVal == 0)
    {
        if (iSum == 0)
        {
            printf("No reservations on that date.\n");
        }
        else
        {
            printf("The sum of all bookings on ");
            printf("%d ", iDate);
            printf("is:\n");
            printf("%d\n", iSum);
        }
    }
    else if (iVal == 1)
    {
        printf("No reservations exist.\n");
    }
}