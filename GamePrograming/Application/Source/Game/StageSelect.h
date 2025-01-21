/******************************************************
* StageSelect.h		ステージ選択
* 制作者：ミヤタジョウジ
* 作成日：2025/1/16
* 最終更新日：2025/1/21
*******************************************************/
#pragma once
#include "Game/Select.h"
#include "Game/SaveData.h"

class StageSelect : public Select {
public:
	StageSelect();
	~StageSelect();

	void Update() override;
	void Draw() override;
	

private:
	STAGE m_stageNumber;          // ステージ番号
	XMFLOAT2 m_cursorPos;    // カーソル位置
	float m_cursorSpeed;     // カーソルの移動速度
	int m_padIndex;          // 使用するコントローラーのインデックス
};

