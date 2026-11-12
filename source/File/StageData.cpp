
#include "StageData.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <DxLib.h>

// 背景データとブロックデータの配列定義
int g_bgData[MAP_HEIGHT][MAP_WIDTH] = { 0 };
int g_blockData[MAP_HEIGHT][MAP_WIDTH] = { 0 };

// マップチップの画像ハンドルを保持する配列（ID 0~99に対応）
static int s_chipHandles[100];

// ハテナブロックのアニメーション用画像ハンドル配列 (4コマ分)
static int s_hatenaHandles[4] = { -1, -1, -1, -1 };

// ハテナブロックのアニメーション管理用
static int s_animTimer = 0;
static int s_animFrame = 0;

// CSVファイルの読み込み（背景用・ブロック用どちらも安全に読み込める）
bool LoadMapCSV(const char* fileName, int targetMap[MAP_HEIGHT][MAP_WIDTH])
{
    FILE* fp = NULL;
    errno_t err = fopen_s(&fp, fileName, "r");
    if (err != 0 || fp == NULL) return false;

    // まず配列をすべて0でクリア（横幅が224より少ないBackGroundData対策）
    memset(targetMap, 0, sizeof(int) * MAP_HEIGHT * MAP_WIDTH);

    char line[4096];
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        if (fgets(line, sizeof(line), fp) == NULL) break;

        char* context = NULL;
        char* token = strtok_s(line, ",\r\n", &context);
        int x = 0;
        while (token != NULL && x < MAP_WIDTH)
        {
            targetMap[y][x] = atoi(token);
            token = strtok_s(NULL, ",\r\n", &context);
            x++;
        }
    }

    fclose(fp);
    return true;
}

// 画像素材の読み込み処理
bool LoadMapImages()
{
    // 最初に全要素を -1（未読み込み状態）で初期化する
    for (int i = 0; i < 100; i++)
    {
        s_chipHandles[i] = -1;
    }

    // 各チップIDに対応する画像をロード
   
    s_chipHandles[34] = LoadGraph("assets/assets/Images/Block/floor.png");       // 地面
    s_chipHandles[33] = LoadGraph("assets/assets/Images/Block/block.png");       // レンガ
    s_chipHandles[36] = LoadGraph("assets/assets/Images/Block/kai_block.png");   // 階段
    s_chipHandles[5] = LoadGraph("assets/assets/Images/dokan_left_up.png");
    s_chipHandles[6] = LoadGraph("assets/assets/Images/dokan_right_up.png");
    s_chipHandles[7] = LoadGraph("assets/assets/Images/dokan_left_down.png");
    s_chipHandles[8] = LoadGraph("assets/assets/Images/dokan_right_down.png");

    LoadDivGraph("assets/assets/Images/Block/hatena.png", 4, 4, 1, CHIP_SIZE, CHIP_SIZE, s_hatenaHandles);

    return true;
}

// 画像素材の解放処理
void UnloadMapImages()
{
    for (int i = 0; i < 100; i++)
    {
        if (s_chipHandles[i] != -1)
        {
            DeleteGraph(s_chipHandles[i]);
            s_chipHandles[i] = -1;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (s_hatenaHandles[i] != -1)
        {
            DeleteGraph(s_hatenaHandles[i]);
            s_hatenaHandles[i] = -1;
        }
    }
}

// マップの更新処理
void UpdateMap()
{
    s_animTimer++;
    if (s_animTimer % 12 == 0)
    {
        s_animFrame = (s_animFrame + 1) % 4;
    }
}

// マップの描画処理（背景 → ブロックの順に重ねて描画）
void DrawMap(int cameraX, int screenWidth)
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            int drawX = x * CHIP_SIZE - cameraX;
            int drawY = y * CHIP_SIZE;

            if (drawX + CHIP_SIZE < 0 || drawX > screenWidth) continue;

            // 1. 背景レイヤー（g_bgData）を描画
            int bgType = g_bgData[y][x];
            if (bgType >= 0 && bgType < 100 && s_chipHandles[bgType] != -1)
            {
                DrawGraph(drawX, drawY, s_chipHandles[bgType], TRUE);
            }
            else
            {
                // 背景画像がない場所は空色（水色）で塗りつぶす
                DrawBox(drawX, drawY, drawX + CHIP_SIZE, drawY + CHIP_SIZE, GetColor(107, 136, 255), TRUE);
            }

            // 2. ブロックレイヤー（g_blockData）を描画（0は空気なのでスキップ）
            int blockType = g_blockData[y][x];
            if (blockType == 0) continue;

            // ハテナブロックのID（CSVの番号に合わせて必要なら調整してください）
            if (blockType == 21 || blockType == 3)
            {
                if (s_hatenaHandles[s_animFrame] != -1)
                {
                    DrawGraph(drawX, drawY, s_hatenaHandles[s_animFrame], TRUE);
                }
                else
                {
                    DrawBox(drawX, drawY, drawX + CHIP_SIZE, drawY + CHIP_SIZE, GetColor(255, 255, 0), TRUE);
                }
            }
            else if (blockType > 0 && blockType < 100 && s_chipHandles[blockType] != -1)
            {
                DrawGraph(drawX, drawY, s_chipHandles[blockType], TRUE);
            }
            else
            {
                // まだ画像を設定していないブロックは仮の緑四角で表示
                DrawBox(drawX, drawY, drawX + CHIP_SIZE, drawY + CHIP_SIZE, GetColor(0, 200, 0), TRUE);
            }
        }
    }
}