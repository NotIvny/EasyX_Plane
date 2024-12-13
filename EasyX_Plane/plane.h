#ifndef PLANE_H
#define PLANE_H

#include <vector>
#include <windows.h>
#include <algorithm>
#include "utils.h"
#include "item.h"
#include "buff.h"
class Bullet {
public:
    int posX;
    int posY;
    bool isActive;
    Bullet(int x, int y) : posX(x), posY(y), isActive(true) {}
};

class Plane {
public:
    int posX;
    int posY;
    double HP;
    double maxHP;
    double damage;
    int attackSpeed;
    int bulletType;
    int level;
    int exp;
    std::vector<Bullet> bullets;
    std::vector<Buff> buffs;
    int lastBulletTime;
    int lastMoveBulletTime;
    Plane() : posX(0), posY(0), lastBulletTime(0), lastWTime(0), lastSTime(0), lastATime(0), lastDTime(0), bulletType(0), maxHP(100), level(1) {}
    const int FIRE_RATE_LIMIT = MAXFPS / 10;
    const int KEY_RESPONSE_INTERVAL = MAXFPS / 20;
    const int BULLET_MOVE_INTERVAL = MAXFPS / 60;
    void showPlane(int x, int y);
    void fireBullet();
    void moveBullets();
    void handleMovement();
    void RefreshBuffs();
    void pickupItem(std::vector<Item>& items);
    void showBuffInfo();
    void levelup();
    int lastWTime;
    int lastSTime;
    int lastATime;
    int lastDTime;
};

#endif
