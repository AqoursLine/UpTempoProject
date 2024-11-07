/******************************************************
* GameSystem.cpp		�Q�[���V�X�e�����Ǘ�
* ����ҁF�~���^�W���E�W
* �쐬���F2024/10/5
* �ŏI�X�V���F2024/10/22
*******************************************************/
#include "framework.h"
//DirectX�N���X���g����悤�ɂ���
#include "DirectX/DirectX.h"
#include <box2d/box2d.h>
//GameSystem�N���X���g����悤�ɂ���
#include "Game/GameSystem.h"
//�R���g���[���[���g����悤�ɂ���
#include "Game/Controller.h"
//�Q�[���V�[��
#include "Game/GameScene.h"

/******************************************************
* �Q�[���̏�����
*******************************************************/
void GameSystem::Initialize() {
	ChangeScene(SCENE_GAME);

	//���Ԍv���J�n
	m_oldTime = timeGetTime();
}

/******************************************************
* �Q�[���̍X�V
*******************************************************/
void GameSystem::Excute() {
	//���ԍX�V
	DWORD currentTime = timeGetTime();

	m_deltaTime = currentTime - m_oldTime;
	m_elapsedTime += m_deltaTime;

	m_oldTime = currentTime;

	//�R���g���[���[���X�V
	CTRL.UpdateController();

	//�X�V
	m_scene->Update();

	//�`��
	m_scene->Draw();
}

/******************************************************
* �Q�[���̏I��
*******************************************************/
void GameSystem::Finalize() {
//	m_scene->FinalizeGameObject();

	if (m_scene) {
		delete m_scene;
	}


	CTRL.Finalize();
}

/******************************************************
* �V�[���J��
*******************************************************/
void GameSystem::ChangeScene(SCENES scene) {
	if (m_scene) {
		delete m_scene;
	}

	switch (scene) {
		case SCENE_TITLE:
			break;
		case SCENE_GAME:
			m_scene = new GameScene();
			break;
		case SCENE_RESULT:
			break;
		default:
			break;
	}
}

/******************************************************
* �o�ߎ��Ԏ擾
*******************************************************/
const float GameSystem::GetElapsedTime() const {
	return (m_elapsedTime / 1000.0f);
}

/******************************************************
* �P�t���[�����Ԏ擾
*******************************************************/
const float GameSystem::GetDletaTime() const {
	return (m_deltaTime / 1000.0f);
}
