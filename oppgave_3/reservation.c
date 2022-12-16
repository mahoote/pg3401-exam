#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/reservation.h"

Reservation *pHead = NULL;

/* printReservation() -------------------------------
    Revision    : 1.0.0

    Comments:
    Prints all the info regarding a reservation.
   -------------------------------------------------- */
void printReservation(Reservation *_pRes)
{
    printf("Name: %s\n", _pRes->pszName);
    printf("Room number: %s\n", _pRes->pszRoomNumber);
    printf("Date: %d\n", _pRes->iDate);
    printf("Number of days: %d\n", _pRes->iNumDays);
    printf("Price per day: %.2f\n", _pRes->bPricePerDay);
    printf("\n");
}

/* printAllReservations() ---------------------------
    Revision    : 1.0.0
   -------------------------------------------------- */
void printAllReservations()
{
    printf("Linked list:\n");
    Reservation *pCurrent = pHead;

    if (pCurrent == NULL)
    {
        printf("Empty.\n");
    }

    while (pCurrent != NULL)
    {
        printReservation(pCurrent);
        pCurrent = pCurrent->pNext;
    }
}

/* addReservation() ---------------------------------
    Revision    : 1.0.0

    Comments:
    Uses the head reservation to traverse the linked list
    and add the new one to the end. If there is no head,
    the new reservation will become it.
   -------------------------------------------------- */
void addReservation(char *_pszName, char *_pszRoomNumber, int _iDate, int _iNumDays, float _bPricePerDay)
{
    Reservation *pNewRes = newReservation(_pszName, _pszRoomNumber, _iDate, _iNumDays, _bPricePerDay);

    if (pHead == NULL)
    {
        pHead = pNewRes;
    }
    else
    {
        Reservation *pCurrent = pHead;
        while (pCurrent->pNext != NULL)
        {
            pCurrent = pCurrent->pNext;
        }

        pCurrent->pNext = pNewRes;
        pNewRes->pPrev = pCurrent;
    }
}

/* newReservation() ---------------------------------
    Revision    : 1.0.0

    Comments:
    Return a pointer to the new reservation.
   -------------------------------------------------- */
Reservation *newReservation(char *_pszName, char *_pszRoomNumber, int _iDate, int _iNumDays, float _bPricePerDay)
{
    Reservation *pRes = (Reservation *)malloc(sizeof(Reservation));

    // Copy the provided information into the reservation struct
    strcpy(pRes->pszName, _pszName);
    strcpy(pRes->pszRoomNumber, _pszRoomNumber);
    pRes->iDate = _iDate;
    pRes->iNumDays = _iNumDays;
    pRes->bPricePerDay = _bPricePerDay;

    // Set the pointers to NULL
    pRes->pPrev = NULL;
    pRes->pNext = NULL;

    return pRes;
}

/* deleteLastElement() ------------------------------
    Revision    : 1.0.0

    Comments:
    If the list is empty, return immediately.
    If the list has only one element, free the memory
    and set the head to NULL. Else go to the end and free.
   -------------------------------------------------- */
void deleteLastElement()
{
    if (pHead == NULL)
        return;

    if ((pHead)->pNext == NULL)
    {
        free(pHead);
        pHead = NULL;
        return;
    }

    Reservation *pCurrent = pHead;
    while (pCurrent->pNext != NULL)
    {
        pCurrent = pCurrent->pNext;
    }

    free(pCurrent);
    pCurrent = pCurrent->pPrev;
    pCurrent->pNext = NULL;
}

/* deleteAllReservations() --------------------------
    Revision    : 1.0.0

    Comments:
    Uses the head reservation to traverse the linked list
    and delete all the reservations. Frees the memory.
   -------------------------------------------------- */
void deleteAllReservations()
{
    Reservation *pCurrent = pHead;
    while (pCurrent != NULL)
    {
        Reservation *pNext = pCurrent->pNext;
        free(pCurrent);
        pCurrent = pNext;
    }
}

void deleteExpiredReservations()
{
    // If the list is empty, return immediately
    if (pHead == NULL)
        return;

    // Go through the list and delete the expired reservations
    Reservation *pCurrent = pHead;
    while (pCurrent != NULL)
    {
        Reservation *pNext = pCurrent->pNext;
        if (pCurrent->iDate + pCurrent->iNumDays < 16122022)
        {
            // Free the memory and update the pointers
            if (pCurrent->pPrev != NULL)
            {
                pCurrent->pPrev->pNext = pCurrent->pNext;
            }
            else
            {
                // If the current element is the head of the list, update the head
                pHead = pCurrent->pNext;
            }
            if (pCurrent->pNext != NULL)
            {
                pCurrent->pNext->pPrev = pCurrent->pPrev;
            }
            free(pCurrent);
        }
        pCurrent = pNext;
    }
}
