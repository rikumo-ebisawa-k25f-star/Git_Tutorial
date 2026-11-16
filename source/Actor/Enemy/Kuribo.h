#pragma once

#include <vector>

class Kuribo
{
public:
    Kuribo();
    ~Kuribo();

    // 初期化
    bool Init(
        const char* enemyFile,
        const char* blockFile,
        float chipSize
    );

    // 更新
    void Update(float cameraX);

    // 描画
    void Draw(float cameraX);

    // 解放
    void Clear();

private:
    struct EnemyData
    {
        float x;
        float y;
        int type; // 88：クリボー 55：ノコノコ
    };

    std::vector<EnemyData> enemyDataList;

    // 画像ハンドル
    int kuriboHandles[3];
    int nokonokoHandles[4];
};