#include "Kuribo.h"
#include <DxLib.h>

Kuribo::Kuribo()
{
    x = 0.0f;
    y = 0.0f;
    speed = 0.0f;
    state = KuriboState::eDeath;
}

Kuribo::~Kuribo()
{

}

void Kuribo::Init(float startX, float startY, float moveSpeed)
{
    x = startX;
    y = startY;
    speed = moveSpeed;

    state = KuriboState::eSave;
}

bool Kuribo::Update(float delta_time)
{
    // 死亡していたら更新しない
    if (state == KuriboState::eDeath)
    {
        return false;  
    }

    // 右方向へ移動
    x += speed * delta_time;

    // 画面の右端を超えたら死亡
    if (x > 640.0f)
    {
        state = KuriboState::eDeath;
        return false;
    }

    return true;
}

void Kuribo::Draw()
{
    // 生存中だけ描画する
    if (state == KuriboState::eSave)
    {
        DrawCircle((int)x, (int)y, 20, GetColor(255, 0, 0), TRUE);
    }
}
