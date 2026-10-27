#include "StageData.h"
#include <stdio.h>
#include <DxLib.h>

int g_mapData[MAP_HEIGHT][MAP_WIDTH] = { 0 };

bool LoadMapCSV(const char* fileName)
{
    FILE* fp = NULL;
    errno_t err = fopen_s(&fp, fileName, "r");
    if (err != 0 || fp == NULL) return false;

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            fscanf_s(fp, "%d", &g_mapData[y][x]);
            char c = fgetc(fp);
            if (c == '\r') fgetc(fp);
        }
    }

    fclose(fp);
    return true;
}

void DrawMap(int blockHandle)
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            if (g_mapData[y][x] == 1)
            {
                int drawX = x * CHIP_SIZE;
                int drawY = y * CHIP_SIZE;
               DrawGraph(drawX, drawY, blockHandle, TRUE);
            }
        }
    }
}
