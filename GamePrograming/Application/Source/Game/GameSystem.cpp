/******************************************************
* GameSystem.cpp		?Q?[???V?X?e????Ǘ?
* ?????F?~???^?W???E?W
* ?쐬???F2024/10/5
* ?ŏI?X?V???F2024/10/22
*******************************************************/
#include "framework.h"
//DirectX?N???X??g???????ɂ???
#include "DirectX/DirectX.h"
#include <box2d/box2d.h>
//GameSystem?N???X??g???????ɂ???
#include "GameSystem.h"
//?R???g???[???[??g???????ɂ???
#include "Controller.h"
//?Q?[???V?[??
#include "GameScene.h"
//?^?C?g???V?[??
#include "Game/TitleScene.h"
//???U???g?V?[??
#include "Game/ResultScene.h"
//?I??V?[??
#include "Game/ChooseScene.h"

//?Z?[?u?f?[?^
#include "Game/SaveData.h"

/******************************************************
* ?Q?[???̏?????
*******************************************************/
void GameSystem::Initialize() {
	//?Z?[?u?f?[?^??Ƃ肠?????ݒ?
	int totalPlayer = 4;
	SaveData::SetTotalPlayer(totalPlayer);
	SaveData::SetStage(STAGE_OCEAN);
	for (int i = 0; i < totalPlayer; i++) {
		PlayerData pData;
		pData.charactorNum = (CHARACTOR)(CHARACTOR_01 + i);
		pData.PadNum = i;
		SaveData::SetPlayerData(pData);
		SaveData::SetPlayerRank(i + 1);
	}
	

	//?V?[????쐬
	m_sceneNum = SCENE_RESULT;
	ChangeScene(m_sceneNum);

	//?}?g???N?X??????
	D3D.SetWorldViewProjection2D();

	//???Ԍv???J?n
	m_oldTime = timeGetTime();
}

/******************************************************
* ?Q?[???̍X?V
*******************************************************/
void GameSystem::Excute() {
	//???ԍX?V
	DWORD currentTime = timeGetTime();

	m_deltaTime = currentTime - m_oldTime;
	m_elapsedTime += m_deltaTime;

	m_oldTime = currentTime;

	//?R???g???[???[??X?V
	CTRL.UpdateController();

	//?X?V
	m_scene->Update();

	//?`??
	m_scene->Draw();

	if (m_scene->GetIsEnd()) {
		m_isEnd = true;
		return;
	}

	if (m_scene->GetIsFinished()) {
		m_sceneNum = static_cast<SCENES>((m_sceneNum + 1) % SCENE_MAX);
		ChangeScene(m_sceneNum);
	}
}

/******************************************************
* ?Q?[???̏I??
*******************************************************/
void GameSystem::Finalize() {
	if (m_scene) {
		delete m_scene;
	}

	CTRL.Finalize();
}

/******************************************************
* ?V?[???J??
*******************************************************/
void GameSystem::ChangeScene(SCENES scene) {
	if (m_scene) {
		delete m_scene;
	}

	switch (scene) {
	case SCENE_TITLE:
		m_scene = new TitleScene();
		break;
	case SCENE_CHOOSE:
		m_scene = new ChooseScene();
		break;
	case SCENE_GAME:
		m_scene = new GameScene();
		break;
	case SCENE_RESULT:
		m_scene = new ResultScene();
		break;
	default:
		break;
	}
}

/******************************************************
* ?o?ߎ??Ԏ擾
*******************************************************/
const float GameSystem::GetElapsedTime() const {
	return (m_elapsedTime / 1000.0f);
}

/******************************************************
* ?P?t???[?????Ԏ擾
*******************************************************/
const float GameSystem::GetDletaTime() const {
	return (m_deltaTime / 1000.0f);
}
