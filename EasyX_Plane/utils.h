#ifndef UTILS_H
#define UTILS_H
#include <windows.h>
#define MAX_X 420
#define MAX_Y 660
#define MAXFPS 60
#define Width 660
#define Height 660
extern int tick;
extern int hitbgm;
extern int pickupbgm;
extern int enemyLeft;
int getRand(int baseValue, int lower, int upper);
int GetTickNow();

void gotoXY(int x, int y);

void HideCursor();

#endif 
