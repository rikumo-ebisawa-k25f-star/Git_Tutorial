#pragma once
#pragma once

#include "../Math/Vector2.h"
#include <unordered_map>
#include <vector>
/// <summary>
/// パネルのID
/// </summary>
enum class ePanelID : unsigned char
{
	eNone,//なんもない
	eBlock,//壁
	eBranch,//分岐点
	eGate,//ゲート
};

/// <summary>
/// 進行方向の情報
/// </summary>
enum class eAdjacentDirection : unsigned char
{
	eUp,
	eDown,
	eLeft,
	eRight,
};

/// <summary>
/// ステージデータ
/// </summary>
class StageData final
{
public:
	static const float CHIP_SIZE;//1ブロック分の大きさ
	static const float STAGE_OFFSET_X;//ステーz委のずれている量（横）
	static const float STAGE_OFFSET_Y;//ステージのずれている量（縦）
	static const Vector2 STAGE_OFFSET;//ステージのずれている量

private:
	//自クラスポインタ
	static StageData* instance;
	//ステージデータ情報
	std::vector<std::vector<ePanelID>> data;

private:
	StageData() = default;
	StageData(const StageData&) = delete;
	StageData& operator = (StageData&) = delete;
	~StageData() = default;

public:
	static StageData* Get();//インスタンスの取得
	static void Destroy();//インスタンスの破棄

public:
	//ステージデータの取得
	const std::vector<std::vector<ePanelID>>& GetAll();
	//位置情報から確進行方向のパネル情報を取得
	const std::unordered_map<eAdjacentDirection, ePanelID> GetAdjacentPanelData(const Vector2& location) const;
	//位置情報からパネル情報を取得
	const ePanelID GetPanelData(const Vector2& location) const;
	//パネル情報の登録
	void SetPanelData(ePanelID id, const Vector2& location, int mx, int my);
	//位置情報からマス目情報に変換
	void ConvertToIndex(const Vector2& location, int& x, int& y) const;

private:
	void Load();//読み込み
	void Unload();//解放

};

