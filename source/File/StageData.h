#ifndef STAGE_DATA_H
#define STAGE_DATA_H

#define CHIP_SIZE 32
#define MAP_WIDTH 212
#define MAP_HEIGHT 15

extern int g_mapData[MAP_HEIGHT][MAP_WIDTH];

bool LoadMapCSV(const char* fileName);
bool LoadMapImages();      
void UnloadMapImages();    
void DrawMap(int cameraX, int screenWidth); // ˆø”‚ğ•ÏX

#endif