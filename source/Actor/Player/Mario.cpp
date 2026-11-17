//#include "Mario.h"
//
//void Mario::Update()
//{
//    MoveX();
//    MoveY();
//    ApplyPhysics();
//    GroundCheck();
//}
//
//void Mario::Draw()
//{
//    DrawBox((int)Mx - 16, (int)My - 32, (int)Mx + 16, (int)My, GetColor(255, 0, 0), TRUE);
//}
//
//
//void Mario::MoveX()
//{
//    float currentMaxSpeed = maxSpeed;
//
//    // Xキーでダッシュ
//    if (CheckHitKey(KEY_INPUT_X))
//    {
//        currentMaxSpeed = 4.0f; // ダッシュ時の最高速度
//    }
//
//    if (CheckHitKey(KEY_INPUT_RIGHT))
//    {
//        float accel = onGround ? moveAccel : airAccel;
//        vx += accel;
//    }
//    else if (CheckHitKey(KEY_INPUT_LEFT))
//    {
//        float accel = onGround ? moveAccel : airAccel;
//        vx -= accel;
//    }
//    else
//    {
//        if (vx > 0) vx -= moveDecel;
//        else if (vx < 0) vx += moveDecel;
//
//        if (fabs(vx) < 0.1f)
//            vx = 0.0f;
//    }
//
//    // 最高速度制限
//    if (vx > currentMaxSpeed)
//        vx = currentMaxSpeed;
//
//    if (vx < -currentMaxSpeed)
//        vx = -currentMaxSpeed;
//}
////
////void Mario::MoveY()
////{
////    if (onGround && CheckHitKey(KEY_INPUT_Z))
////    {
////        vy = jumpPower;
////        onGround = false;
////    }
////}
//
//void Mario::MoveY()
//{
//    // ジャンプ開始
//    if (onGround && CheckHitKey(KEY_INPUT_Z))
//    {
//        vy = jumpPower;
//        onGround = false;
//    }
//
//    // 上昇中にボタンを離したら高さを抑える
//    if (!CheckHitKey(KEY_INPUT_Z) && vy < 0)
//    {
//        vy *= 0.85f;
//    }
//}
//
//void Mario::ApplyPhysics()
//{
//    vy += gravity;
//    Mx += vx;
//    My += vy;
//}
//
//void Mario::GroundCheck()
//{
//    if (My >= groundY)
//    {
//        My = groundY;
//        vy = 0.0f;
//        onGround = true;
//    }
//    else
//    {
//        onGround = false;
//    }
//}


#include "Mario.h"

#include <cmath>

Mario::Mario(float x, float y)
{
    Mx = x;
    My = y;

    vx = 0.0f;
    vy = 0.0f;

    onGround = false;
}

void Mario::Update()
{
    MoveX();
    MoveY();
    ApplyPhysics();
}

void Mario::Draw()
{
    DrawBox(
        (int)(Mx - 16),
        (int)(My - 32),
        (int)(Mx + 16),
        (int)(My),
        GetColor(255, 0, 0),
        TRUE);
}

void Mario::MoveX()
{
    float currentMaxSpeed = maxSpeed;

    if (CheckHitKey(KEY_INPUT_X))
    {
        currentMaxSpeed = dashSpeed;
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
        if (vx > 0.0f)
        {
            vx -= moveDecel;
            if (vx < 0.0f)
            {
                vx = 0.0f;
            }
        }
        else if (vx < 0.0f)
        {
            vx += moveDecel;
            if (vx > 0.0f)
            {
                vx = 0.0f;
            }
        }
    }

    if (vx > currentMaxSpeed)
    {
        vx = currentMaxSpeed;
    }

    if (vx < -currentMaxSpeed)
    {
        vx = -currentMaxSpeed;
    }
}

void Mario::MoveY()
{
    if (onGround && CheckHitKey(KEY_INPUT_Z))
    {
        vy = jumpPower;
        onGround = false;
    }

    // 可変ジャンプ
    if (!CheckHitKey(KEY_INPUT_Z) && vy < 0.0f)
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

float Mario::GetX() const
{
    return Mx;
}

float Mario::GetY() const
{
    return My;
}

float Mario::GetVX() const
{
    return vx;
}

float Mario::GetVY() const
{
    return vy;
}

void Mario::SetPos(float x, float y)
{
    Mx = x;
    My = y;
}

void Mario::SetVX(float value)
{
    vx = value;
}

void Mario::SetVY(float value)
{
    vy = value;
}

void Mario::SetGround(bool flag)
{
    onGround = flag;
}

bool Mario::IsGround() const
{
    return onGround;
}

Rect Mario::GetRect() const
{
    Rect rect;

    rect.left = Mx - 16;
    rect.top = My - 32;

    rect.right = Mx + 16;
    rect.bottom = My;

    return rect;
}