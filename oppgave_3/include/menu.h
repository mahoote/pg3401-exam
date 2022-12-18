#ifndef __MENU_H__
#define __MENU_H__

/* Macros */
#define READ_INPUT_MAX_LENGTH 256

/* Methods */
int readInput(char *_pszInput, char *_pszPrompt, void *_pVar, const char *_pcszFormat, int _iMaxLength);
void printMenu();
void addReservationOption();
void deleteLastElementOption();
void exitMenu();
void getReservationByNameOption();

#endif