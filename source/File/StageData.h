
#pragma once

#ifndef STAGE_DATA_H
#define STAGE_DATA_H

// マップの定数定義
#define CHIP_SIZE 32       // 1チップのサイズ（ピクセル）
#define MAP_WIDTH 212      // CSVの列数
#define MAP_HEIGHT 15      // CSVの行数

// グローバル配列・変数の宣言（AppMainや他ファイルから参照できるように外部宣言）
extern int g_mapData[MAP_HEIGHT][MAP_WIDTH];

// 関数のプロトタイプ宣言
bool LoadMapCSV(const char* fileName);
void DrawMap(int cameraX, int screenWidth);

#endif // STAGE_DATA_H


