#include <stdio.h>

#include "include/menu.h"

int main()
{
    int option;

    // Continue to run the program until the user selects the exit option
    while (1)
    {
        printMenu();

        // Get the user's input
        printf("\nEnter your option: ");
        scanf("%d", &option);

        // Perform the selected option
        switch (option)
        {
        case 1:
            printf("\nHello World\n");
            break;
        case 2:
            printf("\nThe result of 2 + 2 is %d\n", 2 + 2);
            break;
        case 3:
            printf("\nExiting menu...\n");
            return 0;
        default:
            printf("\nInvalid option. Please try again.\n");
            break;
        }
    }

    return 0;
}