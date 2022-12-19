#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/reservation.h"
#include "include/timedate.h"

Reservation *pHead = NULL;

/* printReservation() -------------------------------
    Revision    : 1.0.0

    Comments:
    Prints all the info regarding a reservation.
   -------------------------------------------------- */
void printReservation(Reservation *_pRes)
{
    printf("Name: %s\n", _pRes->szName);
    printf("Room number: %s\n", _pRes->szRoomNumber);
    printf("Date: %d\n", _pRes->iDate);
    printf("Number of days: %d\n", _pRes->iNumDays);
    printf("Price per day: %.2f\n", _pRes->flPricePerDay);
    printf("\n");
}

/* printReservationMin() ----------------------------
    Revision    : 1.0.0

    Comments:
    A minimal version of printReservation().
   -------------------------------------------------- */
void printReservationMin(Reservation *_pRes)
{
    printf("Name: %s\n", _pRes->szName);
    printf("Room number: %s\n", _pRes->szRoomNumber);
    printf("Number of days: %d\n", _pRes->iNumDays);
    printf("\n");
}

/* printAllReservations() ---------------------------
    Revision    : 1.0.0
   -------------------------------------------------- */
int printAllReservations()
{
    if (pHead == NULL)
    {
        return 1;
    }

    Reservation *pCurrent = pHead;

    while (pCurrent != NULL)
    {
        printReservation(pCurrent);
        pCurrent = pCurrent->pNext;
    }

    return 0;
}

/* printAllReservationsByDate() ---------------------
    Revision    : 1.0.0

    Comments:
    Uses the head reservation to traverse the linked list
    and if the element's date is today's date, print the
    reservation with minimal details (name, room, numDays).
   -------------------------------------------------- */
int printAllReservationsByCurrentDate()
{
    if (pHead == NULL)
    {
        return 1;
    }

    int iToday, iReservationCount = 0;
    Reservation *pCurrent = pHead;

    getCurrentDate(&iToday);

    while (pCurrent != NULL)
    {
        int *iDate = &pCurrent->iDate;

        if (iToday == *iDate)
        {
            iReservationCount++;
            printReservationMin(pCurrent);
        }

        pCurrent = pCurrent->pNext;
    }

    if (iReservationCount > 0)
        return 0;

    return 2;
}

/* addReservation() ---------------------------------
    Revision    : 1.0.0

    Comments:
    Uses the head reservation to traverse the linked list
    and add the new one to the end. If there is no head,
    the new reservation will become it.
    _iDate format = YYYYMMDD.
   -------------------------------------------------- */
void addReservation(char *_pszName, char *_pszRoomNumber, int _iDate, int _iNumDays, float _flPricePerDay)
{
    Reservation *pNewRes = newReservation(_pszName, _pszRoomNumber, _iDate, _iNumDays, _flPricePerDay);

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
    _iDate format = YYYYMMDD.
   -------------------------------------------------- */
Reservation *newReservation(char *_pszName, char *_pszRoomNumber, int _iDate, int _iNumDays, float _flPricePerDay)
{
    Reservation *pRes = (Reservation *)malloc(sizeof(Reservation));

    strcpy(pRes->szName, _pszName);
    strcpy(pRes->szRoomNumber, _pszRoomNumber);
    pRes->iDate = _iDate;
    pRes->iNumDays = _iNumDays;
    pRes->flPricePerDay = _flPricePerDay;

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

    Reservation *pPrev = pCurrent->pPrev;
    free(pCurrent);
    pCurrent = pPrev;
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
    if (pHead == NULL)
        return;

    Reservation *pCurrent = pHead;
    while (pCurrent != NULL)
    {
        Reservation *pNext = pCurrent->pNext;
        free(pCurrent);
        pCurrent = pNext;
    }
}

/* deleteExpiredReservations() ----------------------
    Revision    : 1.0.0

    Comments:
    If the list is empty, return immediately.
    Else go through the list and delete the expired reservations.
    Frees the memory. If the current has a previous and a next,
    connect the two, making the previous's next the current's next and vice versa.
    If the current element is the head, only update the head.
   -------------------------------------------------- */
void deleteExpiredReservations(int *_iReservationCount)
{
    int iToday;
    getCurrentDate(&iToday);

    if (pHead == NULL)
        return;

    Reservation *pCurrent = pHead;
    *_iReservationCount = 0;

    while (pCurrent != NULL)
    {
        Reservation *pNext = pCurrent->pNext;

        int iEndDate = pCurrent->iDate;
        addDays(&iEndDate, pCurrent->iNumDays);

        if (iEndDate < iToday)
        {
            ++*_iReservationCount;

            if (pCurrent->pPrev != NULL)
            {
                pCurrent->pPrev->pNext = pCurrent->pNext;
            }
            else
            {
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

/* getReservationByName() ---------------------------
    Revision    : 1.0.0

    Comments:
    If the list is empty, return immediately.
    Else go through the list and compare reservation name with input.
    Return if the guest is found and print to terminal. Else if not found
    print "not found" message. Returns the first reservation where the name
    occurs.
   -------------------------------------------------- */
int getReservationByName(char *_pszName, Reservation **_pRes)
{
    if (pHead == NULL)
    {
        return 1;
    }

    Reservation *pCurrent = pHead;

    while (pCurrent != NULL)
    {
        if (strcmp(pCurrent->szName, _pszName) == 0)
        {
            *_pRes = pCurrent;
            return 0;
        }
        else
        {
            pCurrent = pCurrent->pNext;
        }
    }

    return 2;
}

/* sumBookingPricesByDate() -------------------------
    Revision    : 1.0.0

    Comments:
    Uses the head reservation to traverse the linked list
    and find elements where the input date is between reservation
    start and end date. If the date is between the values, the price per day
    is added to the sum.
   -------------------------------------------------- */
int sumBookingPricesByDate(int _iDate, int *_iSum)
{
    *_iSum = 0;

    if (pHead == NULL)
    {
        return 1;
    }

    Reservation *pCurrent = pHead;

    while (pCurrent != NULL)
    {
        int *iDate = &pCurrent->iDate;
        int iEndDate = *iDate;
        addDays(&iEndDate, pCurrent->iNumDays);

        if (_iDate >= *iDate && _iDate <= iEndDate)
        {
            *_iSum += pCurrent->flPricePerDay;
        }

        pCurrent = pCurrent->pNext;
    }

    return 0;
}
