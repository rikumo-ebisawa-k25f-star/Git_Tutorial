#include "Mario.h"

void Mario::Update()
{
    MoveX();
    MoveY();
    ApplyPhysics();
    GroundCheck();
}

void Mario::Draw()
{
    DrawBox((int)Mx - 16, (int)My - 32, (int)Mx + 16, (int)My, GetColor(255, 0, 0), TRUE);
}


void Mario::MoveX()
{
    float currentMaxSpeed = maxSpeed;

    // Xキーでダッシュ
    if (CheckHitKey(KEY_INPUT_X))
    {
        currentMaxSpeed = 4.0f; // ダッシュ時の最高速度
    }

    if (CheckHitKey(KEY_INPUT_RIGHT))
    {
        float accel = onGround ? moveAccel : airAccel;
        vx += accel;
    }
    else if (CheckHitKey(KEY_INPUT_LEFT))
    {
        float accel = onGround ? moveAccel : airAccel;
        vx -= accel;
    }
    else
    {
        if (vx > 0) vx -= moveDecel;
        else if (vx < 0) vx += moveDecel;

        if (fabs(vx) < 0.1f)
            vx = 0.0f;
    }

    // 最高速度制限
    if (vx > currentMaxSpeed)
        vx = currentMaxSpeed;

    if (vx < -currentMaxSpeed)
        vx = -currentMaxSpeed;
}
//
//void Mario::MoveY()
//{
//    if (onGround && CheckHitKey(KEY_INPUT_Z))
//    {
//        vy = jumpPower;
//        onGround = false;
//    }
//}

void Mario::MoveY()
{
    // ジャンプ開始
    if (onGround && CheckHitKey(KEY_INPUT_Z))
    {
        vy = jumpPower;
        onGround = false;
    }

    // 上昇中にボタンを離したら高さを抑える
    if (!CheckHitKey(KEY_INPUT_Z) && vy < 0)
    {
        vy *= 0.85f;
    }
}

void Mario::ApplyPhysics()
{
    vy += gravity;
    Mx += vx;
    My += vy;
}

void Mario::GroundCheck()
{
    if (My >= groundY)
    {
        My = groundY;
        vy = 0.0f;
        onGround = true;
    }
    else
    {
        onGround = false;
    }
}
