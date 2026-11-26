//#include "DxLib.h"
//#include "InGameScene.h"
//#include "../File/StageData.h"
//#include "../Actor/Player/Mario.h"
//#include "../Actor/Enemy/Kuribo.h"
//
//static int s_cameraX = 0;
//static StageData s_stage;          // ★ステージデータのインスタンスを追加
//static Mario* s_pMario = nullptr;  // マリオのインスタンスポインタ
//static Kuribo s_kuribo;
//static void CheckCollision();
//
//// ※WinMainの仮想画面サイズ(512)と合わせる場合は 512 に変更してください
//#define SCREEN_WIDTH 512
//
//// 初期化処理
//bool InitInGameScene() {
//    s_cameraX = 0;
//
//    // ★s_stage のメンバ関数としてCSVと画像を読み込む
//    if (!s_stage.LoadBgCSV("assets/assets/Data/BackGroundData (1).csv")) {
//        return false;
//    }
//    if (!s_stage.LoadBlockCSV("assets/assets/Data/BlockData.csv")) {
//        return false;
//    }
//    if (!s_stage.LoadMapImages()) {
//        return false;
//    }
//
//    // マリオのインスタンス生成 (初期座標: X=100, Y=100)
//    s_pMario = new Mario(100.0f, 100.0f);
//
//    // クリボー・ノコノコの初期化
//    if (!s_kuribo.Init(
//        "assets/assets/Data/enemy_map.csv",
//        "assets/assets/Data/BlockData.csv",
//        StageData::CHIP_SIZE
//    ))
//    {
//        return false;
//    }
//
//    return true;
//}
//
//// 更新処理
//void UpdateInGameScene() {
//    // ★ハテナブロックのアニメーション更新（元のコードで呼び忘れていた部分）
//    s_stage.UpdateMap();
//
//    // 左右キーでカメラ移動
//    if (CheckHitKey(KEY_INPUT_RIGHT)) s_cameraX += 4;
//    if (CheckHitKey(KEY_INPUT_LEFT))  s_cameraX -= 4;
//
//    // カメラ移動範囲の制限（★定数は StageData:: から取得）
//    if (s_cameraX < 0) s_cameraX = 0;
//    int maxCameraX = StageData::MAP_WIDTH * StageData::CHIP_SIZE - SCREEN_WIDTH;
//    if (s_cameraX > maxCameraX) s_cameraX = maxCameraX;
//
//    // マリオの更新
//    if (s_pMario)
//    {
//        s_pMario->Update();
//
//        CheckCollision();
//    }
//    // 敵の更新
//    s_kuribo.Update((float)s_cameraX);
//}
//
//// 描画処理
//void DrawInGameScene() {
//    // 1. マップの描画（★s_stage から呼ぶ）
//    s_stage.DrawMap(s_cameraX, SCREEN_WIDTH);
//
//    // 2. マリオの描画
//    if (s_pMario) {
//        s_pMario->Draw();
//    }
//    // クリボー・ノコノコの描画
//    s_kuribo.Draw((float)s_cameraX);
//}
//
//static void CheckCollision()
//{
//    if (!s_pMario)
//    {
//        return;
//    }
//
//    // 落下中だけ地面判定
//    if (s_pMario->GetVY() < 0.0f)
//    {
//        return;
//    }
//
//    int leftX =
//        (int)(s_pMario->GetFootLeftX())
//        / StageData::CHIP_SIZE;
//
//    int centerX =
//        (int)(s_pMario->GetFootCenterX())
//        / StageData::CHIP_SIZE;
//
//    int rightX =
//        (int)(s_pMario->GetFootRightX())
//        / StageData::CHIP_SIZE;
//
//    int footY =
//        (int)(s_pMario->GetFootY())
//        / StageData::CHIP_SIZE;
//    /////////////////////////////////////
//   /* printfDx("%d",
//        s_stage.GetBlockChip(
//            centerX,
//            footY));*/
//    ///////////////////////////////
//    bool hitGround =
//        s_stage.IsSolidBlock(leftX, footY) ||
//        s_stage.IsSolidBlock(centerX, footY) ||
//        s_stage.IsSolidBlock(rightX, footY);
//
//    if (hitGround)
//    {
//        float blockTop =
//            footY * StageData::CHIP_SIZE;
//
//        s_pMario->SetPos(
//            s_pMario->GetX(),
//            blockTop);
//
//        s_pMario->SetVY(0.0f);
//        s_pMario->SetGround(true);
//        //////////////////////////
//        /*printfDx(
//            "hit=%d footY=%d blockTop=%.1f My=%.1f",
//            hitGround,
//            footY,
//            blockTop,
//            s_pMario->GetY()
//        );*/
//        //////////////////////////////////
//
//
//
//    }
//    else
//    {
//        s_pMario->SetGround(false);
//    }
//}
//
//// 終了処理
//void FinalizeInGameScene() {
//    // マリオの破棄
//    if (s_pMario) {
//        delete s_pMario;
//        s_pMario = nullptr;
//    }
//
//    // マップ画像の解放（★s_stage から呼ぶ）
//    s_stage.UnloadMapImages();
//}


#include "DxLib.h"
#include "InGameScene.h"

#include "../File/StageData.h"
#include "../Actor/Player/Mario.h"
#include "../Actor/Enemy/Kuribo.h"

static int s_cameraX = 0;

static StageData s_stage;
static Mario* s_pMario = nullptr;
static Kuribo s_kuribo;

static void CheckCollision();

#define SCREEN_WIDTH 512

//-------------------------------------------------
// 初期化
//-------------------------------------------------

bool InitInGameScene()
{
    s_cameraX = 0;

    if (!s_stage.LoadBgCSV(
        "assets/assets/Data/BackGroundData (1).csv"))
    {
        return false;
    }

    if (!s_stage.LoadBlockCSV(
        "assets/assets/Data/BlockData.csv"))
    {
        return false;
    }

    if (!s_stage.LoadMapImages())
    {
        return false;
    }

    s_pMario = new Mario(
        100.0f,
        100.0f);

    if (!s_kuribo.Init(
        "assets/assets/Data/enemy_map.csv",
        "assets/assets/Data/BlockData.csv",
        StageData::CHIP_SIZE))
    {
        return false;
    }

    return true;
}

//
// 更新
//

void UpdateInGameScene()
{
    s_stage.UpdateMap();

    if (s_pMario)
    {
        s_pMario->Update();

        CheckCollision();

        // カメラ追従
        s_cameraX =
            (int)s_pMario->GetX() -
            SCREEN_WIDTH / 2;

        if (s_cameraX < 0)
        {
            s_cameraX = 0;
        }

        int maxCameraX =
            StageData::MAP_WIDTH *
            StageData::CHIP_SIZE -
            SCREEN_WIDTH;

        if (s_cameraX > maxCameraX)
        {
            s_cameraX = maxCameraX;
        }
    }

    s_kuribo.Update((float)s_cameraX);
}

//
// 描画
//

void DrawInGameScene()
{
    s_stage.DrawMap(
        s_cameraX,
        SCREEN_WIDTH);

    if (s_pMario)
    {
        s_pMario->Draw(s_cameraX);
    }

    s_kuribo.Draw(
        (float)s_cameraX);
}

//
// 衝突判定
//

static void CheckCollision()
{
    if (!s_pMario)
    {
        return;
    }

    //-----------------------------------
    // 左右の壁判定
    //-----------------------------------

    int leftWallX =
        (int)((s_pMario->GetX() - 16) /
            StageData::CHIP_SIZE);

    int rightWallX =
        (int)((s_pMario->GetX() + 16) /
            StageData::CHIP_SIZE);

    int bodyTopY =
        (int)((s_pMario->GetY() - 28) /
            StageData::CHIP_SIZE);

    int bodyBottomY =
        (int)((s_pMario->GetY() - 4) /
            StageData::CHIP_SIZE);

    // 左壁
    if (s_pMario->GetVX() < 0)
    {
        bool hitLeftWall =
            s_stage.IsSolidBlock(leftWallX, bodyTopY) ||
            s_stage.IsSolidBlock(leftWallX, bodyBottomY);

        if (hitLeftWall)
        {
            float wallRight =
                (leftWallX + 1) *
                StageData::CHIP_SIZE;

            s_pMario->SetPos(
                wallRight + 16,
                s_pMario->GetY());

            s_pMario->SetVX(0.0f);
        }
    }

    // 右壁
    if (s_pMario->GetVX() > 0)
    {
        bool hitRightWall =
            s_stage.IsSolidBlock(rightWallX, bodyTopY) ||
            s_stage.IsSolidBlock(rightWallX, bodyBottomY);

        if (hitRightWall)
        {
            float wallLeft =
                rightWallX *
                StageData::CHIP_SIZE;

            s_pMario->SetPos(
                wallLeft - 16,
                s_pMario->GetY());

            s_pMario->SetVX(0.0f);
        }
    }

    //-----------------------------------
    // 天井判定
    //-----------------------------------

    if (s_pMario->GetVY() < 0)
    {
        int headY =
            (int)((s_pMario->GetY() - 32) /
                StageData::CHIP_SIZE);

        int leftHeadX =
            (int)((s_pMario->GetX() - 12) /
                StageData::CHIP_SIZE);

        int rightHeadX =
            (int)((s_pMario->GetX() + 12) /
                StageData::CHIP_SIZE);

        bool hitHead =
            s_stage.IsSolidBlock(leftHeadX, headY) ||
            s_stage.IsSolidBlock(rightHeadX, headY);

        if (hitHead)
        {
            s_pMario->SetVY(0.0f);

            // ハテナブロック判定用
            int blockID =
                s_stage.GetBlockChip(
                    leftHeadX,
                    headY);

            if (blockID == 37)
            {
                // TODO:
                // ハテナブロック処理
            }
        }
    }

    //-----------------------------------
    // 床判定
    //-----------------------------------

    if (s_pMario->GetVY() >= 0)
    {
        int leftX =
            (int)((s_pMario->GetX() - 12) /
                StageData::CHIP_SIZE);

        int centerX =
            (int)(s_pMario->GetX() /
                StageData::CHIP_SIZE);

        int rightX =
            (int)((s_pMario->GetX() + 12) /
                StageData::CHIP_SIZE);

        int footY =
            (int)((s_pMario->GetY() + 2) /
                StageData::CHIP_SIZE);

        bool hitGround =
            s_stage.IsSolidBlock(leftX, footY) ||
            s_stage.IsSolidBlock(centerX, footY) ||
            s_stage.IsSolidBlock(rightX, footY);

        if (hitGround)
        {
            float blockTop =
                footY *
                StageData::CHIP_SIZE;

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
}

//
// 終了
//

void FinalizeInGameScene()
{
    if (s_pMario)
    {
        delete s_pMario;
        s_pMario = nullptr;
    }

    s_stage.UnloadMapImages();
}