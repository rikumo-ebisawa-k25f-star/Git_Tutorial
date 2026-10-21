#include "InGameScene.h"
#include "../Actor/Enemy/Kuribo.h"

// 敵のインスタンス
Kuribo enemy;

// 初期化処理
void InGameScene::Init()
{
    // 敵を画面左側に配置
    // x座標: 50、y座標: 240、移動速度: 100
    enemy.Init(50.0f, 240.0f, 100.0f);
}

// 更新処理
void InGameScene::Update()
{
    // 60FPSを想定した経過時間
    enemy.Update(1.0f / 60.0f);
}

// 描画処理
void InGameScene::Draw()
{
    enemy.Draw();
}