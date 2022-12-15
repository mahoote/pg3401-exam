#ifndef __MAIN_H__
#define __MAIN_H__

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

void printReservation(Reservation *res);
Reservation *newReservation(char *name, char *room_number, int date, int num_days, float price_per_day);
void printAllReservations();

#endif