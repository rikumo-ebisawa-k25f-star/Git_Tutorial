
#include "DxLib.h"
#include "Actor/Player/Mario.h"
#include "File/StageData.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(TRUE);
    SetGraphMode(1024, 768, 32);
    if (DxLib_Init() == -1) return -1;
    SetDrawScreen(DX_SCREEN_BACK);

    // マップ読み込み
    if (!LoadMapCSV("assets/Data/MAPp.csv"))
    {
        printf("CSVファイルの読み込みに失敗しました。\n");
    }

    Mario mario(100.0f, 100.0f);

    int blockImg = LoadGraph("assets/Images/block.png");

    while (ProcessMessage() == 0 && ClearDrawScreen() == 0)
    {
        DrawMap(blockImg);

        mario.Update();
        mario.Draw();

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}
