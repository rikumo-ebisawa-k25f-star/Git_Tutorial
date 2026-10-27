#pragma once

#define CHIP_SIZE 32
#define MAP_WIDTH 69
#define MAP_HEIGHT 2

extern int g_mapData[MAP_HEIGHT][MAP_WIDTH];

bool LoadMapCSV(const char* fileName);
void DrawMap(int blockHandle);
