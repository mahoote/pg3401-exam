#include <stdio.h>

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
            exitMenu();
            return 0;
        case 1:
            addReservationOption();
            break;
        case 2:
            deleteLastElementOption();
        case 3:
            deleteExpiredReservations();
            break;
        case 4:
            getReservationByNameOption();
            break;
        case 6:
            printAllReservations();
            break;
        default:
            printf("Invalid option. Please try again.\n");
            break;
        }
    }

    return 0;
}