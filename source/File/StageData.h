
#pragma once

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

//#pragma once
//#pragma once
//
//#include "../Math/Vector2.h"
//#include <unordered_map>
//#include <vector>
///// <summary>
///// パネルのID
///// </summary>
//enum class ePanelID : unsigned char
//{
//	eNone,//なんもない
//	eBlock,//壁
//	eBranch,//分岐点
//	eGate,//ゲート
//};
//
///// <summary>
///// 進行方向の情報
///// </summary>
//enum class eAdjacentDirection : unsigned char
//{
//	eUp,
//	eDown,
//	eLeft,
//	eRight,
//};
//
///// <summary>
///// ステージデータ
///// </summary>
//class StageData final
//{
//public:
//	static const float CHIP_SIZE;//1ブロック分の大きさ
//	static const float STAGE_OFFSET_X;//ステーz委のずれている量（横）
//	static const float STAGE_OFFSET_Y;//ステージのずれている量（縦）
//	static const Vector2 STAGE_OFFSET;//ステージのずれている量
//
//private:
//	//自クラスポインタ
//	static StageData* instance;
//	//ステージデータ情報
//	std::vector<std::vector<ePanelID>> data;
//
//private:
//	StageData() = default;
//	StageData(const StageData&) = delete;
//	StageData& operator = (StageData&) = delete;
//	~StageData() = default;
//
//public:
//	static StageData* Get();//インスタンスの取得
//	static void Destroy();//インスタンスの破棄
//
//public:
//	//ステージデータの取得
//	const std::vector<std::vector<ePanelID>>& GetAll();
//	//位置情報から確進行方向のパネル情報を取得
//	const std::unordered_map<eAdjacentDirection, ePanelID> GetAdjacentPanelData(const Vector2& location) const;
//	//位置情報からパネル情報を取得
//	const ePanelID GetPanelData(const Vector2& location) const;
//	//パネル情報の登録
//	void SetPanelData(ePanelID id, const Vector2& location, int mx, int my);
//	//位置情報からマス目情報に変換
//	void ConvertToIndex(const Vector2& location, int& x, int& y) const;
//
//private:
//	void Load();//読み込み
//	void Unload();//解放
//
//};

