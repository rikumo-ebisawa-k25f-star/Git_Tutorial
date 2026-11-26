#pragma once

#include "DxLib.h"

class Mario
{
public:

    Mario(float x, float y);

    void Update();
    void Draw(int cameraX);

    float GetX() const;
    float GetY() const;

    float GetVX() const;
    float GetVY() const;

    void SetPos(float x, float y);

    void SetVX(float value);
    void SetVY(float value);

    void SetGround(bool flag);

    float GetFootLeftX() const;
    float GetFootCenterX() const;
    float GetFootRightX() const;
    float GetFootY() const;

private:

    void MoveX();
    void MoveY();
    void ApplyPhysics();

private:

    float Mx;
    float My;

    float vx;
    float vy;

    bool onGround;

    const float gravity = 0.5f;

    const float moveAccel = 0.20f;
    const float moveDecel = 0.15f;

    const float airAccel = 0.10f;

    const float maxSpeed = 3.0f;
    const float dashSpeed = 5.0f;

    const float jumpPower = -15.0f;
};