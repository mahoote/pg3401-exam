#ifndef __MENU_H__
#define __MENU_H__

/* Macros */
#define READ_INPUT_MAX_LENGTH 256

/* Methods */
int readInput(char *pszInput, char *pszPrompt, void *pVar, const char *pcszFormat, int iMaxLength);
void printMenu();
void addReservationOption();
void deleteLastElementOption();

#endif