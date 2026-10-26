#pragma once
// アイテムの種類を定義（列挙型）
enum class ItemType {
    Mushroom,   // スーパーキノコ
    OneUp,      // 1UPキノコ
    Flower,     // ファイアフラワー
    Coin        // コイン
};

class Item {
private:
    ItemType type;    // このアイテムの種類
    float x, y;       // 座標
    float vx, vy;     // 移動速度
    bool isAlive;     // 画面に存在しているか

public:
    // コンストラクタ（生成時に種類と位置を指定する）
    Item(ItemType itemType, float startX, float startY);

    void Update(float delta_second); // 更新処理（移動など）
    void Draw();                     // 描画処理
    void OnCollideWithPlayer();      // プレイヤーと当たった時の処理
};