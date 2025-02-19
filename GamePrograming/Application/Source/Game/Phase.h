/******************************************************
* Phaseh		?t?F?[?Y?Ǘ?
* ?????F?~???^?W???E?W
* ?쐬???F2024/10/22
* ?ŏI?X?V???F2024/10/22
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
* ?t?F?[?Y?N???X
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
	//???E?̃??[??
	Physics* m_physics = nullptr;

	//?t?F?[?Y?̃X?e?[?g
	static PHASESTATE m_state;

	//?w?i
	Texture m_texture;

	//?}?l?[?W???[?Q
	PlayerManager* m_playerManager;
	FieldManager* m_fieldManager = nullptr;
	ThrowObjectManager* m_throwObjectManager = nullptr;

	//?X?e?[?g?֐?
	virtual void Start();
	virtual void Finish();
	virtual void Run(); 

private:
	bool m_isFinished = false;

	int m_stateCount = 0;
	int m_targetCount = 30;

	//?w?i?p
	XMFLOAT2 m_bgPos = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	XMFLOAT2 m_bgSize = XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT);
};
