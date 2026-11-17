#pragma once
#include <vector>

struct EnemyData
{
    float x;
    float y;
    float vy;
    int height;
    int type;       // 88: クリボー, 55: ノコノコ
    int dir;        // ★追加：移動方向（-1: 左, 1: 右）
    bool isActive;  // 画面に入って起動したか
    bool isDead;    // ★追加：倒された（または画面外に消えた）か
};

class Kuribo
{
public:
    Kuribo();
    ~Kuribo();

    bool Init(const char* enemyFile, const char* blockFile, float chipSize);
    void Update(float cameraX);
    void Draw(float cameraX);
    void Clear();

    // ★追加：プレイヤーとの当たり判定
    // 戻り値: 0=接触なし, 1=敵を踏んだ, 2=プレイヤーがダメージを受けた
    int CheckPlayerCollision(float playerX, float playerY, int playerW, int playerH, float& playerVy);

private:
    int kuriboHandles[3];
    int nokonokoHandles[4];
    std::vector<EnemyData> enemyDataList;

    std::vector<std::vector<int>> blockMap;
    float chipSize;
};