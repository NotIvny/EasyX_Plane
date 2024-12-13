#include "enemy_bullet.h"
#include "utils.h"
EnemyBullet::EnemyBullet() : posX(0), posY(0), isActive(false), lastMoveTime(GetTickNow()) {}

void EnemyBullet::move() {
    if (isActive) {
        int currentTime = GetTickNow();
        if (currentTime - lastMoveTime >= MAXFPS / 60) {
            posX += speed_X;
            posY += speed_Y;
            lastMoveTime = currentTime;
            if (posY >= MAX_Y) {
                isActive = false;
            }
        }
    }
}