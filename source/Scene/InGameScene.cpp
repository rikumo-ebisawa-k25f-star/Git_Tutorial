//#include "InGameScene.h"
//#include "../Actor/Enemy/Kuribo.h"
//
//// 敵のインスタンス
//Kuribo enemy;
//
//// 初期化処理
//void InGameScene::Init()
//{
//    // 敵を画面左側に配置
//    // x座標: 50、y座標: 240、移動速度: 100
//    enemy.Init(50.0f, 240.0f, 100.0f);
//}
//
//// 更新処理
//void InGameScene::Update()
//{
//    // 60FPSを想定した経過時間
//    enemy.Update(1.0f / 60.0f);
//}
//
//// 描画処理
//void InGameScene::Draw()
//{
//    enemy.Draw();
//}
#include "DxLib.h"
#include "InGameScene.h"
#include "../File/StageData.h"

// シーン内で管理する変数
static int s_cameraX = 0;
#define SCREEN_WIDTH 640

// 初期化処理
bool InitInGameScene() {
    s_cameraX = 0;

    // StageData 側の読み込み処理を呼び出す
    if (!LoadMapCSV("mario_map.csv")) {
        return false;
    }
    return true;
}

// 更新処理（1フレームごとの計算・操作）
void UpdateInGameScene() {
    // 左右キーでカメラ（プレイヤー）移動
    if (CheckHitKey(KEY_INPUT_RIGHT)) s_cameraX += 4;
    if (CheckHitKey(KEY_INPUT_LEFT))  s_cameraX -= 4;

    // カメラの移動範囲制限
    if (s_cameraX < 0) {
        s_cameraX = 0;
    }
    int maxCameraX = MAP_WIDTH * CHIP_SIZE - SCREEN_WIDTH;
    if (s_cameraX > maxCameraX) {
        s_cameraX = maxCameraX;
    }
}

// 描画処理
void DrawInGameScene() {
    // StageData 側の描画処理を呼び出し
    DrawMap(s_cameraX, SCREEN_WIDTH);
}

// 終了処理
void FinalizeInGameScene() {
    // 必要に応じて画像の解放処理などを記述
}