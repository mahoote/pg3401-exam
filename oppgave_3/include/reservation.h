#ifndef __RESERVATION_H__
#define __RESERVATION_H__

/* Structs */
typedef struct reservation
{
    char szName[32];
    char szRoomNumber[8];
    int iDate;
    int iNumDays;
    float flPricePerDay;

    struct reservation *pPrev;
    struct reservation *pNext;
} Reservation;

/* Methods */
void printReservation(Reservation *_pRes);
int printAllReservations();
int printAllReservationsByCurrentDate();
void addReservation(char *_pszName, char *_pszRoomNumber, int _iDate, int _iNumDays, float _flPricePerDay);
Reservation *newReservation(char *_pszName, char *_pszRoomNumber, int _iDate, int _iNumDays, float _flPricePerDay);
void deleteLastElement();
void deleteAllReservations();
void deleteExpiredReservations(int *_iReservationCount);
int getReservationByName(char *_pszName, Reservation **_pRes);
int sumBookingPricesByDate(int _iDate, int *_iSum);

#endif