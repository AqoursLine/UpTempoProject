/******************************************************
* GameSystem.h		�Q�[���V�X�e�����Ǘ�
* ����ҁF�~���^�W���E�W
* �쐬���F2024/10/5
* �ŏI�X�V���F2024/10/22
*******************************************************/
#pragma once
#include "Game/Scene.h"

/******************************************************
* �񋓑̐錾
*******************************************************/
//�V�[��
enum SCENES {
	SCENE_TITLE = 0,
	SCENE_GAME,
	SCENE_RESULT,
};

/******************************************************
* GameSystem�N���X
* �����̃Q�[���̓y��ƂȂ����
*******************************************************/
class GameSystem {
public:
	//�Q�[���̏�����
	void Initialize();

	//�Q�[���̍X�V
	void Excute();

	//�Q�[���̏I��
	void Finalize();

	//�V�[���J��
	void ChangeScene(SCENES scene);

	//�o�ߎ��Ԏ擾
	const float GetElapsedTime() const;
	//�P�t���[�����Ԏ擾
	const float GetDletaTime() const;

private:
	//�V�[��
	Scene* m_scene = nullptr;
	//���Ԍv��
	DWORD m_oldTime = 0;
	DWORD m_elapsedTime = 0;
	DWORD m_deltaTime = 0;

	//���̃N���X���A�ǂ�����ł��A�N�Z�X�ł���悤�ɃV���O���g���p�^�[����
	//�B��̃C���X�^���X�p�̃|�C���^
	static inline GameSystem* s_instance;
	//�R���X�g���N�^��private�ɂ���
	GameSystem() { m_scene = nullptr; }
public:
	//�C���X�^���X����
	static void CreateInstance() {
		DeleteInstance();

		s_instance = new GameSystem();
	}
	//�C���X�^���X�폜
	static void DeleteInstance() {
		if (s_instance != nullptr) {
			delete s_instance;
			s_instance = nullptr;
		}
	}
	//�B��̃C���X�^���X���擾
	static GameSystem& GetInstance() {
		return *s_instance;
	}
};

//GameSystem�̗B��̃C���X�^���X���ȒP�Ɏ擾���邽�߂̃}�N��
#define GAMESYS GameSystem::GetInstance()
