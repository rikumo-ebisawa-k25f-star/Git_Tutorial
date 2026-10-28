
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
#include "DxLib.h"
#include "Scene/InGameScene.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(TRUE);
    SetGraphMode(1024, 768, 32);
    if (DxLib_Init() == -1) return -1;
    SetDrawScreen(DX_SCREEN_BACK);

    // インゲームシーンの初期化（マップ読み込み・画像ロードなど）
    if (!InitInGameScene())
    {
        DxLib_End();
        return -1;
    }

    // メインループ
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

        // 更新処理（マリオの移動、カメラの移動など）
        UpdateInGameScene();

        // 描画処理（マップ描画、マリオ描画など）
        DrawInGameScene();

        ScreenFlip();
    }

    // 終了処理（メモリ解放など）
    FinalizeInGameScene();

    DxLib_End();
    return 0;
}