#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/main.h"
#include "include/menu.h"

int main()
{
    int option;

    // Continue to run the program until the user selects the exit option.
    while (1)
    {
        printMenu();

        // Get the user's input
        printf("\nEnter your option: ");
        scanf("%d", &option);
        printf("- - - - - - - - -\n");

        switch (option)
        {
        case 0:
            printf("\nExiting menu...\n");
            return 0;
        case 1:
            printAllReservations();
            break;
        default:
            printf("\nInvalid option. Please try again.\n");
            break;
        }
    }

    return 0;
}

// Print a reservation to the terminal.
void printReservation(Reservation *res)
{
    printf("Name: %s\n", res->pszName);
    printf("Room number: %s\n", res->pszRoomNumber);
    printf("Date: %d\n", res->iDate);
    printf("Number of days: %d\n", res->iNumDays);
    printf("Price per day: %.2f\n", res->bPricePerDay);
    printf("\n");
}

// Create a new reservation and return a pointer to it.
Reservation *newReservation(char *name, char *room_number, int date, int num_days, float price_per_day)
{
    Reservation *res = (Reservation *)malloc(sizeof(Reservation));

    // Copy the provided information into the reservation struct
    strcpy(res->pszName, name);
    strcpy(res->pszRoomNumber, room_number);
    res->iDate = date;
    res->iNumDays = num_days;
    res->bPricePerDay = price_per_day;

    // Set the pointers to NULL
    res->pPrev = NULL;
    res->pNext = NULL;

    return res;
}

/* -----------------------------------
    Currently just an example
   ----------------------------------- */
void printAllReservations()
{
    // Create the head of the linked list.
    Reservation *head = newReservation("John Doe", "101", 20230101, 3, 100.00);

    // Create some reservations and add them to the linked list.
    Reservation *res1 = newReservation("Jane Smith", "102", 20230105, 2, 75.00);
    head->pNext = res1;
    res1->pPrev = head;

    Reservation *res2 = newReservation("Robert Johnson", "103", 20230108, 5, 125.00);
    res1->pNext = res2;
    res2->pPrev = res1;

    printf("Linked list:\n");
    Reservation *current = head;
    while (current != NULL)
    {
        printReservation(current);
        current = current->pNext;
    }
}