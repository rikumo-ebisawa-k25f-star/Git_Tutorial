#include "DxLib.h"
#include "InGameScene.h"
#include "../File/StageData.h"

// シーン内で管理する変数
static int s_cameraX = 0;
static int s_blockHandle = -1;  // ← ブロック画像ハンドルをここで定義
#define SCREEN_WIDTH 640

// 初期化処理
bool InitInGameScene() {
    s_cameraX = 0;

    // マップデータ読み込み
    if (!LoadMapCSV("assets/Data/mario_map.csv")) {
        return false;
    }

    // ブロック画像読み込み
    s_blockHandle = LoadGraph("assets/Images/block.png");
    if (s_blockHandle == -1) {
        return false; // 読み込み失敗時
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
    DrawMap(s_blockHandle);
}

// 終了処理
void FinalizeInGameScene() {
    // 画像の解放処理
    if (s_blockHandle != -1) {
        DeleteGraph(s_blockHandle);
        s_blockHandle = -1;
    }
}
