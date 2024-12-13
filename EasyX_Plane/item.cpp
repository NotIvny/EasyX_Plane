#include "item.h"
#include <algorithm>
#include "utils.h"
#include "image.h"
std::vector<Item> items;

Item::Item(int x, int y, int type) {
    posX = x;
    posY = y;
    this->type = type;
    isActive = true;
}
//移动道具
void Item::move() {
    if (isActive) {
        posY += 1;
        if (posY >= MAX_Y) {
            isActive = false;
        }
    }
}
//绘图
void Item::draw() {
    if (isActive) {
        switch (type) {
            case 0:
                putimage(posX, posY, &double_bullet_, SRCINVERT);
                break;
            case 1:
                putimage(posX, posY, &health_, SRCINVERT);
                break;
        }
        
    }
}
//掉落道具
void dropitem(int x, int y) {
    int randomInt = rand() % 100;
    if (randomInt < 10) {
        Item newItem(x, y, 0);
        newItem.draw();
        items.push_back(newItem);
    }else if (randomInt < 20) {
        Item newItem(x, y, 1);
        newItem.draw();
        items.push_back(newItem);
    }
}
//移动道具
void moveItems() {
    for (auto& item : items) {
        item.move();
    }
    items.erase(std::remove_if(items.begin(), items.end(), [](const Item& item) { return !item.isActive; }), items.end());
}
//绘图
void drawItems() {
    for (auto& item : items) {
        item.draw();
    }
}
