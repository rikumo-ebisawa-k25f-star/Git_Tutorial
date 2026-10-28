#include "StageData.h"
#include <stdio.h>
#include <DxLib.h>

// マップデータの配列定義
int g_mapData[MAP_HEIGHT][MAP_WIDTH] = { 0 };

// マップチップの画像ハンドルを保持する配列（ID 0?15に対応）
static int s_chipHandles[15] = { -1 };

// CSVファイルの読み込み
bool LoadMapCSV(const char* fileName)
{
    FILE* fp = NULL;
    errno_t err = fopen_s(&fp, fileName, "r");
    if (err != 0 || fp == NULL) return false;

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            // カンマ区切りで数値を読み込む
            if (x == MAP_WIDTH - 1)
            {
                fscanf_s(fp, "%d", &g_mapData[y][x]);
            }
            else
            {
                fscanf_s(fp, "%d,", &g_mapData[y][x]);
            }
        }
    }

    fclose(fp);
    return true;
}

// 画像素材の読み込み処理
bool LoadMapImages()
{
    // 各チップIDに対応する画像をロード
    s_chipHandles[1] = LoadGraph("assets/Images/Block/floor.png");       // 地面
    s_chipHandles[2] = LoadGraph("assets/Images/Block/block.png");       // レンガ
    s_chipHandles[3] = LoadGraph("assets/Images/Block/hatena.png");      // ハテナブロック
    s_chipHandles[4] = LoadGraph("assets/Images/Block/kai_block.png");   // 階段

    //// 必須画像の読み込みチェック（エラー判定）
    //if (s_chipHandles[1] == -1 || s_chipHandles[2] == -1 || s_chipHandles[3] == -1)
    //{
    //    return false;
    //}
    return true;
}

// 画像素材の解放処理
void UnloadMapImages()
{
    for (int i = 0; i < 15; i++)
    {
        if (s_chipHandles[i] != -1)
        {
            DeleteGraph(s_chipHandles[i]);
            s_chipHandles[i] = -1;
        }
    }
}

// マップの描画処理（カメラ位置と画面幅を受け取る）
void DrawMap(int cameraX, int screenWidth)
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            int chipType = g_mapData[y][x];

            // 0（空気）は描画しない
            if (chipType == 0) continue;

            // スクロール（cameraX）を引いて描画位置を計算
            int drawX = x * CHIP_SIZE - cameraX;
            int drawY = y * CHIP_SIZE;

            // 画面外のマスは描画をスキップ（軽量化）
            if (drawX + CHIP_SIZE < 0 || drawX > screenWidth) continue;

            // 画像が読み込まれていれば画像を描画
            if (chipType < 15 && s_chipHandles[chipType] != -1)
            {
                DrawGraph(drawX, drawY, s_chipHandles[chipType], TRUE);
            }
            else
            {
                // まだ画像を設定していないチップ（土管やゴールなど）は仮の緑四角で表示
                DrawBox(drawX, drawY, drawX + CHIP_SIZE, drawY + CHIP_SIZE, GetColor(0, 200, 0), TRUE);
            }
        }
    }
}