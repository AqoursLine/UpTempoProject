/******************************************************
* Phaseh		?t?F?[?Y?Ǘ?
* ?????F?~???^?W???E?W
* ?쐬???F2024/10/22
* ?ŏI?X?V???F2024/10/22
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Phase.h"
#include "Game/Player.h"

/****************************************************
* ?X?^?e?B?b?N?ϐ???????
*****************************************************/
PHASESTATE Phase::m_state = PHASESTATE_RUN;

/****************************************************
* ?t?F?[?Y??????
*****************************************************/
Phase::Phase(const int phaseNum, const float gravityX, const float gravityY) : m_physics(new Physics(gravityX, gravityY)) {
	m_state = PHASESTATE_START;

	m_fieldManager = new FieldManager();
	m_throwObjectManager = new ThrowObjectManager();
	m_playerManager = new PlayerManager(phaseNum);
}

/****************************************************
* ?t?F?[?Y?X?V
*****************************************************/
void Phase::Update() {
	switch (m_state) {
		case PHASESTATE_START:
			Start();
			break;
		case PHASESTATE_FINISH:
			Finish();
			break;
		case PHASESTATE_RUN:
			Run();
			break;
		default:
			break;
	}
}

/****************************************************
* ?t?F?[?Y?`??
*****************************************************/
void Phase::Draw() {
	D3D.Draw2D(m_texture, m_bgPos, m_bgSize);

	m_fieldManager->Draw();
	m_throwObjectManager->Draw();
	m_playerManager->Draw();

	//?X?^?[?g???o?`??
	if (m_state == PHASESTATE_START) {
	}

	//?I?????o?`??
	if (m_state == PHASESTATE_FINISH) {
	}
}

/****************************************************
* ?t?F?[?Y?I??
*****************************************************/
Phase::~Phase() {
	if (m_fieldManager) delete m_fieldManager;
	if (m_throwObjectManager) delete m_throwObjectManager;
	if (m_playerManager) delete m_playerManager;
	if (m_physics) delete m_physics;
}

/****************************************************
* ?t?F?[?Y?N???????ׂ????ɂȂ???
*****************************************************/
void Phase::Start() {
	//?t?F?[?Y?N??????

	//?t?F?[?Y?N???????I??
	if (true) {
		m_state = PHASESTATE_RUN;
	}
}

/****************************************************
* ?t?F?[?Y?I???܂?
*****************************************************/
void Phase::Finish() {
	m_stateCount++;
	if (m_stateCount >= m_targetCount) {
		m_isFinished = true;
	}
}

/****************************************************
* ?t?F?[?Y???s
*****************************************************/
void Phase::Run() {
	m_physics->UpdatePhysics((1.0f / 60.0f), 8, 3);
	m_fieldManager->Update();
	m_throwObjectManager->Update();
	m_playerManager->Update();
}

/****************************************************
* ?t?F?[?Y?J??
*****************************************************/
void Phase::ChangeState(PHASESTATE state) {
	m_state = state;
}
