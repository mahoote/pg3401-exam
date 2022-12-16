#ifndef __RESERVATION_H__
#define __RESERVATION_H__

/* Structs */
typedef struct reservation
{
    char pszName[32];
    char pszRoomNumber[8];
    int iDate;
    int iNumDays;
    float bPricePerDay;

    struct reservation *pPrev;
    struct reservation *pNext;
} Reservation;

/* Variables */
extern Reservation *head;

/* Methods */
void printReservation(Reservation *res);
void addReservation(Reservation **ppHead, char *_pszName, char *_pszRoomNumber, int _iDate, int _iNumDays, float _bPricePerDay);
Reservation *newReservation(char *_pszName, char *_pszRoomNumber, int _iDate, int _iNumDays, float _bPricePerDay);
void printAllReservations();

#endif