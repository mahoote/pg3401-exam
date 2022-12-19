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
    printf("6. Print todays reservations\n");
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

    int iRes = printAllReservations();

    if (iRes == 1)
    {
        printf("List empty.\n");
    }
}

/* printAllReservationsOption() ---------------------
    Revision    : 1.0.0
   -------------------------------------------------- */
void printAllReservationsByCurrentDateOption()
{
    printf("Reservations: \n\n");

    int iRes = printAllReservationsByCurrentDate();

    if (iRes == 1)
    {
        printf("List empty.\n");
    }
    else if (iRes == 2)
    {
        printf("No reservations today.\n");
    }
}

/* addReservationOption() ---------------------------
    Revision    : 1.0.0

    Comments:
    Reads the different reservation fields from the user.
   -------------------------------------------------- */
void addReservationOption()
{
    char szInput[READ_INPUT_MAX_LENGTH];
    char szName[32];
    char szRoomNumber[8];
    int iDate;
    int iNumDays;
    float flPricePerDay;

    printf("Enter the following information:\n");

    // Consume any pending newline characters.
    int c = getchar();
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }

    readInput(szInput, "Name: ", szName, "%[^\n]", 32);
    readInput(szInput, "Room number: ", szRoomNumber, "%[^\n]", 8);
    readInput(szInput, "Date: ", &iDate, "%d", READ_INPUT_MAX_LENGTH);
    readInput(szInput, "Number of days: ", &iNumDays, "%d", READ_INPUT_MAX_LENGTH);
    readInput(szInput, "Price per day: ", &flPricePerDay, "%f", READ_INPUT_MAX_LENGTH);

    addReservation(szName, szRoomNumber, iDate, iNumDays, flPricePerDay);

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
    int iReservationCount;
    deleteExpiredReservations(&iReservationCount);
    printf("%d ", iReservationCount);
    printf("expired reservations deleted.\n");
}

/* getReservationByNameOption -----------------------
    Revision    : 1.0.0

    Comments:
    Get input name from user and run get method.
   -------------------------------------------------- */
void getReservationByNameOption()
{
    char szInput[READ_INPUT_MAX_LENGTH];
    char szName[32];

    Reservation *reservation = NULL;

    // Consume any pending newline characters.
    int c = getchar();
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }

    printf("Enter the guest ");
    readInput(szInput, "name: ", szName, "%[^\n]", 32);

    int iRes = getReservationByName(szName, &reservation);

    if (iRes == 0)
    {
        printf("Reservation found:\n\n");
        printReservation(reservation);
    }
    else if (iRes == 1)
    {
        printf("No reservations exist.\n");
    }
    else if (iRes == 2)
    {
        printf("The guest does not have a reservation.\n");
    }
}

void sumBookingPricesByDateOption()
{
    char szInput[READ_INPUT_MAX_LENGTH];
    int iDate;
    int iSum;

    printf("Enter ");

    // Consume any pending newline characters.
    int c = getchar();
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }

    readInput(szInput, "date: ", &iDate, "%d", READ_INPUT_MAX_LENGTH);

    int iRes = sumBookingPricesByDate(iDate, &iSum);

    if (iRes == 0)
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
    else if (iRes == 1)
    {
        printf("No reservations exist.\n");
    }
}