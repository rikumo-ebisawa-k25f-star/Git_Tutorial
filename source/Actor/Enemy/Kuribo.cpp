
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

bool Kuribo::Update(float delta_time)
{
    // 死亡状態なら更新しない
    if (Kstate == KuriboState::eDeath)
    {
        return false;
    }

    // 右から左へ移動
    Kx -= Kspeed * delta_time;

    // 画面左端を超えたら右側にリスポーン
    if (Kx < -20.0f)
    {
        Kx = 1024.0f + 20.0f;
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
