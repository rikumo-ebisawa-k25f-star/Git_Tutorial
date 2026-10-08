#pragma once
#include "../Math/Vector2.h"
/// <summary>
/// アクターの状態
/// </summary>
enum class eActorState : unsigned char
{
	eActive = 0,
	ePaused,
	eDead,
};
/// <summary>
/// アクターのタグ
/// </summary>
enum class eActorTag : unsigned char
{
	eNone,
	ePlayer,
	eEnemy,
	eWall,
	eItem,
};
/// <summary>
/// 画面上に出てくるオブジェクトの基底クラス
/// </summary>
class Actor
{
protected:
	Vector2 location;//位置情報
	float radius;//半径情報
	int image;//画像情報
	double scale;//大きさ
	unsigned char z_layer;//Zレイヤー
	bool is_destroy;//死んだ状態
	eActorState actor_state;//アクターの状態

public:
	Actor();
	virtual ~Actor();

public:
	//初期化処理
	virtual void Initialize();
	//更新処理
	virtual void Update(float delta_second);
	//描画処理
	virtual void Draw() const;
	//終了時処理
	virtual void Finalize();

public:
	//当たり判定通知処理
	virtual void OnHitCollisionEnter(Actor* actor);
	//タグ取得
	virtual eActorTag GetActorTag() const = 0;

public:
	//位置情報の取得
	const Vector2& GetLocation() const;
	//半径情報取得
	float GetRadius() const;
	//アクターの状態取得
	eActorState GetState() const;
	//レイヤーの情報の取得
	unsigned char GetZLayer() const;
	//位置情報の設定
	void SetLocation(const Vector2& location);

protected:
	//アクターの破棄
	void DestroyActor(Actor* target = nullptr);

};
