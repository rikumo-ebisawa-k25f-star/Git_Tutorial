
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
   // --- 背景（雲・草・山・ポール・空・城） ---
    m_chipHandles[1] = LoadGraph("assets/assets/Images/cloud2.png");            // 雲
    m_chipHandles[2] = LoadGraph("assets/assets/Images/cloud3.png");            // 雲
    m_chipHandles[3] = LoadGraph("assets/assets/Images/cloud4.png");            // 雲
    m_chipHandles[4] = LoadGraph("assets/assets/Images/cloud5.png");            // 雲
    m_chipHandles[5] = LoadGraph("assets/assets/Images/cloud6.png");            // 雲

    m_chipHandles[12] = LoadGraph("assets/assets/Images/ha0.png");               // 草
    m_chipHandles[13] = LoadGraph("assets/assets/Images/ha1.png");               // 草
    m_chipHandles[14] = LoadGraph("assets/assets/Images/ha2.png");               // 草

    m_chipHandles[15] = LoadGraph("assets/assets/Images/mountain_left.png");     // 山
    m_chipHandles[16] = LoadGraph("assets/assets/Images/mountain_right.png");    // 山
    m_chipHandles[17] = LoadGraph("assets/assets/Images/mountain_surface.png");  // 山
    m_chipHandles[18] = LoadGraph("assets/assets/Images/mountain_surface1.png"); // 山
    m_chipHandles[19] = LoadGraph("assets/assets/Images/mountain_surface2.png"); // 山
    m_chipHandles[20] = LoadGraph("assets/assets/Images/mountain_up.png");       // 山

    m_chipHandles[21] = LoadGraph("assets/assets/Images/pole.png");              // ポールの頂点
    m_chipHandles[22] = LoadGraph("assets/assets/Images/pole_down.png");         // ポール
    m_chipHandles[24] = LoadGraph("assets/assets/Images/sora.png");              // 空
    m_chipHandles[40] = LoadGraph("assets/assets/Images/siro1.png");             // 城
    m_chipHandles[41] = LoadGraph("assets/assets/Images/cloud1.png");            // 雲

    // --- ブロック・土管 ---
    m_chipHandles[6] = LoadGraph("assets/assets/Images/dokan_left_down.png");   // 土管下側左
    m_chipHandles[7] = LoadGraph("assets/assets/Images/dokan_left_up.png");     // 土管上側左
    m_chipHandles[8] = LoadGraph("assets/assets/Images/dokan_right_down.png");  // 土管下側右
    m_chipHandles[9] = LoadGraph("assets/assets/Images/dokan_right_up.png");    // 土管上側右

    m_chipHandles[33] = LoadGraph("assets/assets/Images/Block/block.png");       // レンガ
    m_chipHandles[34] = LoadGraph("assets/assets/Images/Block/floor.png");       // 地面
    m_chipHandles[36] = LoadGraph("assets/assets/Images/Block/kai_block.png");   // 階段
    LoadDivGraph("assets/assets/Images/Block/hatena.png", 4, 4, 1, CHIP_SIZE, CHIP_SIZE, m_hatenaHandles.data());//ハテナブロック

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
           /* else
            {
                DrawBox(drawX, drawY, drawX + CHIP_SIZE, drawY + CHIP_SIZE, skyColor, TRUE);
            }*/

            // 2. ブロックレイヤーを描画
            int blockType = m_blockData[y][x];
            if (blockType == 0) continue;

            // ハテナブロックの描画
            if (blockType == 37)
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
bool StageData::IsSolidBlock(int chipX, int chipY) const
{
    int id = GetBlockChip(chipX, chipY);

    // 0（空気）や、ゴールポール（21, 22）など「すり抜けるもの」以外は壁（true）とする
    if (id == 0 || id == 21 || id == 22)
    {
        return false;
    }
    return true;
}