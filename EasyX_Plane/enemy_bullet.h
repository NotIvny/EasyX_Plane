#ifndef ENEMY_BULLET_H
#define ENEMY_BULLET_H

#include <windows.h>

class EnemyBullet {
public:
    double posX;
    double posY;
    bool isActive;
    int lastMoveTime;
    double damage;
    double speed_X;
    double speed_Y;
    EnemyBullet();
    void move();
};

#endif