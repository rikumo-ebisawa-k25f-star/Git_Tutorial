
//#include "DxLib.h"
//#include "Actor/Player/Mario.h"
//#include "File/StageData.h"
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//{
//    ChangeWindowMode(TRUE);
//    SetGraphMode(1024, 768, 32);
//    if (DxLib_Init() == -1) return -1;
//    SetDrawScreen(DX_SCREEN_BACK);
//
//    // マップ読み込み
//    if (!LoadMapCSV("assets/Data/MAPp.csv"))
//    {
//        printf("CSVファイルの読み込みに失敗しました。\n");
//    }
//
//    Mario mario(100.0f, 100.0f);
//
//    int blockImg = LoadGraph("assets/Images/block.png");
//
//    while (ProcessMessage() == 0 && ClearDrawScreen() == 0)
//    {
//        DrawMap(blockImg);
//
//        mario.Update();
//        mario.Draw();
//
//        ScreenFlip();
//    }
//
//    DxLib_End();
//    return 0;
//}
//#include "DxLib.h"
//#include "Scene/InGameScene.h"
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//{
//    ChangeWindowMode(TRUE);
//    SetGraphMode(1024, 768, 32);
//    if (DxLib_Init() == -1) return -1;
//    SetDrawScreen(DX_SCREEN_BACK);
//
//    // インゲームシーンの初期化（マップ読み込み・画像ロードなど）
//    if (!InitInGameScene())
//    {
//        DxLib_End();
//        return -1;
//    }
//
//    // メインループ
//    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
//    {
//        ClearDrawScreen();
//
//        // 更新処理（マリオの移動、カメラの移動など）
//        UpdateInGameScene();
//
//        // 描画処理（マップ描画、マリオ描画など）
//        DrawInGameScene();
//
//        ScreenFlip();
//    }
//
//    // 終了処理（メモリ解放など）
//    FinalizeInGameScene();
//
//    DxLib_End();
//    return 0;
//}
#include "DxLib.h"
#include "Scene/InGameScene.h"

// 16マス分の内部ゲームサイズ
#define GAME_WIDTH  512
#define GAME_HEIGHT 480

int WINAPI WinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPSTR lpCmdLine,_In_ int nCmdShow)
{
    ChangeWindowMode(TRUE);

    // ウィンドウ解像度は 1024 x 768 に固定
    SetGraphMode(1024, 768, 32);

    if (DxLib_Init() == -1) return -1;

    // 16マス分(512x480)の描画用キャンバス（仮想画面）を作成
    int screenHandle = MakeScreen(GAME_WIDTH, GAME_HEIGHT, FALSE);

    if (!InitInGameScene())
    {
        DxLib_End();
        return -1;
    }

    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        // 1. まずは 512x480 (16マス分) のキャンバスに描画
        SetDrawScreen(screenHandle);
        ClearDrawScreen();

        UpdateInGameScene();
        DrawInGameScene();

        // 2. 実際の画面 (1024x768) に切り替えて、キャンバスを画面全体へ拡大描画
        SetDrawScreen(DX_SCREEN_BACK);
        ClearDrawScreen();

        // ドット絵がボヤけないようくっきり拡大
        SetDrawMode(DX_DRAWMODE_NEAREST);

        // 512x480の画面を 1024x768 のウィンドウ全体へ引き伸ばして描画
        DrawExtendGraph(0, 0, 1024, 768, screenHandle, FALSE);

        ScreenFlip();
    }

    DeleteGraph(screenHandle);
    FinalizeInGameScene();
    DxLib_End();
    return 0;
}