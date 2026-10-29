//#include "DxLib.h"
//#include "InGameScene.h"
//#include "../File/StageData.h"
//
//// シーン内で管理する変数
//static int s_cameraX = 0;
//static int s_blockHandle = -1;  // ← ブロック画像ハンドルをここで定義
//#define SCREEN_WIDTH 640
//
//// 初期化処理
//bool InitInGameScene() {
//    s_cameraX = 0;
//
//    // マップデータ読み込み
//    if (!LoadMapCSV("assets/Data/mario_map.csv")) {
//        return false;
//    }
//
//    // ブロック画像読み込み
//    s_blockHandle = LoadGraph("assets/Images/block.png");
//    if (s_blockHandle == -1) {
//        return false; // 読み込み失敗時
//    }
//
//    return true;
//}
//
//// 更新処理（1フレームごとの計算・操作）
//void UpdateInGameScene() {
//    // 左右キーでカメラ（プレイヤー）移動
//    if (CheckHitKey(KEY_INPUT_RIGHT)) s_cameraX += 4;
//    if (CheckHitKey(KEY_INPUT_LEFT))  s_cameraX -= 4;
//
//    // カメラの移動範囲制限
//    if (s_cameraX < 0) {
//        s_cameraX = 0;
//    }
//    int maxCameraX = MAP_WIDTH * CHIP_SIZE - SCREEN_WIDTH;
//    if (s_cameraX > maxCameraX) {
//        s_cameraX = maxCameraX;
//    }
//}
//
//// 描画処理
//void DrawInGameScene() {
//    // StageData 側の描画処理を呼び出し
//    DrawMap(s_blockHandle);
//}
//
//// 終了処理
//void FinalizeInGameScene() {
//    // 画像の解放処理
//    if (s_blockHandle != -1) {
//        DeleteGraph(s_blockHandle);
//        s_blockHandle = -1;
//    }
//}
//#include "DxLib.h"
//#include "InGameScene.h"
//#include "../File/StageData.h"
//
//// シーン内で管理する変数
//static int s_cameraX = 0;
//#define SCREEN_WIDTH 640
//
//// 初期化処理
//bool InitInGameScene() {
//    s_cameraX = 0;
//
//    // マップデータ読み込み
//    if (!LoadMapCSV("assets/Data/mario_map.csv")) {
//        return false;
//    }
//
//    // マップ画像群の読み込み（StageData側で一括読み込み）
//    if (!LoadMapImages()) {
//        return false; // 読み込み失敗時
//    }
//
//    return true;
//}
//
//// 更新処理（1フレームごとの計算・操作）
//void UpdateInGameScene() {
//    // 左右キーでカメラ（プレイヤー）移動
//    if (CheckHitKey(KEY_INPUT_RIGHT)) s_cameraX += 4;
//    if (CheckHitKey(KEY_INPUT_LEFT))  s_cameraX -= 4;
//
//    // カメラの移動範囲制限
//    if (s_cameraX < 0) {
//        s_cameraX = 0;
//    }
//    int maxCameraX = MAP_WIDTH * CHIP_SIZE - SCREEN_WIDTH;
//    if (s_cameraX > maxCameraX) {
//        s_cameraX = maxCameraX;
//    }
//}
//
//// 描画処理
//void DrawInGameScene() {
//    // StageData 側の描画処理を呼び出し（カメラ位置と画面幅を渡す）
//    DrawMap(s_cameraX, SCREEN_WIDTH);
//}
//
//// 終了処理
//void FinalizeInGameScene() {
//    // マップ画像の解放処理（StageData側で一括解放）
//    UnloadMapImages();
//}
#include "DxLib.h"
#include "InGameScene.h"
#include "../File/StageData.h"
#include "../Actor/Player/Mario.h"
#include "../Actor/Enemy/Kuribo.h"

static int s_cameraX = 0;
static Mario* s_pMario = nullptr; // マリオのインスタンスポインタ
static Kuribo s_kuribo;

#define SCREEN_WIDTH 1024

// 初期化処理
bool InitInGameScene() {
    s_cameraX = 0;

    // マップCSVの読み込み
    if (!LoadMapCSV("assets/assets/Data/mario_map.csv")) { // ※ファイル名はプロジェクト内の正しい名前に設定
        return false;
    }

    // マップ画像の読み込み（StageData側で処理）
    if (!LoadMapImages()) {
        return false;
    }

    // マリオのインスタンス生成 (初期座標: X=100, Y=100)
    s_pMario = new Mario(100.0f, 100.0f);

    // 画面右側から出現させる
    s_kuribo.Init(
        (float)(s_cameraX + SCREEN_WIDTH + 20),
        360.0f,
        100.0f
    );

    return true;
}

// 更新処理
void UpdateInGameScene() {
    // 左右キーでカメラ移動
    if (CheckHitKey(KEY_INPUT_RIGHT)) s_cameraX += 4;
    if (CheckHitKey(KEY_INPUT_LEFT))  s_cameraX -= 4;

    // カメラ移動範囲の制限
    if (s_cameraX < 0) s_cameraX = 0;
    int maxCameraX = MAP_WIDTH * CHIP_SIZE - SCREEN_WIDTH;
    if (s_cameraX > maxCameraX) s_cameraX = maxCameraX;

    // マリオの更新
    if (s_pMario) {
        s_pMario->Update();
    }
    // クリボーの更新
    s_kuribo.Update(
        1.0f / 60.0f,
        (float)s_cameraX,
        (float)SCREEN_WIDTH
    );
}

// 描画処理
void DrawInGameScene() {
    // 1. マップの描画
    DrawMap(s_cameraX, SCREEN_WIDTH);

    // 2. マリオの描画
    if (s_pMario) {
        s_pMario->Draw();
    }
    //3.クリボーの描画
    s_kuribo.Draw((float)s_cameraX);
}

// 終了処理
void FinalizeInGameScene() {
    // マリオの破棄
    if (s_pMario) {
        delete s_pMario;
        s_pMario = nullptr;
    }

    // マップ画像の解放
    UnloadMapImages();
}