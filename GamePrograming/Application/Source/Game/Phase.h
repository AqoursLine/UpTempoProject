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

enum PHASESTATE {
	PHASESTATE_START = 0,
	PHASESTATE_FINISH,
	PHASESTATE_RUN,
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

	//ステート関数
	virtual void Start();
	virtual void Finish();
	virtual void Run(); 

private:
	bool m_isFinished = false;

	int m_stateCount = 0;
	int m_targetCount = 30;

	//背景用
	XMFLOAT2 m_bgPos = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	XMFLOAT2 m_bgSize = XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT);
};

