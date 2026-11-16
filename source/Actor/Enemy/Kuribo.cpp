#include "Kuribo.h"
#include <DxLib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

Kuribo::Kuribo()
{
    for (int i = 0; i < 3; i++)
        kuriboHandles[i] = -1;

    for (int i = 0; i < 4; i++)
        nokonokoHandles[i] = -1;
}

Kuribo::~Kuribo()
{
    Clear();
}

bool Kuribo::Init(
    const char* enemyFile,
    const char* blockFile,
    float chipSize)
{
    Clear();

    // クリボー画像（32×32を3コマ）
    LoadDivGraph(
        "assets/assets/Images/Enemy/kuribo.png",
        3, 3, 1, 32, 32,
        kuriboHandles
    );

    // ノコノコ画像（32×32を4コマ）
    LoadDivGraph(
        "assets/assets/Images/Enemy/nokonoko.png",
        4, 2, 2, 32, 32,
        nokonokoHandles
    );

    // 敵CSV読み込み
    std::ifstream enemyCsv(enemyFile);

    if (!enemyCsv.is_open())
        return false;

    // 地面CSV読み込み
    std::vector<std::vector<int>> blockMap;

    std::ifstream blockCsvStream(blockFile);

    if (!blockCsvStream.is_open())
        return false;

    std::string line;

    while (std::getline(blockCsvStream, line))
    {
        std::stringstream ss(line);
        std::string cell;

        std::vector<int> row;

        while (std::getline(ss, cell, ','))
        {
            try
            {
                row.push_back(std::stoi(cell));
            }
            catch (...)
            {
                row.push_back(0);
            }
        }

        blockMap.push_back(row);
    }

    // 敵の配置
    int y = 0;

    while (std::getline(enemyCsv, line))
    {
        std::stringstream ss(line);
        std::string cell;

        int x = 0;

        while (std::getline(ss, cell, ','))
        {
            int id = 0;

            try
            {
                id = std::stoi(cell);
            }
            catch (...)
            {
                id = 0;
            }

            if (id == 88 || id == 55)
            {
                int groundY = -1;

                // 敵の下にある地面を探す
                for (int by = y + 1; by < (int)blockMap.size(); by++)
                {
                    if (x < (int)blockMap[by].size() &&
                        blockMap[by][x] != 0)
                    {
                        groundY = by;
                        break;
                    }
                }

                // 地面が見つからない場合は元の行を使用
                if (groundY == -1)
                {
                    groundY = y + 1;
                }

                EnemyData data;

                data.x = x * chipSize;
                data.y = groundY * chipSize - 32.0f;
                data.type = id;

                enemyDataList.push_back(data);
            }

            x++;
        }

        y++;
    }

    return true;
}

// 更新処理
void Kuribo::Update(float cameraX)
{
    // 敵は移動させない
    // カメラ移動に合わせて描画位置だけ変更する
}

// 描画処理
void Kuribo::Draw(float cameraX)
{
    for (const EnemyData& data : enemyDataList)
    {
        int screenX = (int)(data.x - cameraX);
        int screenY = (int)data.y;

        if (screenX < -32 || screenX > 1024)
            continue;

        if (data.type == 88)
        {
            // クリボー
            DrawGraph(
                screenX,
                screenY,
                kuriboHandles[0],
                TRUE
            );
        }
        else if (data.type == 55)
        {
            // ノコノコ
            DrawGraph(
                screenX,
                screenY,
                nokonokoHandles[0],
                TRUE
            );
        }
    }
}

// 解放処理
void Kuribo::Clear()
{
    enemyDataList.clear();

    for (int i = 0; i < 3; i++)
    {
        if (kuriboHandles[i] != -1)
        {
            DeleteGraph(kuriboHandles[i]);
            kuriboHandles[i] = -1;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (nokonokoHandles[i] != -1)
        {
            DeleteGraph(nokonokoHandles[i]);
            nokonokoHandles[i] = -1;
        }
    }
}