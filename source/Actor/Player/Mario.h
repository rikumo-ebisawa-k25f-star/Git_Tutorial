//#pragma once
//#include "DxLib.h"
//#include <cmath>
//
//class Mario
//{
//public:
//    float Mx, My;
//    float vx, vy;
//    bool onGround;
//
//    const float gravity = 0.3f;
//    const float moveAccel = 0.5f;
//    const float moveDecel = 0.4f;
//    const float airAccel = 0.2f;
//    const float maxSpeed = 2.0f;
//    const float jumpPower = -10.0f;
//    const float groundY = 400.0f;
//
//    Mario(float _x = 100.0f, float _y = 100.0f)
//    {
//        Mx = _x;
//        My = _y;
//        vx = vy = 0.0f;
//        onGround = false;
//    }
//
//    void Update();
//    void Draw();
//
//private:
//    void MoveX();
//    void MoveY();
//    void ApplyPhysics();
//    void GroundCheck();
//};




#pragma once

#include "DxLib.h"

struct Rect
{
    float left;
    float top;
    float right;
    float bottom;
};

class Mario
{
public:

    Mario(float x = 100.0f, float y = 100.0f);

    void Update();
    void Draw();

    // 座標取得
    float GetX() const;
    float GetY() const;

    // 速度取得
    float GetVX() const;
    float GetVY() const;

    // 座標変更
    void SetPos(float x, float y);

    // 速度変更
    void SetVX(float value);
    void SetVY(float value);

    // 接地状態
    void SetGround(bool flag);
    bool IsGround() const;

    // 当たり判定用
    Rect GetRect() const;

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

    const float gravity = 0.6f;

    const float moveAccel = 0.15f;
    const float moveDecel = 0.08f;
    const float airAccel = 0.08f;

    const float maxSpeed = 2.5f;
    const float dashSpeed = 4.0f;

    const float jumpPower = -15.0f;
};