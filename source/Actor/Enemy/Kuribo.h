#pragma once
#include <vector>

struct EnemyData
{
    float x;
    float y;
    float vy;
    int height;
    int normalHeight; // 通常時の高さ
    int type;       // 88: クリボー, 55: ノコノコ
    int dir;        // 移動方向（-1: 左, 1: 右）
    bool isActive;  // 画面に入って起動したか
    bool isDead;    // 倒された、または画面外に消えたか
    bool isSquashed; // 踏みつけアニメーション中か
    int squashStartTime; // 踏みつけアニメーション開始時刻
    int shellState; // 0=通常, 1=停止した甲羅, 2=転がる甲羅
    int shellStartTime; // 甲羅になった時刻
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

    // プレイヤーとの当たり判定
    // 戻り値: 0=接触なし, 1=敵を踏んだ, 2=プレイヤーがダメージを受けた
    int CheckPlayerCollision(float playerX, float playerY, int playerW, int playerH, float& playerVy);

private:
    int kuriboHandles[3];
    int nokonokoHandles[4];
    int nokonokoShellHandles[2]; // 0=転がる甲羅, 1=停止した甲羅
    std::vector<EnemyData> enemyDataList;

    std::vector<std::vector<int>> blockMap;
    float chipSize;
};
