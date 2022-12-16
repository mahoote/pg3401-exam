#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/reservation.h"

Reservation *head = NULL;

/* - printReservation() -------------------------------
    Revision    : 1.0.0

    Comments:
    Prints all the info regarding a reservation.
   -------------------------------------------------- */
void printReservation(Reservation *res)
{
    printf("Name: %s\n", res->pszName);
    printf("Room number: %s\n", res->pszRoomNumber);
    printf("Date: %d\n", res->iDate);
    printf("Number of days: %d\n", res->iNumDays);
    printf("Price per day: %.2f\n", res->bPricePerDay);
    printf("\n");
}

/* - addReservation() -------------------------------
    Revision    : 1.0.0

    Comments:
    Uses the head reservation to traverse the linked list
    and add the new one to the end. If there is no head,
    the new reservation will become it.
   -------------------------------------------------- */
void addReservation(Reservation **ppHead, char *_pszName, char *_pszRoomNumber, int _iDate, int _iNumDays, float _bPricePerDay)
{
    Reservation *pNewRes = newReservation(_pszName, _pszRoomNumber, _iDate, _iNumDays, _bPricePerDay);

    if (*ppHead == NULL)
    {
        *ppHead = pNewRes;
    }
    else
    {
        Reservation *pCurrent = *ppHead;
        while (pCurrent->pNext != NULL)
        {
            pCurrent = pCurrent->pNext;
        }

        pCurrent->pNext = pNewRes;
        pNewRes->pPrev = pCurrent;
    }
}

/* - newReservation() -------------------------------
    Revision    : 1.0.0

    Comments:
    Return a pointer to the new reservation.
   -------------------------------------------------- */
Reservation *newReservation(char *_pszName, char *_pszRoomNumber, int _iDate, int _iNumDays, float _bPricePerDay)
{
    Reservation *res = (Reservation *)malloc(sizeof(Reservation));

    // Copy the provided information into the reservation struct
    strcpy(res->pszName, _pszName);
    strcpy(res->pszRoomNumber, _pszRoomNumber);
    res->iDate = _iDate;
    res->iNumDays = _iNumDays;
    res->bPricePerDay = _bPricePerDay;

    // Set the pointers to NULL
    res->pPrev = NULL;
    res->pNext = NULL;

    return res;
}

/* - printAllReservations() -------------------------------
    Revision    : 1.0.0
   -------------------------------------------------- */
void printAllReservations()
{
    printf("Linked list:\n");
    Reservation *current = head;
    while (current != NULL)
    {
        printReservation(current);
        current = current->pNext;
    }
}