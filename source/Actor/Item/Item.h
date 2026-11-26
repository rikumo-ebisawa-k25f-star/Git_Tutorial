#pragma once
class StageData;
enum class ItemType {
    Mushroom,   // スーパーキノコ
    OneUp,      // 1UPキノコ
    Flower,     // ファイアフラワー
    Coin        // コイン
};

class Item {
private:
    ItemType type;      // アイテムの種類
    float x, y;         // 座標
    float vx, vy;       // 移動速度
    float width, height;// 当たり判定サイズ（32x32など）
    bool isAlive;       // 画面に存在しているか
    int graphHandle;    // 画像ハンドル

public:
    Item(ItemType itemType, float startX, float startY, int handle = -1);

    void Update(float delta_second, const StageData& stage); // マップ判定込みで更新
    void Draw(int cameraX) const;                            // カメラ位置に合わせて描画
    void OnCollideWithPlayer();                              // プレイヤーと当たった時の処理

    // --- 外部（InGameSceneやMario）から状態を取得する関数 ---
    bool IsAlive() const { return isAlive; }
    ItemType GetType() const { return type; }
    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetWidth() const { return width; }
    float GetHeight() const { return height; }
};