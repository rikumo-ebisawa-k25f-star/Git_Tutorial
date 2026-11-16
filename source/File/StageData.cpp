
#include "StageData.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <DxLib.h>

StageData::StageData()
{
    m_chipHandles.fill(-1);
    m_hatenaHandles.fill(-1);
}

StageData::~StageData()
{
    // インスタンス破棄時に確実に画像を解放
    UnloadMapImages();
}

bool StageData::LoadBgCSV(const std::string& fileName)
{
    return LoadCSVInternal(fileName, m_bgData);
}

bool StageData::LoadBlockCSV(const std::string& fileName)
{
    return LoadCSVInternal(fileName, m_blockData);
}

bool StageData::LoadCSVInternal(const std::string& fileName, MapGrid& targetMap)
{
    std::ifstream file(fileName);
    if (!file.is_open()) return false;

    // 配列をすべて0でクリア
    for (auto& row : targetMap)
    {
        row.fill(0);
    }

    std::string line;
    for (int y = 0; y < MAP_HEIGHT && std::getline(file, line); ++y)
    {
        std::stringstream ss(line);
        std::string token;
        int x = 0;

        while (x < MAP_WIDTH && std::getline(ss, token, ','))
        {
            try
            {
                targetMap[y][x] = std::stoi(token);
            }
            catch (...)
            {
                // 空文字や数値以外が入っていた場合は0にする
                targetMap[y][x] = 0;
            }
            ++x;
        }
    }

    return true;
}

bool StageData::LoadMapImages()
{
    // 二重読み込み防止のため一度解放
    UnloadMapImages();

    // 各チップIDに対応する画像をロード
    m_chipHandles[34] = LoadGraph("assets/assets/Images/Block/floor.png");      // 地面
    m_chipHandles[33] = LoadGraph("assets/assets/Images/Block/block.png");      // レンガ
    m_chipHandles[36] = LoadGraph("assets/assets/Images/Block/kai_block.png");  // 階段
    m_chipHandles[5] = LoadGraph("assets/assets/Images/dokan_left_up.png");
    m_chipHandles[6] = LoadGraph("assets/assets/Images/dokan_right_up.png");
    m_chipHandles[7] = LoadGraph("assets/assets/Images/dokan_left_down.png");
    m_chipHandles[8] = LoadGraph("assets/assets/Images/dokan_right_down.png");

    LoadDivGraph("assets/assets/Images/Block/hatena.png", 4, 4, 1, CHIP_SIZE, CHIP_SIZE, m_hatenaHandles.data());

    return true;
}

void StageData::UnloadMapImages()
{
    for (int& handle : m_chipHandles)
    {
        if (handle != -1)
        {
            DeleteGraph(handle);
            handle = -1;
        }
    }

    for (int& handle : m_hatenaHandles)
    {
        if (handle != -1)
        {
            DeleteGraph(handle);
            handle = -1;
        }
    }
}

void StageData::UpdateMap()
{
    ++m_animTimer;
    if (m_animTimer % 12 == 0)
    {
        m_animFrame = (m_animFrame + 1) % static_cast<int>(m_hatenaHandles.size());
    }
}

void StageData::DrawMap(int cameraX, int screenWidth) const
{
    // 画面に映るX座標の範囲だけを計算してループ回数を削減
    // std::max と std::min を ( ) で囲む
    int startX = (std::max)(0, cameraX / CHIP_SIZE);
    int endX = (std::min)(MAP_WIDTH - 1, (cameraX + screenWidth) / CHIP_SIZE);
    const unsigned int skyColor = GetColor(107, 136, 255);
    const unsigned int hatenaFallbackColor = GetColor(255, 255, 0);
    const unsigned int blockFallbackColor = GetColor(0, 200, 0);

    for (int y = 0; y < MAP_HEIGHT; ++y)
    {
        for (int x = startX; x <= endX; ++x)
        {
            int drawX = x * CHIP_SIZE - cameraX;
            int drawY = y * CHIP_SIZE;

            // 1. 背景レイヤーを描画
            int bgType = m_bgData[y][x];
            if (bgType >= 0 && bgType < MAX_CHIP_TYPES && m_chipHandles[bgType] != -1)
            {
                DrawGraph(drawX, drawY, m_chipHandles[bgType], TRUE);
            }
            else
            {
                DrawBox(drawX, drawY, drawX + CHIP_SIZE, drawY + CHIP_SIZE, skyColor, TRUE);
            }

            // 2. ブロックレイヤーを描画（0は空気なのでスキップ）
            int blockType = m_blockData[y][x];
            if (blockType == 0) continue;

            // ハテナブロックの描画
            if (blockType == 21 || blockType == 3)
            {
                if (m_hatenaHandles[m_animFrame] != -1)
                {
                    DrawGraph(drawX, drawY, m_hatenaHandles[m_animFrame], TRUE);
                }
                else
                {
                    DrawBox(drawX, drawY, drawX + CHIP_SIZE, drawY + CHIP_SIZE, hatenaFallbackColor, TRUE);
                }
            }
            else if (blockType > 0 && blockType < MAX_CHIP_TYPES && m_chipHandles[blockType] != -1)
            {
                DrawGraph(drawX, drawY, m_chipHandles[blockType], TRUE);
            }
            else
            {
                DrawBox(drawX, drawY, drawX + CHIP_SIZE, drawY + CHIP_SIZE, blockFallbackColor, TRUE);
            }
        }
    }
}

int StageData::GetBlockChip(int x, int y) const
{
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
    {
        return 0; // 範囲外は空気扱い
    }
    return m_blockData[y][x];
}