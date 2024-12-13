#include <graphics.h>
#include "image.h"
IMAGE background_;
IMAGE plane_bullet;
IMAGE plane_;
IMAGE plane_background_;
IMAGE enemy_;
IMAGE enemy_bullet_;
IMAGE double_bullet_;
IMAGE health_;
IMAGE pause_;
IMAGE background_right_;
IMAGE gameover_;
IMAGE restart_;
IMAGE gameover_text_;
IMAGE pause_background_;
IMAGE enemy_1_;
void loadImages() {
    loadimage(&background_, _T("./resources/background.png"), 450, 710, true);
    loadimage(&plane_background_, _T("./resources/plane_background.png"), 50, 50, true);
    loadimage(&plane_, _T("./resources/plane.png"), 50, 50, true);
    loadimage(&plane_bullet, _T("./resources/plane_bullet.png"), 11, 65, true);
    loadimage(&enemy_bullet_, _T("./resources/enemy_bullet.png"), 20, 20, true);
    loadimage(&enemy_1_, _T("./resources/enemy_1.png"), 50, 50, true);
    loadimage(&enemy_, _T("./resources/enemy.png"), 50, 50, true);
    loadimage(&double_bullet_, _T("./resources/double_bullet.png"), 30, 30, true);
    loadimage(&health_, _T("./resources/health.png"), 30, 30, true);
    loadimage(&pause_, _T("./resources/pause.png"), 307, 75, true);
    loadimage(&pause_background_, _T("./resources/pause_background.png"), 307, 75, true);
    loadimage(&background_right_, _T("./resources/background_right.png"), 284, 710, true);
    loadimage(&gameover_, _T("./resources/gameover.png"), 266, 78, true);
    loadimage(&restart_, _T("./resources/restart.png"), 267, 78, true);
    loadimage(&gameover_text_, _T("./resources/gameover_text.png"), 213, 132, true);
}