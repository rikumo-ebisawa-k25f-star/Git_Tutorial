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
    DrawBox((int)x - 16, (int)y - 32, (int)x + 16, (int)y, GetColor(255, 0, 0), TRUE);
}

void Mario::MoveX()
{
    if (CheckHitKey(KEY_INPUT_RIGHT))
        vx += onGround ? moveAccel : airAccel;
    else if (CheckHitKey(KEY_INPUT_LEFT))
        vx -= onGround ? moveAccel : airAccel;
    else
    {
        if (vx > 0) vx -= moveDecel;
        else if (vx < 0) vx += moveDecel;
        if (fabs(vx) < 0.1f) vx = 0.0f;
    }

    if (vx > maxSpeed) vx = maxSpeed;
    if (vx < -maxSpeed) vx = -maxSpeed;
}

void Mario::MoveY()
{
    if (onGround && CheckHitKey(KEY_INPUT_Z))
    {
        vy = jumpPower;
        onGround = false;
    }
}

void Mario::ApplyPhysics()
{
    vy += gravity;
    x += vx;
    y += vy;
}

void Mario::GroundCheck()
{
    if (y >= groundY)
    {
        y = groundY;
        vy = 0.0f;
        onGround = true;
    }
    else
    {
        onGround = false;
    }
}
