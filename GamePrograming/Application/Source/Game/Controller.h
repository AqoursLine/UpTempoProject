/******************************************************
* Controller.h		�R���g���[���[�ݒ�
* ����ҁF�~���^�W���E�W
* �쐬���F2024/10/17
* �ŏI�X�V���F2024/10/22
*******************************************************/
#pragma once
#include "DirectX/Input.h"

/****************************************************
* �񋓑̐錾
*****************************************************/
//�Q�[���p�b�h�\���L�[
enum GAMEPAD_CROSS {
	GAMEPAD_CROSS_UP = 0,
	GAMEPAD_CROSS_RIGHT,
	GAMEPAD_CROSS_DOWN,
	GAMEPAD_CROSS_LEFT,
};
//�Q�[���p�b�h�{�^��
enum GAMEPAD_BUTTON {
	GAMEPAD_BUTTON_PS4_SQUARE = 0,
	GAMEPAD_BUTTON_PS4_CROSS,
	GAMEPAD_BUTTON_PS4_CIRCLE,
	GAMEPAD_BUTTON_PS4_TRIANGLE,
	GAMEPAD_BUTTON_PS4_L1,
	GAMEPAD_BUTTON_PS4_R1,
	GAMEPAD_BUTTON_PS4_L2,
	GAMEPAD_BUTTON_PS4_R2,
	GAMEPAD_BUTTON_PS4_SHARE,
	GAMEPAD_BUTTON_PS4_OPTIONS,
	GAMEPAD_BUTTON_PS4_L3,
	GAMEPAD_BUTTON_PS4_R3,
	GAMEPAD_BUTTON_PS4_PS,
	GAMEPAD_BUTTON_PS4_TOUCH,
};


/****************************************************
* �R���g���[���[�N���X
*****************************************************/
class Controller {
public:
	//�R���g���[���[��������
	bool Initialize(HINSTANCE hInstance, HWND hWnd);
	//�R���g���[���[���X�V
	void UpdateController();
	//�R���g���[���[���I��
	void Finalize();

	//�L�[�{�[�h�������܂�
	const bool GetKeyboardPress(DWORD key);
	//�L�[�{�[�h�������u��
	const bool GetKeyboardTrigger(DWORD key);
	//�L�[�{�[�h�������u��
	const bool GetKeyboardRelease(DWORD key);

	//�}�E�X�{�^���������܂�
	const bool GetMousePress(MOUSEKEY key);
	//�}�E�X�{�^���������u��
	const bool GetMouseTrigger(MOUSEKEY key);
	//�}�E�X�{�^���������u��
	const bool GetMouseRelease(MOUSEKEY key);
	//�}�E�X�����ړ���
	const LONG GetMouseDeltaHorizontal();
	//�}�E�X�����ړ���
	const LONG GetMouseDeltaVertical();
	//�}�E�X�ړ����x�ݒ�
	const void SetMouseSensitivity(float sensitivity);

	//�Q�[���p�b�h�\���L�[�������܂�
	const bool GetGamepadCrossPress(GAMEPAD_CROSS key, int padIndex);
	//�Q�[���p�b�h�\���L�[�������u��
	const bool GetGamepadCrossTrigger(GAMEPAD_CROSS key, int padIndex);
	//�Q�[���p�b�h�\���L�[�������u��
	const bool GetGamepadCrossRelease(GAMEPAD_CROSS key, int padIndex);
	//�Q�[���p�b�h���X�e�B�b�N�����p�x
	const LONG GetLeftStickHorizontal(int padIndex);
	//�Q�[���p�b�h���X�e�B�b�N�����p�x
	const LONG GetLeftStickVertical(int padIndex);
	//�Q�[���p�b�h�E�X�e�B�b�N�����p�x
	const LONG GetRightStickHorizontal(int padIndex);
	//�Q�[���p�b�h�E�X�e�B�b�N�����p�x
	const LONG GetRightStickVertical(int padIndex);
	//�Q�[���p�b�h�{�^���������܂�
	const bool GetGamepadButtonPress(GAMEPAD_BUTTON key, int padIndex);
	//�Q�[���p�b�h�{�^���������u��
	const bool GetGamepadButtonTrigger(GAMEPAD_BUTTON key, int padIndex);
	//�Q�[���p�b�h�{�^���������u��
	const bool GetGamepadButtonRelease(GAMEPAD_BUTTON key, int padIndex);

	//�Q�[���p�b�h�X�e�B�b�N���x�ݒ�
	void SetStickSensitivity(float sensitivity, int padIndex);
	//�Q�[���p�b�h�n���h���擾
	const int GetGamepadHandle();
	//�Q�[���p�b�h�n���h�����
	void ReleaseGamepadHandle(int i);

private:
	//DirectInput
	Input m_dInput;

	//�}�E�X���x
	float m_mouseSensitivity = 0;
	
	//�Q�[���p�b�h����
	int m_gamepadMax = 0;
	//�Q�[���p�b�h�g�p�ς�
	bool* m_isUsed = nullptr;

	//�\���L�[�v�Z
	bool CalCross(LONG angle, GAMEPAD_CROSS key);

	//���̃N���X���A�ǂ�����ł��A�N�Z�X�ł���悤�ɃV���O���g���p�^�[����
	//�B��̃C���X�^���X�p�̃|�C���^
	static inline Controller* s_instance;
	//�R���X�g���N�^��private�ɂ���
	Controller() {}
public:
	//�C���X�^���X����
	static void CreateInstance() {
		DeleteInstance();

		s_instance = new Controller();
	}
	//�C���X�^���X�폜
	static void DeleteInstance() {
		if (s_instance != nullptr) {
			delete s_instance;
			s_instance = nullptr;
		}
	}
	//�B��̃C���X�^���X���擾
	static Controller& GetInstance() {
		return *s_instance;
	}
};

#define CTRL Controller::GetInstance()
