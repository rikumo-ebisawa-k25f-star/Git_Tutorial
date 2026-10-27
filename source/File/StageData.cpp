#include "DxLib.h"
#include "StageData.h"
#include <stdio.h>

// マップデータを保持する配列の実体
int g_mapData[MAP_HEIGHT][MAP_WIDTH] = { 0 };

// CSVファイルを読み込む関数
bool LoadMapCSV(const char* fileName) {
    FILE* fp = NULL;
    fopen_s(&fp, fileName, "r");
    if (fp == NULL) return false;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (x == MAP_WIDTH - 1) {
                fscanf_s(fp, "%d", &g_mapData[y][x]);
            }
            else {
                fscanf_s(fp, "%d,", &g_mapData[y][x]);
            }
        }
    }

    fclose(fp);
    return true;
}

// チップIDごとの色取得（デバッグ用）
static unsigned int GetChipColor(int chipType) {
    switch (chipType) {
    case 1:  return GetColor(200, 76, 12);   // 地面（茶色）
    case 2:  return GetColor(200, 76, 12);   // レンガ（茶色）
    case 3:  return GetColor(252, 188, 60);  // ハテナブロック（黄色）
    case 4:  return GetColor(252, 188, 180); // 階段（薄茶）
    case 5:                                  // 土管
    case 6:
    case 7:
    case 8:  return GetColor(0, 168, 0);     // 土管（緑）
    case 9:  return GetColor(252, 252, 252); // ゴールポール（白）
    case 10: return GetColor(168, 0, 32);    // お城（赤褐色）
    default: return GetColor(0, 0, 0);
    }
}

// マップ描画関数
void DrawMap(int cameraX, int screenWidth) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int chipType = g_mapData[y][x];

            // 0（空気）はスキップ
            if (chipType == 0) continue;

            // スクロール位置を加味した描画座標
            int drawX = x * CHIP_SIZE - cameraX;
            int drawY = y * CHIP_SIZE;

            // 画面外描画スキップ
            if (drawX + CHIP_SIZE < 0 || drawX > screenWidth) continue;

            // ブロック描画
            unsigned int color = GetChipColor(chipType);
            DrawBox(drawX, drawY, drawX + CHIP_SIZE, drawY + CHIP_SIZE, color, TRUE);
            DrawBox(drawX, drawY, drawX + CHIP_SIZE, drawY + CHIP_SIZE, GetColor(0, 0, 0), FALSE); // 枠線
        }
    }
}