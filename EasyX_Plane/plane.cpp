#include "plane.h"
#include "utils.h"
#include <chrono>
#include <windows.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <graphics.h>
#include "image.h"
#include "item.h"
#include "buff.h"
void Plane::fireBullet() {
    int currentTime = GetTickNow();
    switch (bulletType) {
        case 0:
            if (currentTime - lastBulletTime >= FIRE_RATE_LIMIT) {
                bullets.push_back(Bullet(posX, posY));
                lastBulletTime = currentTime;
            }
            break;
        case 1:
            if (currentTime - lastBulletTime >= FIRE_RATE_LIMIT) {
                bullets.push_back(Bullet(posX - 6, posY));
                bullets.push_back(Bullet(posX + 6, posY));
                lastBulletTime = currentTime;
            }
            break;
    }
}

void Plane::moveBullets() {
    int currentTime = GetTickNow();
    if (currentTime - lastMoveBulletTime >= BULLET_MOVE_INTERVAL){
        for (auto& bullet : bullets) {
            if (bullet.isActive) {
                bullet.posY -= 12;
                if (bullet.posY < 0)
                    bullet.isActive = false;
            }
        }
        lastMoveBulletTime = currentTime;
    }
}

void Plane::handleMovement() {
    int currentTime = GetTickNow();
    //按键按下判断，每个按键独立
    //如果坐标超出范围则不移动。
    if ((GetAsyncKeyState('W') & 0x8000) && (currentTime - lastWTime >= KEY_RESPONSE_INTERVAL)) {
        if (posY > 20) posY -= 720 / MAXFPS;
        lastWTime = currentTime;
    }
    if ((GetAsyncKeyState('S') & 0x8000) && (currentTime - lastSTime >= KEY_RESPONSE_INTERVAL)) {
        if (posY < MAX_Y - 40) posY += 720 / MAXFPS;
        lastSTime = currentTime;
    }
    if ((GetAsyncKeyState('A') & 0x8000) && (currentTime - lastATime >= KEY_RESPONSE_INTERVAL)) {
        if (posX > 0) posX -= 720 / MAXFPS;
        lastATime = currentTime;
    }
    if ((GetAsyncKeyState('D') & 0x8000) && (currentTime - lastDTime >= KEY_RESPONSE_INTERVAL)) {
        if (posX < MAX_X - 20) posX += 720 / MAXFPS;
        lastDTime = currentTime;
    }
}
void Plane::showPlane(int x, int y){
    putimage(x - 20, y, &plane_background_, SRCAND);
    putimage(x - 20, y, &plane_, SRCPAINT);
    //绘制血量
    setfillcolor(BLACK);
    bar(posX - 20, posY + 50, posX + 30, posY + 55);
    setfillcolor(RED);
    int healthBarWidth = (HP * 50) / maxHP; 
    bar(posX - 20, posY + 50, posX - 20 + healthBarWidth, posY + 55);
    //绘制经验
    setfillcolor(BLACK);
    bar(posX - 20, posY + 60, posX + 30, posY + 65);
    setfillcolor(YELLOW);
    int healthBarWidth1 = (exp * 50) / (90 + level * 10);
    bar(posX - 20, posY + 60, posX - 20 + healthBarWidth1, posY + 65);
}

void Plane::pickupItem(std::vector<Item>& items) {
    for (auto& item : items) {
        //判断飞机与道具的距离
        if (abs(posX - item.posX) <= 24 && abs(posY - item.posY) <= 24) {
            //播放拾取音效
            mciSendString(_T("open ./resources/pickup.mp3 alias pickup"), NULL, 0, NULL);
            mciSendString(_T("seek pickup to 70"), NULL, 0, NULL);
            mciSendString(_T("play pickup"), NULL, 0, NULL);
            pickupbgm = GetTickNow();
            bool found = false;
            //判断是否已有该buff
            for (auto& buff : buffs) {
                if (buff.type == item.type) {
                    buff.endTime = GetTickNow() + buff.endTime - buff.startTime;
                    buff.startTime = GetTickNow();
                    found = true;
                    break;
                }
            }
            //判断物品类型
            if (!found) {
                switch (item.type) {
                    case 0:
                        buffs.push_back(Buff(GetTickNow(), GetTickNow() + MAXFPS * 15, item.type));
                        break;
                    case 1:
                        buffs.push_back(Buff(GetTickNow(), GetTickNow() + MAXFPS * 8, item.type));
                        break;
                }
                
            }
            item.isActive = false;
        }
    }
}

void Plane::RefreshBuffs(){
    for (auto& buff : buffs) {
        if (GetTickNow() > buff.endTime) {
            switch (buff.type) {
                case 0:
                    bulletType = 0;
                    break;
                }
            buff.isActive = false;
        }
        else {
            switch (buff.type) {
                case 0:
                    bulletType = 1;
                    break;
                case 1:
                    HP += 3 / double(MAXFPS);
                    if (HP > maxHP) {
                        HP = maxHP;
                    }
            }
        }
        buffs.erase(std::remove_if(buffs.begin(), buffs.end(), [](const Buff& buff) { return !buff.isActive; }), buffs.end());
    }
}
//显示飞机拥有的buff
void Plane::showBuffInfo() {
    int count = 0;
    for (auto& buff : buffs) {
        int timeLeft = 0;
        switch (buff.type) {
        //根据buff类型输出不同内容
        case 0:
            timeLeft = buff.endTime - GetTickNow();
            if (timeLeft > 0) {
                double time = timeLeft / double(MAXFPS);
                //显示在状态栏上的文字,unicode编码
                TCHAR* str = new TCHAR[128];
                _stprintf_s(str, 128, _T("\u5B50\u5F39\u5F3A\u5316\uFF1A %.1f s"), time);
                outtextxy(470, 320 + 30 * count, str);
                count++;
            }
            break;
        case 1:
            timeLeft = buff.endTime - GetTickNow();
            if (timeLeft > 0) {
                double time = timeLeft / double(MAXFPS);
                //显示在状态栏上的文字,unicode编码
                TCHAR* str = new TCHAR[128];
                _stprintf_s(str, 128, _T("\u751F\u547D\u6062\u590D\uFF1A %.1f s"), time);
                outtextxy(470, 320 + 30 * count, str);
                count++;
            }
            break;
        } 
    }
}
void Plane::levelup() {
    if (exp > 90 + level * 10) {
        exp -= 90 + level * 10;
        level++;
        HP += 20;
        maxHP += 5;
        if (HP > maxHP) {
            HP = maxHP;
        }
        damage += 0.03 + level * 0.0025;
    }
}