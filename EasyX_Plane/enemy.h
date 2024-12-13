#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include <windows.h>
#include "enemy_bullet.h"

class Enemy {
public:
    int posX;
    int posY;
    bool isActive;
    double HP;
    double maxHP;
    double damage;
    int lastBulletTime;
    int lastMoveTime;
    int type;
    double speed;
    int bulletSpawnInterval;
    std::vector<EnemyBullet> bullets;

    static int spawnInterval;

    Enemy();
    void spawn();
    void draw();
    void fireBullet(int x, int y);
    void move();
};

#endif
