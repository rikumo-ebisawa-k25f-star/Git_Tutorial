#include "Mario.h"

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

void Mario::Draw(int cameraX)
{
    DrawBox(
        (int)(Mx - cameraX - 16),
        (int)(My - 32),
        (int)(Mx - cameraX + 16),
        (int)(My),
        GetColor(255, 0, 0),
        TRUE);
}
void Mario::MoveX()
{
    float limitSpeed = maxSpeed;

    // ダッシュ
    if (CheckHitKey(KEY_INPUT_X))
    {
        limitSpeed = dashSpeed;
    }

    // 右移動
    if (CheckHitKey(KEY_INPUT_RIGHT))
    {
        vx += onGround ? moveAccel : airAccel;
    }
    // 左移動
    else if (CheckHitKey(KEY_INPUT_LEFT))
    {
        vx -= onGround ? moveAccel : airAccel;
    }
    // 減速
    else
    {
        if (vx > 0)
        {
            vx -= moveDecel;

            if (vx < 0)
            {
                vx = 0;
            }
        }

        if (vx < 0)
        {
            vx += moveDecel;

            if (vx > 0)
            {
                vx = 0;
            }
        }
    }

    // 最高速度制限
    if (vx > limitSpeed)
    {
        vx = limitSpeed;
    }

    if (vx < -limitSpeed)
    {
        vx = -limitSpeed;
    }
}

void Mario::MoveY()
{
    // ジャンプ開始
    if (onGround &&
        CheckHitKey(KEY_INPUT_Z))
    {
        vy = jumpPower;
        onGround = false;
    }

    // 可変ジャンプ
    if (!CheckHitKey(KEY_INPUT_Z) &&
        vy < 0.0f)
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

float Mario::GetFootLeftX() const
{
    return Mx - 12.0f;
}

float Mario::GetFootCenterX() const
{
    return Mx;
}

float Mario::GetFootRightX() const
{
    return Mx + 12.0f;
}

float Mario::GetFootY() const
{
    return My;
}