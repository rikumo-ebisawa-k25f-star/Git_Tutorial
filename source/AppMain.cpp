#include <DxLib.h>
#include <algorithm>
#include "File/StageData.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	ChangeWindowMode(TRUE);
	SetGraphMode(640, 480, 32);
	if (DxLib_Init() == -1) return -1;
	SetDrawScreen(DX_SCREEN_BACK);

	// 1. マップデータの読み込み
	StageData::Get()->Load();

	// 2. 床画像の読み込み（画像ファイル名を入れる）
	// ※ 画像がまだない場合はコメントアウトしてください
	int blockGraph = LoadGraph("block.png");

	float cameraX = 0.0f;
	float cameraY = 0.0f;

	while (ProcessMessage() != -1)
	{
		
		ClearDrawScreen();

		// 3. 描画関数を呼び出す
	

		ScreenFlip();
	}


	DxLib_End();
	return 0;
}
