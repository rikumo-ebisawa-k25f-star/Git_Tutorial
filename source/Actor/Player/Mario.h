#pragma once
#include "DxLib.h"
#include <cmath>

class Mario
{
public:
    float x, y;
    float vx, vy;
    bool onGround;

    const float gravity = 0.3f;
    const float moveAccel = 0.5f;
    const float moveDecel = 0.4f;
    const float airAccel = 0.2f;
    const float maxSpeed = 4.0f;
    const float jumpPower = -12.0f;
    const float groundY = 400.0f;

    Mario(float _x = 100.0f, float _y = 100.0f)
    {
        x = _x;
        y = _y;
        vx = vy = 0.0f;
        onGround = false;
    }

    void Update();
    void Draw();

private:
    void MoveX();
    void MoveY();
    void ApplyPhysics();
    void GroundCheck();
};
