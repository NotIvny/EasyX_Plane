#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>
#include <cstring>
#include <algorithm>
#include "utils.h"
int tick = 0;
int hitbgm = 0;
int pickupbgm = 0;
int enemyLeft = 500000;
int getRand(int baseValue, int lower, int upper) {
    double variance = (rand() % (upper - lower + 1) + lower) / 100.0;
    return static_cast<int>(baseValue * (1 + variance));
}
int GetTickNow() {
    return tick;
}
void HideCursor() {
    CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}