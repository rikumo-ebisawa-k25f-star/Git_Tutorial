#include "DxLib.h"
#include "InGameScene.h"
#include "../File/StageData.h"
#include "../Actor/Player/Mario.h"
#include "../Actor/Enemy/Kuribo.h"

static int s_cameraX = 0;
static StageData s_stage;          // ★ステージデータのインスタンスを追加
static Mario* s_pMario = nullptr;  // マリオのインスタンスポインタ
static Kuribo s_kuribo;

static void CheckCollision();

// ※WinMainの仮想画面サイズ(512)と合わせる場合は 512 に変更してください
#define SCREEN_WIDTH 512

// 初期化処理
bool InitInGameScene() {
    s_cameraX = 0;

    // ★s_stage のメンバ関数としてCSVと画像を読み込む
    if (!s_stage.LoadBgCSV("assets/assets/Data/BackGroundData (1).csv")) {
        return false;
    }
    if (!s_stage.LoadBlockCSV("assets/assets/Data/BlockData.csv")) {
        return false;
    }
    if (!s_stage.LoadMapImages()) {
        return false;
    }

    // マリオのインスタンス生成 (初期座標: X=100, Y=100)
    s_pMario = new Mario(100.0f, 100.0f);

    // クリボー・ノコノコの初期化
    if (!s_kuribo.Init(
        "assets/assets/Data/enemy_map.csv",
        "assets/assets/Data/BlockData.csv",
        StageData::CHIP_SIZE
    ))
    {
        return false;
    }

    return true;
}

// 更新処理
void UpdateInGameScene() {
    // ★ハテナブロックのアニメーション更新（元のコードで呼び忘れていた部分）
    s_stage.UpdateMap();

    // 左右キーでカメラ移動
    if (CheckHitKey(KEY_INPUT_RIGHT)) s_cameraX += 4;
    if (CheckHitKey(KEY_INPUT_LEFT))  s_cameraX -= 4;

    // カメラ移動範囲の制限（★定数は StageData:: から取得）
    if (s_cameraX < 0) s_cameraX = 0;
    int maxCameraX = StageData::MAP_WIDTH * StageData::CHIP_SIZE - SCREEN_WIDTH;
    if (s_cameraX > maxCameraX) s_cameraX = maxCameraX;

    // マリオの更新
    if (s_pMario) {
        s_pMario->Update();

        CheckCollision();
    }
    // 敵の更新
    s_kuribo.Update((float)s_cameraX);

}

// 描画処理
void DrawInGameScene() {
    // 1. マップの描画（★s_stage から呼ぶ）
    s_stage.DrawMap(s_cameraX, SCREEN_WIDTH);

    // 2. マリオの描画
    if (s_pMario) {
        s_pMario->Draw();
    }
    // クリボー・ノコノコの描画
    s_kuribo.Draw((float)s_cameraX);
}

// 終了処理
void FinalizeInGameScene() {
    // マリオの破棄
    if (s_pMario) {
        delete s_pMario;
        s_pMario = nullptr;
    }

    // マップ画像の解放（★s_stage から呼ぶ）
    s_stage.UnloadMapImages();
}

static void CheckCollision()
{
    Rect rect = s_pMario->GetRect();

    int leftFootX =
        (int)(rect.left + 2) /
        StageData::CHIP_SIZE;

    int rightFootX =
        (int)(rect.right - 2) /
        StageData::CHIP_SIZE;

    int footY =
        (int)(rect.bottom) /
        StageData::CHIP_SIZE;

    bool leftGround =
        s_stage.IsSolidBlock(
            leftFootX,
            footY);

    bool rightGround =
        s_stage.IsSolidBlock(
            rightFootX,
            footY);

    if (leftGround || rightGround)
    {
        float blockTop =
            footY * StageData::CHIP_SIZE;

        s_pMario->SetPos(
            s_pMario->GetX(),
            blockTop);

        s_pMario->SetVY(0.0f);

        s_pMario->SetGround(true);
    }
    else
    {
        s_pMario->SetGround(false);
    }
}