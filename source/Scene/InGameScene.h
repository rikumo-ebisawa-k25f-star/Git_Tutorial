#pragma once

#include "../Actor/Enemy/Kuribo.h"

class InGameScene
{
public:
    InGameScene() = default;
    ~InGameScene() = default;

    // 初期化処理
    void Init();

    // 更新処理
    void Update();

    // 描画処理
    void Draw();

private:
    Kuribo kuribo;
};

#ifndef IN_GAME_SCENE_H
#define IN_GAME_SCENE_H

// インゲームシーンの初期化（CSV読み込みなど）
bool InitInGameScene();

// インゲームシーンの更新処理（入力・カメラ移動など）
void UpdateInGameScene();

// インゲームシーンの描画処理
void DrawInGameScene();

// インゲームシーンの終了処理
void FinalizeInGameScene();

#endif // IN_GAME_SCENE_H