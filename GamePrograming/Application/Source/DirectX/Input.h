/******************************************************
* input.h		DirectInput�ݒ�
* ����ҁF�~���^�W���E�W
* �쐬���F2024/10/17
* �ŏI�X�V���F2024/10/22
*******************************************************/
#pragma once

/****************************************************
* �萔�錾
*****************************************************/
constexpr int KEY_MAX = 256;

/****************************************************
* �񋓑̐錾
*****************************************************/
enum MOUSEKEY {
	MOUSEKEY_LEFT = 0,
	MOUSEKEY_RIGHT,
	MOUSEKEY_CENTER,
	MOUSEKEY_SIDE
};

/****************************************************
* 2���������O�\����
*****************************************************/
struct LONG2 {
	LONG2() { x = 0, y = 0;};
	LONG2(LONG inix, LONG iniy) : x(inix), y(iniy) {}

	LONG x;
	LONG y;

	//�����Z
	LONG2 operator+ (const LONG2& l2) {
		LONG2 tmp(x + l2.x, y + l2.y);
		return tmp;
	}

	//�����Z
	LONG2 operator- (const LONG2& l2) {
		LONG2 tmp(x - l2.x, y + l2.y);
		return tmp;
	}

	//�|���Z
	LONG2 operator* (const float& f) {
		LONG2 tmp((LONG)(x * f), (LONG)(y * f));
		return tmp;
	}
};

/****************************************************
* �C���v�b�g
*****************************************************/
class Input {
public:
	//DirectInput������
	bool Initialize(const HINSTANCE& hInstance, const HWND& hWnd);
	//Input�X�V
	void UpdateInput();
	//Input�I��
	void Finalize();

	//���݂̃L�[�{�[�h����
	bool GetCurrentKeyboardDown(DWORD key);
	//��O�̃L�[�{�[�h����
	bool GetOldKeyboardDown(DWORD key);
	//�}�E�X�ړ���
	LONG2 GetMouseDelta();
	//���݂̃}�E�X����
	bool GetCurrentMouseDown(MOUSEKEY key);
	//��O�̃}�E�X�̓���
	bool GetOldMouseDown(MOUSEKEY key);
	//�Q�[���p�b�h���X�e�B�b�N
	LONG2 GetLeftStick(int padIndex);
	//�Q�[���p�b�h�E�X�e�B�b�N
	LONG2 GetRightStick(int padIndex);
	//���݂̏\���L�[
	LONG GetCurrentCrossKey(int padIndex);
	//��O�̏\���L�[
	LONG GetOldCrossKey(int padIndex);
	//���݂̃p�b�h�{�^��
	bool GetCurrentGamepadDown(int key, int padIndex);
	//��O�̃p�b�h�{�^��
	bool GetOldGamepadDown(int key, int padIndex);

	//�p�b�h���ڑ�����Ă��邩
	bool GetExistsGamepad(int padIndex);
	//�p�b�h�̑������擾
	int GetGamepadMax();

	//�p�b�h��
	friend BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
private:
	//DirectInput�I�u�W�F�N�g
	LPDIRECTINPUT8 m_directInput = nullptr;

	//�L�[�{�[�h�f�o�C�X
	LPDIRECTINPUTDEVICE8 m_keyboard = nullptr;
	//���̃L�[�{�[�h�̏��
	BYTE m_currentKeyboardState[KEY_MAX] = {};
	//��O�L�[�{�[�h�̏��
	BYTE m_oldKeyboardState[KEY_MAX] = {};

	//�}�E�X�f�o�C�X
	LPDIRECTINPUTDEVICE8 m_mouse = nullptr;
	//���̃}�E�X�̏��
	DIMOUSESTATE m_currntMouseState = {};
	//��O�̃}�E�X�̏��
	DIMOUSESTATE m_oldMouseState = {};

	//�Q�[���p�b�h���X�g
	std::vector<LPDIRECTINPUTDEVICE8> m_gamepads;
	//���̃Q�[���p�b�h�̏��
	DIJOYSTATE* m_currentGamepadsState = nullptr;
	//��O�̃Q�[���p�b�h�̏��
	DIJOYSTATE* m_oldGamepadsState = nullptr;

};