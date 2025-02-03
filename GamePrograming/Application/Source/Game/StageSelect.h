/******************************************************
* StageSelect.h		ステージ選択
* 制作者：ミヤタジョウジ
* 作成日：2025/1/16
* 最終更新日：2025/1/26
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
	
	// ステージ決定処理
	void DetermineFinalStage();

private:

	// ステージ番号
	STAGE m_stageNumber; 
	
	//総プレイヤー数
	int	  m_totalPlayer;

	
	// カーソル情報
	XMFLOAT2 m_cursorPos[4];// 各コントローラーのカーソル位置
	
	float m_cursorSpeed[4];	// 各コントローラーのカーソル移動速度
	
	//ボタン変数
	XMFLOAT2 m_buttonPos[4];// ボタンの位置
	XMFLOAT2 m_buttonSize[4];		// ボタンのサイズ
	bool m_buttonSelected[4][4]; // 各ボタンが選択されているかどうかのフラグ

	XMFLOAT2 m_backGroundPos;
	// コントローラー情報
	int m_padIndex[4];		// 各コントローラーのハンドル

	// 各プレイヤーの選択したステージ
	std::vector<int> m_selectedStage;

	// 全員の選択が完了したか
	bool m_isSelectionComplete = false;

	std::vector<bool> m_cursorLocked; // 各プレイヤーのカーソルがロックされているか
	
};

