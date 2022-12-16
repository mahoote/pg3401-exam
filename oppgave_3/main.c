#include <stdio.h>

#include "include/main.h"
#include "include/menu.h"
#include "include/reservation.h"

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
            addReservation(&head, "John Doe", "101", 16122022, 3, 100.00);
            break;
        case 6:
            printAllReservations();
            break;
        default:
            printf("\nInvalid option. Please try again.\n");
            break;
        }
    }

    return 0;
}