#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>
#include <cstring>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <sstream>
#include <graphics.h>
#include <tchar.h>
#include "plane.h"
#include "enemy.h"
#include "utils.h"
#include "image.h"
#include "item.h"
#include "buff.h"
#pragma execution_character_set("utf-8") 
#pragma comment(lib,"Winmm.lib")
using namespace std;
vector<Enemy> enemies;
vector<EnemyBullet> enemyBullets;
int lastSpawnTime = 0;
int lastBulletTime = 0;
int currentTime = 0;
int lastTimeShowFPS = -5;
int lastFPS = 0;
int score = 0;
int lastTimeInit = 0;
int lastTimePause = 0;
double FPS = 0;
LOGFONT font;
Plane plane;
void init() {
    if (GetTickCount() - lastTimeInit > 1000) {
        enemies.clear();
        enemyBullets.clear();
        tick = 0;
        plane.posX = MAX_X / 2;
        plane.posY = MAX_Y / 2;
        plane.damage = 1;
        plane.HP = 100;
        plane.maxHP = 100;
        plane.level = 1;
        plane.exp = 0;
        plane.lastBulletTime = 0;
        plane.lastMoveBulletTime = 0;
        plane.lastWTime = 0;
        plane.lastATime = 0;
        plane.lastDTime = 0;
        plane.lastSTime = 0;
        score = 0;
        items.clear();
        plane.bullets.clear();
        plane.buffs.clear();
        enemyLeft = 500000;
        lastTimeInit = GetTickCount();
        currentTime = 0;
        lastSpawnTime = 0;
    }
    
}
void displayScore(int score, double planeHealth , double maxHealth, double exp, int level) {
    settextstyle(&font);
    //在屏幕上输出文字
    TCHAR* str = new TCHAR[128];
    _stprintf_s(str, 128, _T("积分：%d"), score);
    outtextxy(470, 50, str);
    //5帧显示一次帧数，显示太快的话看不清
    if (GetTickNow() - lastTimeShowFPS >= 5) {
        _stprintf_s(str, 128, _T("FPS：%d"), int(FPS > 60 ? 60 : FPS));
        outtextxy(470, 80, str);
        lastTimeShowFPS = GetTickNow();
        lastFPS = FPS;
    }
    else {
        _stprintf_s(str, 128, _T("FPS：%d"), int(lastFPS > MAXFPS ? MAXFPS : lastFPS));
        outtextxy(470, 80, str);
    }
    _stprintf_s(str, 128, _T("生命值：%d / %d"), int(planeHealth), int(maxHealth));
    outtextxy(470, 230, str);
    _stprintf_s(str, 128, _T("经验：%d / %d"), int(exp), int(90 + 10 * level));
    outtextxy(470, 260, str); 
    _stprintf_s(str, 128, _T("等级：%d"), level);
    outtextxy(470, 290, str);
    _stprintf_s(str, 128, _T("按键帮助："));
    outtextxy(470, 450, str);
    _stprintf_s(str, 128, _T("操控飞机：WASD"));
    outtextxy(470, 480, str);
    _stprintf_s(str, 128, _T("发射子弹：空格"));
    outtextxy(470, 510, str);
    _stprintf_s(str, 128, _T("暂停/继续：ESC"));
    outtextxy(470, 540, str);
    _stprintf_s(str, 128, _T("退出：Q"));
    outtextxy(470, 570, str);
    _stprintf_s(str, 128, _T("重玩：R"));
    outtextxy(470, 600, str);
}
void resethitbgm() {
    if ((GetTickNow() - hitbgm) > MAXFPS) {
        mciSendString(_T("close hit"), NULL, 0, NULL);
    }
}
void resetpickupbgm() {
    if ((GetTickNow() - pickupbgm) > MAXFPS) {
        mciSendString(_T("close pickup"), NULL, 0, NULL);
    }
}
bool isEnemyInactive(const Enemy& enemy) {
    return !enemy.isActive;
}
bool isEnemyBulletInactive(const EnemyBullet& bullet) {
    return !bullet.isActive;
}
int main() {
    initgraph(Width, Height);
    init();
    loadImages();
    mciSendString(_T("open ./resources/bgm.mp3 alias bkmusic"), NULL, 0, NULL);
    mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);
    SetConsoleOutputCP(CP_UTF8);
    //初始化飞机
    init();
    double enemyHealth = 10;
    double enemyDamage = 10;
    
    //隐藏光标
    HideCursor();
    //时间戳
    lastSpawnTime = GetTickNow();
    lastBulletTime = GetTickNow();
    
    //生成随机数
    srand(static_cast<unsigned>(time(0)));
    while (true) {
        auto loopStartTime = std::chrono::high_resolution_clock::now();
        BeginBatchDraw();
        cleardevice();
        putimage(0, 0, &background_);
        putimage(440, 0, &background_right_);
        //游戏内tick计数
        tick++;
        displayScore(score, plane.HP, plane.maxHP, plane.exp, plane.level);
        plane.showPlane(plane.posX, plane.posY);
        plane.handleMovement();
        plane.pickupItem(items);
        if (GetAsyncKeyState('Q') & 0x8000) {
            return 0;
        }
        if (GetAsyncKeyState('R') & 0x8000) {
            init();
        }
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            plane.fireBullet();
        }
        double time = GetTickNow() / MAXFPS;
        enemyHealth = 10 + 0.05 * time + 0.000015 * pow(time, 2);
        enemyDamage = 10 + 0.03 * time + 0.000010 * pow(time, 2);
        //生成敌机
        currentTime = GetTickNow();
        if (currentTime - lastSpawnTime >= Enemy::spawnInterval) {
            if (enemyLeft > 0) {
                Enemy newEnemy;
                newEnemy.spawn();
                enemyLeft -= 1;
                enemies.push_back(newEnemy);
            }
            lastSpawnTime = currentTime;
        }
        
        //显示敌人
        for (auto& enemy : enemies) {
            if (enemy.isActive) {
                enemy.move();
                enemy.draw();
                enemy.fireBullet(plane.posX, plane.posY);
            }
            //碰撞检测
            if (abs(enemy.posX - plane.posX) <= 24 && abs(enemy.posY - plane.posY) <= 24) {
                plane.HP -= 0.3;
            }
            for (auto& bullet : enemy.bullets) {
                enemyBullets.push_back(bullet);
            }
            enemy.bullets.clear();
        }

        //显示子弹
        for (auto& bullet : enemyBullets) {
            bullet.move();
            if (bullet.isActive) {
                if (bullet.posX < 0 || bullet.posX > MAX_X || bullet.posY < 0 || bullet.posY > MAX_Y) {
                    bullet.isActive = false;
                    continue;
                }
                putimage(bullet.posX, bullet.posY, &enemy_bullet_, SRCINVERT);
                if (abs(bullet.posX - plane.posX) <= 18 && abs(bullet.posY - plane.posY) <= 12) {
                    plane.HP -= bullet.damage;
                    bullet.isActive = false;
                }
            }
        }

        
        //我方飞机子弹移动
        plane.moveBullets();

        
        resethitbgm();
        resetpickupbgm();
        //子弹击中判断
        for (auto& bullet : plane.bullets) {
            if (bullet.isActive) {
                putimage(bullet.posX, bullet.posY, &plane_bullet, SRCINVERT);
                for (auto& enemy : enemies) {
                    if (enemy.isActive && abs(bullet.posX - enemy.posX) <= 24 && abs(enemy.posY - bullet.posY) <= 12) {
                        enemy.HP -= plane.damage;
                        bullet.isActive = false;
                        //敌人死亡
                        if (enemy.HP <= 0) {
                            mciSendString(_T("open ./resources/hit.mp3 alias hit"), NULL, 0, NULL);
                            mciSendString(_T("seek hit to 100"), NULL, 0, NULL);
                            mciSendString(_T("play hit"), NULL, 0, NULL);
                            hitbgm = GetTickNow();
                            dropitem(enemy.posX, enemy.posY);
                            plane.exp += enemy.maxHP;
                            plane.levelup();
                            enemy.isActive = false;
                            score++;
                        }
                    }
                }
            }
        }

        //移除敌机
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), isEnemyInactive), enemies.end());

        //移除子弹
        enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(), isEnemyBulletInactive), enemyBullets.end());
        moveItems();
        drawItems();
        plane.RefreshBuffs();
        plane.showBuffInfo();
        if (plane.HP <= 0) {
            TCHAR* str = new TCHAR[128];
            putimage(140, 100, &gameover_text_, SRCINVERT);
            setbkmode(TRANSPARENT);
            settextstyle(35, 0, _T("微软雅黑"));
            _stprintf_s(str, 128, _T("得分：%d"), score);
            outtextxy(200, 250, str);
            putimage(120, 300, &gameover_);
            _stprintf_s(str, 128, _T("按Q退出"));
            outtextxy(200, 320, str);
            putimage(120, 400, &restart_);
            _stprintf_s(str, 128, _T("按R重玩"));
            outtextxy(200, 420, str);
            FlushBatchDraw();
            while (true) {
                Sleep(1000 / double(MAXFPS));
                if (GetAsyncKeyState('Q') & 0x8000) {
                    return 0;
                }
                if (GetAsyncKeyState('R') & 0x8000) {
                    init();
                    break;
                }
            }
        }
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000 && GetTickCount() - lastTimePause > 200) {
            lastTimePause = GetTickCount();
            putimage(150, 300, &pause_background_, SRCAND);
            putimage(150, 300, &pause_, SRCINVERT);
            
            FlushBatchDraw();
            while (true) {
                Sleep(1000 / double(MAXFPS));
                if (GetAsyncKeyState(VK_ESCAPE) & 0x8000 && GetTickCount() - lastTimePause > 200) {
                    lastTimePause = GetTickCount();
                    break;
                }
            }
        }
        FlushBatchDraw();
        //帧率控制，sleep()精度太低，使用chrono可以稳定控制帧率
        auto loopEndTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsedTime = loopEndTime - loopStartTime;
        FPS = 1000 / elapsedTime.count();
        if (elapsedTime.count() < 1000 / double(MAXFPS)) {
            auto nextFrameTime = loopStartTime + std::chrono::duration<double, std::milli>(1000 / double(MAXFPS));
            while (std::chrono::high_resolution_clock::now() < nextFrameTime) {
            }
        }
        
    }
    return 0;
}
