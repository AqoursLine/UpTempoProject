/******************************************************
* StageSelect.h		ステージ選択
* 制作者：ミヤタジョウジ
* 作成日：2025/1/16
* 最終更新日：2025/1/26
*******************************************************/
#pragma once
#include "DirectX/video_texture.h"
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

	// ステージ遷移アニメーション
	void FinalStageAnim();

private:
	//動画読み込む機構
	VideoTexture m_video;	//背景動画
	VideoTexture m_video2;	//ステージ１
	VideoTexture m_video3;	//ステージ２
	VideoTexture m_video4;	//ステージ３
	VideoTexture m_video5;	//ステージ４

	VideoTexture m_animvideo;	//キラキラ
	VideoTexture m_animvideo2;	//箱アニメーション
	
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

	//変わったボタン
	XMFLOAT2 m_ChangebuttonSize[4];


	XMFLOAT2 m_backGroundPos;
	// コントローラー情報
	int m_padIndex[4];		// 各コントローラーのハンドル

	// 各プレイヤーの選択したステージ
	std::vector<int> m_selectedStage;
	std::vector<bool> m_cursorLocked; // 各プレイヤーのカーソルがロックされているか
	
	//ステージの映像の位置とサイズ
	XMFLOAT2 m_moviePos[4];
	XMFLOAT2 m_movieSize[4];

	XMFLOAT2 m_animObjectPos;

	XMFLOAT2 m_boxPos;
	XMFLOAT2 m_boxSize;

	bool m_animStart;

};

