#include "enemy.h"
#include "utils.h"
#include <iostream>
#include <cmath>
#include <windows.h>
#include <graphics.h>
#include "image.h"
#define PI 3.1415926
int Enemy::spawnInterval = 2 * MAXFPS;

Enemy::Enemy() : posX(0), posY(0), isActive(false), lastBulletTime(0), lastMoveTime(0) {}

void Enemy::spawn() {
    posX = rand() % (MAX_X - 40) + 20;
    posY = 0;
    double time = GetTickNow() / MAXFPS;
    int randomInt = rand() % 100;
    if (randomInt < 50) {
        //飞机属性设置
        putimage(posX - 12, posY - 12, &enemy_, SRCINVERT);
        isActive = true;
        lastBulletTime = GetTickNow();
        lastMoveTime = GetTickNow();
        maxHP = 10 + 0.045 * time + 0.000030 * pow(time, 2);
        HP = maxHP;
        damage = (10 + 0.03 * time + 0.000015 * pow(time, 2));
        type = 0;
        speed = 3;
        bulletSpawnInterval = MAXFPS;
    }
    else {
        //飞机属性设置
        putimage(posX - 12, posY - 12, &enemy_, SRCINVERT);
        isActive = true;
        lastBulletTime = GetTickNow();
        lastMoveTime = GetTickNow();
        maxHP = (10 + 0.045 * time + 0.000030 * pow(time, 2)) * 2.5;
        HP = maxHP;
        damage = (10 + 0.03 * time + 0.000015 * pow(time, 2)) * 0.4;
        type = 1;
        speed = 1;
        bulletSpawnInterval = 3 * MAXFPS;
    }

    // 下一次的敌机生成时间在-20%~20%之间浮动，增加随机性，防止大量子弹同时发射
    spawnInterval = getRand(3 * MAXFPS, -20, 20);
}

void Enemy::draw() {
    if (isActive) {
        switch (type) {
            case 0:
                putimage(posX - 18, posY, &enemy_, SRCINVERT);
                break;
            case 1:
                putimage(posX - 18, posY, &enemy_1_, SRCINVERT);
                break;
        }
        setfillcolor(BLACK);
        bar(posX - 20, posY - 10, posX + 30, posY - 5);  // 绘制血条背景
        // 画血量
        setfillcolor(RED);
        int healthBarWidth = (HP * 50) / maxHP;  // 根据血量计算血条长度
        bar(posX - 20, posY - 10, posX - 20 + healthBarWidth, posY - 5);
    }
}

void Enemy::fireBullet(int x, int y) {
    int baseInterval = bulletSpawnInterval;
    // 敌人子弹发射时间-20%~20%随机浮动
    double currentTime = GetTickNow();
    int randomInterval = getRand(baseInterval, -20, 20);

    if (currentTime - lastBulletTime > randomInterval) {
        switch (type) {
            case 0: {
                //子弹属性设置
                EnemyBullet newBullet;
                newBullet.damage = damage;
                newBullet.posX = posX;
                newBullet.posY = posY + 10;
                double deltaX = x - newBullet.posX;
                double deltaY = y - newBullet.posY;
                double distance = sqrt(deltaX * deltaX + deltaY * deltaY);
                double speed_X = (deltaX / distance) * 4;
                double speed_Y = (deltaY / distance) * 4;
                newBullet.isActive = true;
                newBullet.speed_X = speed_X;
                newBullet.speed_Y = speed_Y;
                bullets.push_back(newBullet);
                lastBulletTime = currentTime;
                break;
            }
            case 1: {
                //子弹属性设置
                const int bulletCount = 9;
                const double speed = 2;
                const double angleStep = 360.0 / bulletCount;
                for (int i = 0; i < bulletCount; ++i) {
                    double angle = i * angleStep * PI / 180.0;  // 转换为弧度
                    EnemyBullet newBullet;
                    newBullet.damage = damage;
                    newBullet.posX = posX;
                    newBullet.posY = posY;
                    newBullet.isActive = true;
                    newBullet.speed_X = cos(angle) * speed;
                    newBullet.speed_Y = sin(angle) * speed;
                    bullets.push_back(newBullet);
                }
                lastBulletTime = currentTime;
                break;
            }
                
        }
        
    }
}

void Enemy::move() {
    int currentTime = GetTickNow();
    if (currentTime - lastMoveTime > 1) {
        posY += speed;
        if (posY >= MAX_Y) {
            isActive = false;
        }
        lastMoveTime = currentTime;
    }
}
