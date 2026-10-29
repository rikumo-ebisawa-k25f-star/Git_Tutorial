
#include "Kuribo.h"
#include <DxLib.h>

Kuribo::Kuribo()
{
    Kuribo_image = -1;
    Kx = 0.0f;
    Ky = 0.0f;
    Kspeed = 0.0f;
    Kstate = KuriboState::eSave;
}

Kuribo::~Kuribo()
{
}

void Kuribo::Init(float startX, float startY, float speed)
{
    Kx = startX;
    Ky = startY;
    Kspeed = speed;
    Kstate = KuriboState::eSave;
}

bool Kuribo::Update(float delta_time, float cameraX, float screenWidth)
{
    if (Kstate == KuriboState::eDeath)
    {
        return false;
    }

    // 左方向へ移動
    Kx -= Kspeed * delta_time;

    // 画面上のX座標
    float screenX = Kx - cameraX;

    // 画面左端を超えたら、画面右端の外へリスポーン
    if (screenX < -20.0f)
    {
        Kx = cameraX + screenWidth + 20.0f;
    }

    return true;
}

void Kuribo::Draw(float cameraX)
{
    if (Kstate == KuriboState::eSave)
    {
        // ワールド座標から画面座標に変換
        int screenX = (int)(Kx - cameraX);
        int screenY = (int)Ky;

        // 画面内にいる場合のみ描画
        if (screenX >= -20 && screenX <= 1044)
        {
            DrawCircle(
                screenX,
                screenY,
                20,
                GetColor(255, 255, 255),
                TRUE
            );
        }
    }
}
