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

    // ノコノコ画像（横2コマ・縦1コマで画像サイズに合わせて自動分割）
    int nokoW = 0, nokoH = 0;
    int tempHandle = LoadGraph("assets/assets/Images/Enemy/nokonoko.png");
    if (tempHandle != -1)
    {
        GetGraphSize(tempHandle, &nokoW, &nokoH);
        DeleteGraph(tempHandle);

        // 横に2体並んでいるので、1コマの幅は nokoW / 2、高さは nokoH
        LoadDivGraph(
            "assets/assets/Images/Enemy/nokonoko.png",
            2, 2, 1, nokoW / 2, nokoH,
            nokonokoHandles
        );
    }

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
                for (int by = y; by < (int)blockMap.size(); by++)
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
                // ノコノコは64px、クリボーは32px
                if (id == 55)
                {
                    // ノコノコを32px上に移動
                    data.y = groundY * chipSize - 64;
                }
                else
                {
                    data.y = groundY * chipSize - 32;
                }
                data.type = id;
                data.isActive = false; // 最初は待機状態

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
    const float SCREEN_WIDTH = 1024.0f; // Draw関数に合わせて1024に設定
    const float MOVE_SPEED = 1.5f;      // 移動速度（お好みで調整してください）

    for (EnemyData& data : enemyDataList)
    {
        // まだ動き出していない場合、画面内（右端より内側）に入ったら起動
        if (!data.isActive)
        {
            if (data.x <= cameraX + SCREEN_WIDTH && data.x + 32.0f >= cameraX)
            {
                data.isActive = true;
            }
        }

        // 起動済みなら左へ移動
        if (data.isActive)
        {
            data.x -= MOVE_SPEED;
        }
    }
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

        // 起動中のみ 0 と 1 を交互に切り替える
        int animIndex = data.isActive ? ((GetNowCount() / 200) % 2) : 0;

        if (data.type == 88)
        {
            // クリボー
            DrawGraph(screenX, screenY, kuriboHandles[animIndex], TRUE);
        }
        else if (data.type == 55)
        {
            // ノコノコ
            DrawGraph(screenX, screenY, nokonokoHandles[animIndex], TRUE);
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