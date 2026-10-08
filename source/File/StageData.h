#pragma once#pragma once

#include <vector>
#include <unordered_map>
#include <string>

// パネル（ブロック）の種類
enum class ePanelID
{
	eNone = 0,  // 空白・何もない
	eBlock = 1, // 床・ブロック
	// 必要に応じて追加（例: eItemBlock = 2, ePipe = 3 など）
};

// 座標・ベクトル構造体（既存のVector2定義がある場合はそれを使用）
struct Vector2
{
	float x = 0.0f;
	float y = 0.0f;

	Vector2() = default;
	Vector2(float _x, float _y) : x(_x), y(_y) {}

	Vector2 operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}
};

// ステージデータを管理するシングルトンクラス
class StageData
{
public:
	// 定数定義
	static const float CHIP_SIZE;        // 1マスのサイズ (24.0f)
	static const float STAGE_OFFSET_X;   // オフセットX (0.0f)
	static const float STAGE_OFFSET_Y;   // オフセットY (0.0f)
	static const Vector2 STAGE_OFFSET;

public:
	// シングルトンインスタンスの取得・破棄
	static StageData* Get();
	static void Destroy();

	// ステージ全データの取得
	const std::vector<std::vector<ePanelID>>& GetAll();

	// 指定座標のパネルID取得
	const ePanelID GetPanelData(const Vector2& location) const;

	// 指定座標にパネルIDを書き込み（単一マス）
	void SetPanelData(ePanelID id, const Vector2& location);

	// ワールド座標から配列のインデックス (x, y) へ変換
	void ConvertToIndex(const Vector2& location, int& x, int& y) const;

	// マップデータの読み込み（CSV対応）
	void Load();

	// マップデータの解放
	void Unload();

private:
	// プライベートコンストラクタ（外部からの new を禁止）
	StageData() = default;
	~StageData() = default;

	// コピー・代入の禁止
	StageData(const StageData&) = delete;
	StageData& operator=(const StageData&) = delete;

private:
	static StageData* instance;               // シングルトンインスタンス
	std::vector<std::vector<ePanelID>> data;  // 2次元のマップデータ [y][x]
};