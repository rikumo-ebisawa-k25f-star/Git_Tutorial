#include "InGameScene.h"
#include <DxLib.h>
// 初期化処理
void InGameScene::Init()
{
    // 敵を画面左側に配置
    // X座標: 50、Y座標: 240、移動速度: 100
    kuribo.Init(50.0f, 240.0f, 100.0f);
}

// 更新処理
void InGameScene::Update()
{
    // 60FPSを想定した経過時間
    kuribo.Update(1.0f / 60.0f);
}

// 描画処理
void InGameScene::Draw()
{
    DrawCircle(320, 240, 50, GetColor(255, 0, 0), TRUE);
    ScreenFlip();
}