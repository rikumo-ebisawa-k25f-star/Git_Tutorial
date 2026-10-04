#include <DxLib.h>
#include"Input/InputManager.h"

float GetDeltaSecond();

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	ChangeWindowMode(TRUE);					// ウィンドウモードで起動
	SetGraphMode(640, 480, 32);				// 画面サイズと色深度を設定
	// DXライブラリの初期化
	if (DxLib_Init() == -1)
	{
		return -1;						// 初期化に失敗したら異常終了
	}
	SetDrawScreen(DX_SCREEN_BACK);		// 裏画面への描画を有効化(ダブルバッファ)


	float time = 0.0f;
	float time_speed = 60.0f;

	//メインループ
	while (ProcessMessage() != -1)
	{
		time += GetDeltaSecond();
		if (time >= (1.0f / time_speed))
		{
			time = 0.0;
			//入力の更新処理
			
			//InputUpdate();
			//シーンの更新処理

			ClearDrawScreen();

			ScreenFlip();
		}
	}
	DxLib_End();
	return 0;
}

/// <summary>
/// 1フレームにかかった時間を計測する
/// </summary>
/// <returns></returns>1フレームにかかった時間
float GetDeltaSecond()
{
	//PCが起動されてからの時間を計測する(戻り値はマイクロ秒
	static LONGLONG old_time = GetNowHiPerformanceCount();	// 　前回の取得時間

	LONGLONG current_time = GetNowHiPerformanceCount();	//現在の取得時間

	// 現在時間と前回時間の差分を取得する
	// マイクロ秒→秒に単位を変換する
	float result = (float)(current_time - old_time) * 1.0e-6f;
	old_time = current_time;

	// 計測結果を戻す
	return result;
}