#include "Kuribo.h"
#include <DxLib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

Kuribo::Kuribo() : chipSize(0.0f)
{
    for (int i = 0; i < 3; i++)
        kuriboHandles[i] = -1;

    for (int i = 0; i < 4; i++)
        nokonokoHandles[i] = -1;

    for (int i = 0; i < 2; i++)
        nokonokoShellHandles[i] = -1;
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

    // ノコノコの甲羅画像（左=転がる、右=停止）
    LoadDivGraph(
        "assets/assets/Images/Enemy/nokonoko_revival.png",
        2, 2, 1,
        32, 32,
        nokonokoShellHandles
    );

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
                data.normalHeight = data.height; // ノコノコの通常時の高さを保存
                data.y = groundY * chipSize - data.height;
                data.vy = 0.0f;                        // ★追加：初期落下速度は0
                data.type = id;
                data.dir = -1;          // ★追加：最初は左向き（-1）
                data.isActive = false;
                data.isDead = false;    // ★追加：生存状態
                data.isSquashed = false; // 踏みつけアニメーション中か
                data.squashStartTime = 0; // 踏みつけ開始時刻
                data.shellState = 0; // 0=通常, 1=停止した甲羅, 2=転がる甲羅
                data.shellStartTime = 0; // 甲羅になった時刻

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
    const float MOVE_SPEED = 0.5f;
    const float GRAVITY = 0.5f;
    const float MAX_FALL_SPEED = 10.0f;
    const float ENEMY_WIDTH = 32.0f;

    // スペースキーを押した瞬間に、画面内の敵を踏みつける（テスト用）
    static bool previousSpace = false;
    bool spaceNow = (CheckHitKey(KEY_INPUT_SPACE) != 0);

    if (spaceNow && !previousSpace)
    {
        bool testDone = false;

        // まずノコノコを優先してテストする
        for (EnemyData& data : enemyDataList)
        {
            if (data.isDead || !data.isActive || data.type != 55) continue;

            if (data.shellState == 0)
            {
                // 歩いているノコノコを踏んで甲羅にする
                data.shellState = 1;
                data.shellStartTime = GetNowCount();
                data.y += data.normalHeight - 24;
                data.height = 24;
            }
            else if (data.shellState == 1)
            {
                // 停止中の甲羅をテスト用に右へ蹴る
                data.shellState = 2;
                data.dir = 1;
            }
            else
            {
                // 転がる甲羅を踏んだ扱いにして停止させる
                data.shellState = 1;
                data.shellStartTime = GetNowCount();
            }

            testDone = true;
            break;
        }

        // ノコノコがいなければクリボーを踏む
        if (!testDone)
        {
            for (EnemyData& data : enemyDataList)
            {
                if (!data.isDead && data.isActive && !data.isSquashed && data.type == 88)
                {
                    data.isSquashed = true;
                    data.squashStartTime = GetNowCount();
                    break;
                }
            }
        }
    }
    previousSpace = spaceNow;

    // 敵の移動・壁・地面との当たり判定
    for (EnemyData& data : enemyDataList)
    {
        if (data.isDead) continue;

        // 踏みつけアニメーション中は移動させず、時間が経ったら消す
        if (data.isSquashed)
        {
            if (GetNowCount() - data.squashStartTime >= 400)
            {
                data.isDead = true;
            }
            continue;
        }

        // 停止中の甲羅は5秒後にノコノコへ戻る
        if (data.type == 55 && data.shellState == 1 &&
            GetNowCount() - data.shellStartTime >= 5000)
        {
            data.y -= data.normalHeight - 24;
            data.height = data.normalHeight;
            data.shellState = 0;
        }

        // 画面内に入ったら起動
        if (!data.isActive)
        {
            if (data.x <= cameraX + SCREEN_WIDTH &&
                data.x + ENEMY_WIDTH >= cameraX)
            {
                data.isActive = true;
            }
        }

        if (data.isActive)
        {
            // 1. 横移動（転がる甲羅は速く移動する）
            float moveSpeed = MOVE_SPEED;
            if (data.type == 55 && data.shellState == 2)
            {
                moveSpeed = 5.0f;
            }
            else if (data.type == 55 && data.shellState == 1)
            {
                moveSpeed = 0.0f;
            }

            data.x += moveSpeed * data.dir;

            // 横の壁との当たり判定
            int topMapY = (int)((data.y + 4.0f) / chipSize);
            int bottomMapY = (int)((data.y + data.height - 4.0f) / chipSize);

            if (data.dir == -1)
            {
                // 左へ移動中
                int leftMapX = (int)(data.x / chipSize);
                bool hitWall = false;

                for (int my = topMapY; my <= bottomMapY; my++)
                {
                    if (my >= 0 && my < (int)blockMap.size() &&
                        leftMapX >= 0 &&
                        leftMapX < (int)blockMap[my].size() &&
                        blockMap[my][leftMapX] != 0)
                    {
                        hitWall = true;
                        break;
                    }
                }

                if (hitWall)
                {
                    data.x = (leftMapX + 1) * chipSize;
                    data.dir = 1;
                }
            }
            else if (data.dir == 1)
            {
                // 右へ移動中
                int rightMapX = (int)((data.x + ENEMY_WIDTH) / chipSize);
                bool hitWall = false;

                for (int my = topMapY; my <= bottomMapY; my++)
                {
                    if (my >= 0 && my < (int)blockMap.size() &&
                        rightMapX >= 0 &&
                        rightMapX < (int)blockMap[my].size() &&
                        blockMap[my][rightMapX] != 0)
                    {
                        hitWall = true;
                        break;
                    }
                }

                if (hitWall)
                {
                    data.x = rightMapX * chipSize - ENEMY_WIDTH;
                    data.dir = -1;
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
                    bool isGroundLeft =
                        (leftMapX >= 0 &&
                            leftMapX < (int)blockMap[mapY].size() &&
                            blockMap[mapY][leftMapX] != 0);

                    bool isGroundRight =
                        (rightMapX >= 0 &&
                            rightMapX < (int)blockMap[mapY].size() &&
                            blockMap[mapY][rightMapX] != 0);

                    if (isGroundLeft || isGroundRight)
                    {
                        data.y = mapY * chipSize - data.height;
                        data.vy = 0.0f;
                    }
                }
            }
        }
    }

    // 4. 転がる甲羅と敵の当たり判定
    for (EnemyData& shell : enemyDataList)
    {
        if (shell.isDead || !shell.isActive) continue;
        if (shell.type != 55 || shell.shellState != 2) continue;

        for (EnemyData& target : enemyDataList)
        {
            if (&shell == &target) continue;
            if (target.isDead || !target.isActive || target.isSquashed) continue;

            float shellLeft = shell.x + 2.0f;
            float shellRight = shell.x + 30.0f;
            float shellTop = shell.y + 4.0f;
            float shellBottom = shell.y + shell.height;

            float targetLeft = target.x + 4.0f;
            float targetRight = target.x + 28.0f;
            float targetTop = target.y + 4.0f;
            float targetBottom = target.y + target.height;

            if (shellLeft < targetRight && shellRight > targetLeft &&
                shellTop < targetBottom && shellBottom > targetTop)
            {
                target.isDead = true; // 甲羅に当たった敵を倒す
            }
        }
    }

    // 5. クリボー同士の当たり判定
    for (int i = 0; i < (int)enemyDataList.size(); i++)
    {
        for (int j = i + 1; j < (int)enemyDataList.size(); j++)
        {
            EnemyData& enemy1 = enemyDataList[i];
            EnemyData& enemy2 = enemyDataList[j];

            // 倒された敵、起動していない敵は判定しない
            if (enemy1.isDead || enemy2.isDead) continue;
            if (enemy1.isSquashed || enemy2.isSquashed) continue;
            if (!enemy1.isActive || !enemy2.isActive) continue;

            // クリボー同士だけを判定する
            if (enemy1.type != 88 || enemy2.type != 88) continue;

            // 敵1の当たり判定（上下左右4px内側）
            float e1Left = enemy1.x + 4.0f;
            float e1Right = enemy1.x + ENEMY_WIDTH - 4.0f;
            float e1Top = enemy1.y + 4.0f;
            float e1Bottom = enemy1.y + enemy1.height - 4.0f;

            // 敵2の当たり判定（上下左右4px内側）
            float e2Left = enemy2.x + 4.0f;
            float e2Right = enemy2.x + ENEMY_WIDTH - 4.0f;
            float e2Top = enemy2.y + 4.0f;
            float e2Bottom = enemy2.y + enemy2.height - 4.0f;

            // 縦方向と横方向の重なりを確認
            bool hitY = e1Top < e2Bottom && e1Bottom > e2Top;
            bool hitX = e1Left < e2Right && e1Right > e2Left;

            if (hitX && hitY)
            {
                // 左右の敵を判定
                if (enemy1.x < enemy2.x)
                {
                    float overlap = e1Right - e2Left;

                    // 重なりを解消
                    enemy1.x -= overlap / 2.0f;
                    enemy2.x += overlap / 2.0f;

                    // 進行方向を反転
                    enemy1.dir = -1;
                    enemy2.dir = 1;
                }
                else
                {
                    float overlap = e2Right - e1Left;

                    // 重なりを解消
                    enemy2.x -= overlap / 2.0f;
                    enemy1.x += overlap / 2.0f;

                    // 進行方向を反転
                    enemy2.dir = -1;
                    enemy1.dir = 1;
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

        // ノコノコの甲羅状態
        if (data.type == 55 && data.shellState != 0)
        {
            int handle = -1;

            if (data.shellState == 1)
            {
                // 右側：止まっている甲羅
                handle = nokonokoShellHandles[1];
            }
            else if (data.shellState == 2)
            {
                // 左側：転がっている甲羅
                handle = nokonokoShellHandles[0];
            }

            if (handle != -1)
            {
                // 甲羅は32x32画像。地面に接するように8px上へ描画
                DrawGraph(screenX, screenY - 8, handle, TRUE);
            }

            continue;
        }

        int animIndex = data.isActive ? ((GetNowCount() / 300) % 2) : 0;
        int handle = (data.type == 88) ? kuriboHandles[animIndex] : nokonokoHandles[animIndex];

        // 踏みつけられたクリボーは、横につぶして描画する
        if (data.isSquashed)
        {
            int squashY = screenY + data.height - 12;
            DrawExtendGraph(
                screenX,
                squashY,
                screenX + 32,
                screenY + data.height,
                handle,
                TRUE
            );
        }
        else if (data.dir == -1)
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

    for (int i = 0; i < 2; i++)
    {
        if (nokonokoShellHandles[i] != -1)
        {
            DeleteGraph(nokonokoShellHandles[i]);
            nokonokoShellHandles[i] = -1;
        }
    }
}

int Kuribo::CheckPlayerCollision(float playerX, float playerY, int playerW, int playerH, float& playerVy)
{
    for (EnemyData& data : enemyDataList)
    {
        if (data.isDead || !data.isActive || data.isSquashed) continue;

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
            bool stomp = playerVy > 0.0f &&
                (pBottom - playerVy) <= (data.y + data.height * 0.5f);

            if (data.type == 88)
            {
                if (stomp)
                {
                    data.isSquashed = true;
                    data.squashStartTime = GetNowCount();
                    playerVy = -8.0f;
                    return 1;
                }
                return 2;
            }

            // ノコノコの処理
            if (data.type == 55)
            {
                if (data.shellState == 0 && stomp)
                {
                    // 歩いているノコノコを踏むと甲羅になる
                    data.shellState = 1;
                    data.shellStartTime = GetNowCount();
                    data.y += data.normalHeight - 24;
                    data.height = 24;
                    playerVy = -8.0f;
                    return 1;
                }

                if (data.shellState == 1)
                {
                    if (stomp)
                    {
                        // 停止中の甲羅を上から踏んでも、その場に留まる
                        data.shellStartTime = GetNowCount();
                        playerVy = -8.0f;
                    }
                    else
                    {
                        // 停止中の甲羅に横から触れると蹴る
                        data.shellState = 2;
                        data.dir = (playerX + playerW * 0.5f < data.x + 16.0f) ? 1 : -1;
                    }
                    return 1;
                }

                if (data.shellState == 2 && stomp)
                {
                    // 転がっている甲羅を踏むと停止する
                    data.shellState = 1;
                    data.shellStartTime = GetNowCount();
                    playerVy = -8.0f;
                    return 1;
                }

                // 転がる甲羅に横から触れた場合はダメージ
                return 2;
            }
        }
    }

    return 0; // 接触なし
}
