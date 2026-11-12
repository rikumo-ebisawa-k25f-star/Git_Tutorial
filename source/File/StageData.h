#pragma once

// 新しいCSV（16行 × 224列）のサイズに合わせる
#define MAP_HEIGHT 16
#define MAP_WIDTH  224
#define CHIP_SIZE  32

// 背景用とブロック用の2つのマップ配列
extern int g_bgData[MAP_HEIGHT][MAP_WIDTH];
extern int g_blockData[MAP_HEIGHT][MAP_WIDTH];

bool LoadMapCSV(const char* fileName, int targetMap[MAP_HEIGHT][MAP_WIDTH]);
bool LoadMapImages();
void UnloadMapImages();
void UpdateMap();
void DrawMap(int cameraX, int screenWidth);