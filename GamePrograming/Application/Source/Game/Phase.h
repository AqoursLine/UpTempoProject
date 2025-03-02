/******************************************************
* Phaseh		フェーズ管理
* 制作者：ミヤタジョウジ
* 作成日：2024/10/22
* 最終更新日：2024/10/22
*******************************************************/
#pragma once

#include "Physics.h"
#include "Game/GameObject.h"
#include "Game/FieldManager.h"
#include "Game/ThrowObjectManager.h"
#include "Game/PlayerManager.h"
#include "Game/Transition.h"
#include "DirectX/Audio.h"
#include "DirectX/video_texture_AVI.h"

enum PHASESTATE {
	PHASESTATE_OUTTRANSITION = 0,

	PHASESTATE_COUNTDOWN,

	PHASESTATE_START,
	

	PHASESTATE_RUN,

	PHASESTATE_FINISH,
	PHASESTATE_INTRANSITION,
};

/****************************************************
* フェーズクラス
*****************************************************/
class Phase {
public:
	Phase() = delete;
	Phase(const int phaseNum, const float gravityX, const float gravityY);
	~Phase();

	virtual void Update();
	virtual void Draw();

	static void ChangeState(PHASESTATE state);

	const bool GetIsFinished() const { return m_isFinished; }
protected:
	//世界のルール
	Physics* m_physics = nullptr;

	//フェーズのステート
	static PHASESTATE m_state;

	//背景
	Texture m_texture;

	//マネージャー群
	PlayerManager* m_playerManager;
	FieldManager* m_fieldManager = nullptr;
	ThrowObjectManager* m_throwObjectManager = nullptr;
	StageObjectManager* m_stageObjectManager = nullptr;


	//ステート関数
	virtual void OutTransition();
	virtual void Start();
	
	virtual void Run();

	virtual void InTransition();
	virtual void Finish();

private:
	bool m_isFinished = false;

	int m_stateCount = 0;
	int m_targetCount = 30;

	//SE用変数

	int m_startSound;
	static int m_finishSound;
	

	//背景用
	XMFLOAT2 m_bgPos = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	XMFLOAT2 m_bgSize = XMFLOAT2(SCREEN_WIDTH + 40.0f, SCREEN_HEIGHT + 40.0f);

	Transition m_FinishTransition;
	Transition m_OUT_transition;
	Transition m_IN_transition;

	VideoTextureAVI m_StartAnim;

};



