#include"Item.h"
#include "DxLib.h"
Item::Item(ItemType itemType, float startX, float startY, int handle)
    : type(itemType)
    , x(startX)
    , y(startY)
    , vx(0.0f)
    , vy(0.0f)
    , width(32.0f)
    , height(32.0f)
    , isAlive(true)
    , graphHandle(handle)
{
    // キノコ系なら最初から右へ移動させる
    if (type == ItemType::Mushroom || type == ItemType::OneUp)
    {
        vx = 100.0f; // 1秒間に100ピクセル右へ移動（速度はお好みで調整）
    }
}
void Item::Update(float delta_time, const StageData& stage)
{
    if (!isAlive) return;
}
void Item::Draw(int cameraX)const
{
    if (!isAlive || graphHandle == -1) return;

    int drawX = static_cast<int>(x) - cameraX;
    int drawY = static_cast<int>(y);
    DrawGraph(drawX, drawY, graphHandle, TRUE);
}
void Item::OnCollideWithPlayer()
{
    // プレイヤーに取られたら消える
    isAlive = false;
}