#ifndef ITEM_H
#define ITEM_H

#include <vector>
#include <cstdlib>
#include <ctime>
#include <graphics.h>
#include "utils.h"
class Item {
public:
    int posX;
    int posY;
    int type;
    bool isActive;
    Item(int x, int y, int type);
    void move();
    void draw();
};

extern std::vector<Item> items;

void dropitem(int x, int y);
void moveItems();
void drawItems();

#endif
