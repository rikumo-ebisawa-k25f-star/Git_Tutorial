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