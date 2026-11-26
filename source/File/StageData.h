#pragma once
#include <array>
#include <string>

class StageData
{
public:
    // マップ定数
    static constexpr int MAP_HEIGHT = 16;
    static constexpr int MAP_WIDTH = 224;
    static constexpr int CHIP_SIZE = 32;
    static constexpr int MAX_CHIP_TYPES = 100;
    bool IsSolidBlock(int chipX, int chipY) const;
    // 2次元配列の型エイリアス
    using MapGrid = std::array<std::array<int, MAP_WIDTH>, MAP_HEIGHT>;

public:
    StageData();
    ~StageData();

    // CSVファイルの読み込み
    bool LoadBgCSV(const std::string& fileName);
    bool LoadBlockCSV(const std::string& fileName);
    void BreakBlock(int chipX, int chipY);
    // 画像素材の読み込み・解放
    bool LoadMapImages();
    void UnloadMapImages();

    // 更新・描画
    void UpdateMap();
    void DrawMap(int cameraX, int screenWidth) const;

    // 外部（当たり判定など）からマップデータを参照するためのゲッター
    const MapGrid& GetBgData() const { return m_bgData; }
    const MapGrid& GetBlockData() const { return m_blockData; }
    int GetBlockChip(int x, int y) const;


private:
    // 共通のCSV読み込み内部関数
    bool LoadCSVInternal(const std::string& fileName, MapGrid& targetMap);

private:
    MapGrid m_bgData{};
    MapGrid m_blockData{};

    std::array<int, MAX_CHIP_TYPES> m_chipHandles{};
    std::array<int, 4> m_hatenaHandles{};

    int m_animTimer = 0;
    int m_animFrame = 0;
};