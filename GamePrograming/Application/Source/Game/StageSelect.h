/******************************************************
* StageSelect.h		ステージ選択
* 制作者：ミヤタジョウジ
* 作成日：2025/1/16
* 最終更新日：2025/2/10
*******************************************************/
#pragma once
#include "DirectX/video_texture.h"
#include "Game/Select.h"
#include "Game/SaveData.h"

enum class StageSelectState {
	SELECTION,        //　ステージ選択中
	ANIMATION,        //　ステージ決定アニメーション
	INTRO_ANIMATION   //　箱＆映像アニメーション
};

class StageSelect : public Select {
public:

	StageSelect();
	~StageSelect();
	void Update() override;
	void Draw() override;


private:

	void Select();				//　ステージ選択
	void DetermineFinalStage();	//　ステージ決定処理
	void FinalStageAnim();		//　ステージ遷移アニメーション

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//　ステート管理
	StageSelectState m_state;	//　ステージの状態
	

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//　ステージ関連
	STAGE m_stageNumber;		//　ステージ番号
	std::vector<int> m_selectedStages;// 各プレイヤーの選択したステージ


	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//　コントローラー関連
	int	  m_totalPlayer;		//　総プレイヤー数
	int	  m_padIndex[4];		//　コントローラー配列


	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//　背景関連
	XMFLOAT2 m_backGroundPos;	//　背景の座標
	XMFLOAT2 m_backGroundSize;	//　背景サイズ

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//　カーソル関連
	XMFLOAT2 m_cursorPos[4];		//　カーソル座標配列
	XMFLOAT2 m_cursorSize[4];		//　カーソルサイズ配列
	float	 m_cursorSpeed[4];		//　カーソル移動速度配列
	std::vector<bool>m_cursorLocked;//　カーソルがロックされてるか


	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//　変化前のボタン関連
	XMFLOAT2 m_buttonPos[4];		//　変化前ボタンの座標配列
	XMFLOAT2 m_buttonSize[4];		//　変化前ボタンのサイズ配列
	bool	 m_buttonSelected[4][4];//　各ボタンが選択されているかどうか
	bool	 allSelected;			//　全プレイヤーが選択したか


	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//　変化後のボタン関連
	XMFLOAT2 m_ChangebuttonSize[4];	//　変化後ボタンのサイズ配列

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//　アニメーション関連
	XMFLOAT2 m_animObjectPos;		//　オブジェクトの座標
	int		 m_animObjectIndex;		//　オブジェクトの配列用番号
	float	 m_animObjectTimer;		//　オブジェクトの動く時間
	float	 m_animObjectInterval;	//　オブジェクトの動く間隔
	float	 m_animTotalTime;		//　オブジェクトの動いた合計時間
	bool	 m_animRouletteFinished;//　ルーレットが終わったか？

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//　動画関連
	VideoTexture m_video;		//　背景動画
	VideoTexture m_stageVideo1;	//　ステージ１プレイ動画
	VideoTexture m_stageVideo2;	//　ステージ２プレイ動画
	VideoTexture m_stageVideo3;	//　ステージ３プレイ動画
	VideoTexture m_stageVideo4;	//　ステージ４プレイ動画
	VideoTexture m_animVideo;	//　キラキラ
	VideoTexture m_animVideo2;	//　箱アニメーション
	XMFLOAT2	 m_moviePos[4];	//　動画の座標配列
	XMFLOAT2	 m_movieSize[4];//　動画のサイズ配列

};

