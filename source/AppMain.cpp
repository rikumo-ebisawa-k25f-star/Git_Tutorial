//#include <DxLib.h>
//#include <algorithm>
//#include "File/StageData.h"
//
//int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
//{
//	ChangeWindowMode(TRUE);
//	SetGraphMode(640, 480, 32);
//	if (DxLib_Init() == -1) return -1;
//	SetDrawScreen(DX_SCREEN_BACK);
//
//	// 1. マップデータの読み込み
//	StageData::Get()->Load();
//
//	// 2. 床画像の読み込み（画像ファイル名を入れる）
//	// ※ 画像がまだない場合はコメントアウトしてください
//	int blockGraph = LoadGraph("block.png");
//
//	float cameraX = 0.0f;
//	float cameraY = 0.0f;
//
//	while (ProcessMessage() != -1)
//	{
//		
//		ClearDrawScreen();
//
//		// 3. 描画関数を呼び出す
//	
//
//		ScreenFlip();
//	}
//
//
//	DxLib_End();
//	return 0;
//}
#include "DxLib.h"
#include <stdio.h>

#include "Actor/Player/Mario.h"

// マップの縦横サイズ（ご自身のCSVの行数・列数に合わせて調整してください）
#define MAP_WIDTH  69  // 横のマス数
#define MAP_HEIGHT 2 // 縦のマス数
#define BLOCK_SIZE 32  // 1マスの大きさ（32x32ピクセル）

// マップデータを保持する2次元配列
int g_MapData[MAP_HEIGHT][MAP_WIDTH];

// CSVファイルを読み込む関数
bool LoadMapCSV(const char* filePath) 
{
    FILE* fp = NULL;
    // ファイルを開く
    errno_t err = fopen_s(&fp, filePath, "r");
    if (err != 0 || fp == NULL) 
    {
        return false; // ファイルが開けなかった場合
    }

    // CSVデータを1マスずつ読み込む
    for (int y = 0; y < MAP_HEIGHT; y++) 
    {
        for (int x = 0; x < MAP_WIDTH; x++) 
        {
            // 数字を1つ読み込む
            fscanf_s(fp, "%d", &g_MapData[y][x]);

            // カンマまたは改行を読み飛ばす
            char c = fgetc(fp);
            if (c == '\r')  // Windowsの改行コード対策
            {
                fgetc(fp); // '\n' を読み捨てる
            }
        }
    }

    fclose(fp); // ファイルを閉じる
    return true;
}

// マップを描画する関数
void DrawMap(int blockHandle) 
{
    for (int y = 0; y < MAP_HEIGHT; y++) 
    {
        for (int x = 0; x < MAP_WIDTH; x++) 
        {
            // 描画する画面上の座標を計算
            int drawX = x * BLOCK_SIZE;
            int drawY = y * BLOCK_SIZE;

            // 値が 1（床）の場合は画像を描画
            if (g_MapData[y][x] == 1) 
            {
                // 画像がある場合
                DrawGraph(drawX, drawY, blockHandle, TRUE);

                // ※まだ画像が無い場合は以下のように四角形で代用できます
                // DrawBox(drawX, drawY, drawX + BLOCK_SIZE, drawY + BLOCK_SIZE, GetColor(150, 75, 0), TRUE);
            }
        }
    }
}

// メイン関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
    // DXライブラリ初期化処理
    ChangeWindowMode(TRUE);
    SetGraphMode(1024, 768, 32);
    if (DxLib_Init() == -1) return -1;
    SetDrawScreen(DX_SCREEN_BACK);

    // CSV読み込み（パスは環境に合わせて調整してください）
    if (!LoadMapCSV("assets/Data/MAPp.csv")) 
    {
        // 読み込み失敗時の処理
        printf("CSVファイルの読み込みに失敗しました。\n");
    }

    Mario mario(100.0f, 100.0f);

    // 床の画像を読み込み（画像ファイルがある場合）
    int blockImg = LoadGraph("assets/Images/block.png");

    // メインループ
    while (ProcessMessage() == 0 && ClearDrawScreen() == 0) 
    {

        // マップを描画
        DrawMap(blockImg);

        mario.Update();
        mario.Draw();

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}