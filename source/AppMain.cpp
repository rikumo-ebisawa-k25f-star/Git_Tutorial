#include <DxLib.h>
#include <algorithm>
#include "File/StageData.h"

// 描画関数のプロトタイプ宣言
void RenderStage(float cameraX, float cameraY, int blockGraph);

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
		// テスト用：右キーでカメラ移動
		if (CheckHitKey(KEY_INPUT_RIGHT)) cameraX += 4.0f;
		if (CheckHitKey(KEY_INPUT_LEFT))  cameraX = std::max(0.0f, cameraX - 4.0f);

		ClearDrawScreen();

		// 3. 描画関数を呼び出す
		RenderStage(cameraX, cameraY, blockGraph);

		ScreenFlip();
	}

	StageData::Destroy();
	DxLib_End();
	return 0;
}

void RenderStage(float cameraX, float cameraY, int blockGraph)
{
	StageData* stage = StageData::Get();
	const auto& mapData = stage->GetAll();
	if (mapData.empty()) return;

	// 画面に見える範囲（インデックス）を計算
	int startX = static_cast<int>(cameraX / StageData::CHIP_SIZE);
	int endX = static_cast<int>((cameraX + 640.0f) / StageData::CHIP_SIZE) + 1;
	int startY = static_cast<int>(cameraY / StageData::CHIP_SIZE);
	int endY = static_cast<int>((cameraY + 480.0f) / StageData::CHIP_SIZE) + 1;

	int mapHeight = static_cast<int>(mapData.size());
	startY = std::max(0, std::min(startY, mapHeight));
	endY = std::max(0, std::min(endY, mapHeight));

	for (int y = startY; y < endY; ++y)
	{
		int mapWidth = static_cast<int>(mapData[y].size());
		int currentStartX = std::max(0, std::min(startX, mapWidth));
		int currentEndX = std::max(0, std::min(endX, mapWidth));

		for (int x = currentStartX; x < currentEndX; ++x)
		{
			if (mapData[y][x] == ePanelID::eNone) continue;

			// 画面上の表示位置を計算（ワールド座標 - カメラ座標）
			int screenX = static_cast<int>(x * StageData::CHIP_SIZE - cameraX);
			int screenY = static_cast<int>(y * StageData::CHIP_SIZE - cameraY);
			int chipSize = static_cast<int>(StageData::CHIP_SIZE);

			// --- 画像描画 ---
			if (blockGraph != -1)
			{
				DrawGraph(screenX, screenY, blockGraph, TRUE);
			}
			else
			{
				// 画像がない場合の仮描画（茶色の四角）
				DrawBox(screenX, screenY, screenX + chipSize, screenY + chipSize, GetColor(165, 42, 42), TRUE);
			}
		}
	}
}