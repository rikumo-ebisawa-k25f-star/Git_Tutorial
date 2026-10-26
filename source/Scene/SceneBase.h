#pragma once
#include "SceneType.h"
/// <summary>
/// 各画面の基底のクラス
/// </summary>
class SceneBase
{
private:

public:
	SceneBase() = default;
	virtual ~SceneBase() = default;

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize() {}
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta_second">いフレームにかかった時間</param>
	/// <returns>次に遷移したいシーンの情報</returns>
	virtual eSceneType Update(float delta_second) = 0;
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() const {}
	/// <summary>
	/// 終了時処理
	/// </summary>
	virtual void Finalize() {}

public:
	/// <summary>
	/// 現在のシーン情報を渡す
	/// </summary>
	/// <returns>現在のシーン情報</returns>
	virtual eSceneType GetNowSceneType() const = 0;

};
