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
    this->chipSize = chipSize; // ★追加：chipSizeをメンバ変数に保存

    // クリボー画像
    LoadDivGraph("assets/assets/Images/Enemy/kuribo.png", 3, 3, 1, 32, 32, kuriboHandles);

    // ノコノコ画像
    int nokoW = 0, nokoH = 64;
    int tempHandle = LoadGraph("assets/assets/Images/Enemy/nokonoko.png");
    if (tempHandle != -1)
    {
        GetGraphSize(tempHandle, &nokoW, &nokoH);
        DeleteGraph(tempHandle);
        LoadDivGraph("assets/assets/Images/Enemy/nokonoko.png", 2, 2, 1, nokoW / 2, nokoH, nokonokoHandles);
    }

    std::ifstream enemyCsv(enemyFile);
    if (!enemyCsv.is_open())
        return false;

    // ★変更：ローカルの blockMap 宣言を削除し、メンバ変数の blockMap を使う
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
            try { row.push_back(std::stoi(cell)); }
            catch (...) { row.push_back(0); }
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
            try { id = std::stoi(cell); }
            catch (...) { id = 0; }

            if (id == 88 || id == 55)
            {
                int groundY = -1;
                for (int by = y; by < (int)blockMap.size(); by++)
                {
                    if (x < (int)blockMap[by].size() && blockMap[by][x] != 0)
                    {
                        groundY = by;
                        break;
                    }
                }
                if (groundY == -1)
                {
                    groundY = y + 1;
                }

                EnemyData data;
                data.x = x * chipSize;
                data.height = (id == 55) ? nokoH : 32; // ★追加：高さを保存
                data.y = groundY * chipSize - data.height;
                data.vy = 0.0f;                        // ★追加：初期落下速度は0
                data.type = id;
                data.dir = -1;          // ★追加：最初は左向き（-1）
                data.isActive = false;
                data.isDead = false;    // ★追加：生存状態

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
    const float SCREEN_WIDTH = 1024.0f;
    const float MOVE_SPEED = 0.8f;
    const float GRAVITY = 0.5f;
    const float MAX_FALL_SPEED = 10.0f;

    for (EnemyData& data : enemyDataList)
    {
        if (data.isDead) continue; // 倒された敵はスキップ

        // 画面内に入ったら起動
        if (!data.isActive)
        {
            if (data.x <= cameraX + SCREEN_WIDTH && data.x + 32.0f >= cameraX)
            {
                data.isActive = true;
            }
        }

        if (data.isActive)
        {
            // 1. 横移動（dir が -1 なら左、1 なら右）
            data.x += MOVE_SPEED * data.dir;

            // ★追加：横の壁（ブロック・土管）との当たり判定
            // 足元ギリギリは地面と誤認しないよう、上端+4px ? 下端-4px の範囲でチェック
            int topMapY = (int)((data.y + 4.0f) / chipSize);
            int bottomMapY = (int)((data.y + data.height - 4.0f) / chipSize);

            if (data.dir == -1) // 左へ移動中
            {
                int leftMapX = (int)(data.x / chipSize);
                bool hitWall = false;
                for (int my = topMapY; my <= bottomMapY; my++)
                {
                    if (my >= 0 && my < (int)blockMap.size() &&
                        leftMapX >= 0 && leftMapX < (int)blockMap[my].size() &&
                        blockMap[my][leftMapX] != 0)
                    {
                        hitWall = true;
                        break;
                    }
                }
                if (hitWall)
                {
                    data.x = (leftMapX + 1) * chipSize; // 壁の右端に押し戻す
                    data.dir = 1;                       // 右向きに反転
                }
            }
            else if (data.dir == 1) // 右へ移動中
            {
                int rightMapX = (int)((data.x + 32.0f) / chipSize);
                bool hitWall = false;
                for (int my = topMapY; my <= bottomMapY; my++)
                {
                    if (my >= 0 && my < (int)blockMap.size() &&
                        rightMapX >= 0 && rightMapX < (int)blockMap[my].size() &&
                        blockMap[my][rightMapX] != 0)
                    {
                        hitWall = true;
                        break;
                    }
                }
                if (hitWall)
                {
                    data.x = rightMapX * chipSize - 32.0f; // 壁の左端に押し戻す
                    data.dir = -1;                         // 左向きに反転
                }
            }

            // 2. 重力による落下計算
            data.vy += GRAVITY;
            if (data.vy > MAX_FALL_SPEED)
            {
                data.vy = MAX_FALL_SPEED;
            }
            data.y += data.vy;

            // 3. 足元の地面判定
            if (data.vy >= 0.0f)
            {
                float footY = data.y + data.height;
                int mapY = (int)(footY / chipSize);
                int leftMapX = (int)((data.x + 4.0f) / chipSize);
                int rightMapX = (int)((data.x + 28.0f) / chipSize);

                if (mapY >= 0 && mapY < (int)blockMap.size())
                {
                    bool isGroundLeft = (leftMapX >= 0 && leftMapX < (int)blockMap[mapY].size() && blockMap[mapY][leftMapX] != 0);
                    bool isGroundRight = (rightMapX >= 0 && rightMapX < (int)blockMap[mapY].size() && blockMap[mapY][rightMapX] != 0);

                    if (isGroundLeft || isGroundRight)
                    {
                        data.y = mapY * chipSize - data.height;
                        data.vy = 0.0f;
                    }
                }
            }
        }
    }
}

// 描画処理
void Kuribo::Draw(float cameraX)
{
    for (const EnemyData& data : enemyDataList)
    {
        if (data.isDead) continue; // 倒された敵は描画しない

        int screenX = (int)(data.x - cameraX);
        int screenY = (int)data.y;

        if (screenX < -32 || screenX > 1024)
            continue;

        int animIndex = data.isActive ? ((GetNowCount() / 300) % 2) : 0;
        int handle = (data.type == 88) ? kuriboHandles[animIndex] : nokonokoHandles[animIndex];

        if (data.dir == -1)
        {
            DrawGraph(screenX, screenY, handle, TRUE);     // 左向き
        }
        else
        {
            DrawTurnGraph(screenX, screenY, handle, TRUE); // 右向き（左右反転）
        }
    }
}

// 解放処理
void Kuribo::Clear()
{
    enemyDataList.clear();
    blockMap.clear();

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

int Kuribo::CheckPlayerCollision(float playerX, float playerY, int playerW, int playerH, float& playerVy)
{
    for (EnemyData& data : enemyDataList)
    {
        if (data.isDead || !data.isActive) continue;

        // 当たり判定を少し甘め（内側4px）にして理不尽な被弾を防ぐ
        float eLeft = data.x + 4.0f;
        float eRight = data.x + 28.0f;
        float eTop = data.y + 4.0f;
        float eBottom = data.y + data.height;

        float pLeft = playerX + 4.0f;
        float pRight = playerX + playerW - 4.0f;
        float pTop = playerY + 4.0f;
        float pBottom = playerY + playerH;

        // 矩形同士が重なっているかチェック
        if (pLeft < eRight && pRight > eLeft && pTop < eBottom && pBottom > eTop)
        {
            // プレイヤーが落下中、かつプレイヤーの足元が敵の上半分にあるなら「踏みつけ」
            if (playerVy > 0.0f && (pBottom - playerVy) <= (data.y + data.height * 0.5f))
            {
                data.isDead = true;   // 敵を倒す
                playerVy = -8.0f;     // プレイヤーを少し上に跳ねさせる
                return 1;             // 踏みつけ成功
            }
            else
            {
                return 2;             // 横や下から当たった（プレイヤー被弾）
            }
        }
    }

    return 0; // 接触なし
}