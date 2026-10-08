#include"IngameScene.h"
void RenderStage(float cameraX, float cameraY, int screenWidth, int screenHeight)
{
	// 1. StageDataのインスタンスと全体データを取得
	StageData* stage = StageData::Get();
	const auto& mapData = stage->GetAll();

	if (mapData.empty()) return;

	// 2. 画面内に見えるマス目（インデックス）の範囲を計算（ビューポート Culling）
	// カメラ位置から、画面左端と右端の x インデックスを算出
	int startX = static_cast<int>(cameraX / StageData::CHIP_SIZE);
	int endX = static_cast<int>((cameraX + screenWidth) / StageData::CHIP_SIZE) + 1;

	// 画面上端と下端の y インデックスを算出
	int startY = static_cast<int>(cameraY / StageData::CHIP_SIZE);
	int endY = static_cast<int>((cameraY + screenHeight) / StageData::CHIP_SIZE) + 1;

	// 配列の範囲外アクセスを防ぐ安全ガード
	int mapHeight = static_cast<int>(mapData.size());
	startY = std::max(0, std::min(startY, mapHeight));
	endY = std::max(0, std::min(endY, mapHeight));

	// 3. 画面内にあるマス目だけを二重ループで描画
	for (int y = startY; y < endY; ++y)
	{
		int mapWidth = static_cast<int>(mapData[y].size());
		int currentStartX = std::max(0, std::min(startX, mapWidth));
		int currentEndX = std::max(0, std::min(endX, mapWidth));

		for (int x = currentStartX; x < currentEndX; ++x)
		{
			ePanelID panel = mapData[y][x];

			// 空白（eNone）なら描画スキップ
			if (panel == ePanelID::eNone) continue;

			// ワールド座標（マップ上の絶対座標）を計算
			float worldX = x * StageData::CHIP_SIZE + StageData::STAGE_OFFSET_X;
			float worldY = y * StageData::CHIP_SIZE + StageData::STAGE_OFFSET_Y;

			// スクリーン座標（画面上の表示位置）に変換（カメラ座標を引き算）
			float screenX = worldX - cameraX;
			float screenY = worldY - cameraY;

			// 4. パネルの種類に応じて描画（ライブラリに応じた描画関数を呼び出す）
			switch (panel)
			{
			case ePanelID::eBlock: // 床・ブロックの場合
				// 例: DxLibの場合
				// DrawGraph(screenX, screenY, blockHandle, TRUE);

				// 例: 当たり判定確認用の簡易矩形描画（茶色の床）
				// DrawBox(screenX, screenY, screenX + CHIP_SIZE, screenY + CHIP_SIZE, BROWN);
				break;

			default:
				break;
			}
		}
	}
}