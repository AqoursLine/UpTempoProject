/******************************************************
* input.cpp		DirectInput�ݒ�
* ����ҁF�~���^�W���E�W
* �쐬���F2024/10/17
* �ŏI�X�V���F2024/10/22
*******************************************************/
#include "framework.h"
#include "DirectX.h"
#include "Input.h"
#include "Application.h"
#include <algorithm>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

/****************************************************
* �v���g�^�C�v�錾
*****************************************************/
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

/****************************************************
* �C���v�b�g������
*****************************************************/
bool Input::Initialize(const HINSTANCE& hInstance, const HWND& hWnd) {
	//DirectInput8�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_directInput, nullptr))) {
		return false;
	}

	//�L�[�{�[�h�f�o�C�X�̍쐬
	if (FAILED(m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, nullptr))) {
		return false;
	}
	//�L�[�{�[�h�f�o�C�X�̃f�[�^�`����ݒ�
	if (FAILED(m_keyboard->SetDataFormat(&c_dfDIKeyboard))) {
		return false;
	}
	//�L�[�{�[�h�f�o�C�X�̋������[�h��ݒ�
	if (FAILED(m_keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) {
		return false;
	}
	//�L�[�{�[�h�f�o�C�X���g�p�\�ɂ���
	m_keyboard->Acquire();

	//�}�E�X�f�o�C�X�̍쐬
	if (FAILED(m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, nullptr))) {
		return false;
	}
	//�}�E�X�f�o�C�X�̃f�[�^�`����ݒ�
	if (FAILED(m_mouse->SetDataFormat(&c_dfDIMouse))) {
		return false;
	}
	//�}�E�X�f�o�C�X�̋������[�h��ݒ�
	if (FAILED(m_mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) {
		return false;
	}
	//�}�E�X�f�o�C�X���g�p�\�ɂ���
	m_mouse->Acquire();

	//�Q�[���p�b�h��
	if (FAILED(m_directInput->EnumDevices(DI8DEVCLASS_GAMECTRL, DeviceFindCallBack, (LPVOID)(this), DIEDFL_ATTACHEDONLY))) {
		return false;
	}
	//�Q�[���p�b�h��ԏ�����
	m_currentGamepadsState = new DIJOYSTATE[m_gamepads.size()];
	m_oldGamepadsState = new DIJOYSTATE[m_gamepads.size()];

	return true;
}

/****************************************************
* �C���v�b�g�X�V
*****************************************************/
void Input::UpdateInput() {
	//�ЂƂO�̏�Ԃ�ۑ�
	std::copy(std::begin(m_currentKeyboardState), std::end(m_currentKeyboardState), m_oldKeyboardState);
	m_oldMouseState = m_currntMouseState;
	for (int i = 0; i < m_gamepads.size(); i++) {
		m_oldGamepadsState[i] = m_currentGamepadsState[i];
	}

	//�L�[�{�[�h�̏�Ԃ��擾
	HRESULT hr = m_keyboard->GetDeviceState(sizeof(m_currentKeyboardState), (LPVOID)&m_currentKeyboardState);
	if (FAILED(hr)) {
		//�f�o�C�X���ꎞ�I�Ɏ���ꂽ�ꍇ�A�ēx�g�p�\�ɂ���
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
			m_keyboard->Acquire();
		}
	}

	//�}�E�X�̏�Ԃ��擾
	hr = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_currntMouseState);
	if (FAILED(hr)) {
		//�f�o�C�X���ꎞ�I�Ɏ���ꂽ�ꍇ�A�ēx�g�p�\�ɂ���
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
			m_mouse->Acquire();
		}
	}

	//�Q�[���p�b�h�̏�Ԃ��擾
	for (size_t i = 0; i < m_gamepads.size(); i++) {
		m_gamepads[i]->Poll();

		HRESULT hr = m_gamepads[i]->GetDeviceState(sizeof(DIJOYSTATE), &m_currentGamepadsState[i]);
		if (FAILED(hr)) {
			//�f�o�C�X������ꂽ�ꍇ�A�ēx�g�p�\�ɂ���
			if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
				m_gamepads[i]->Acquire();
				m_gamepads[i]->Poll();
			}
			continue;
		}
	}
}

/****************************************************
* �C���v�b�g�I��
*****************************************************/
void Input::Finalize() {
	//�L�[�{�[�h�f�o�C�X�̉��
	if (m_keyboard) {
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = nullptr;
	}

	//�}�E�X�f�o�C�X�̉��
	if (m_mouse) {
		m_mouse->Unacquire();
		m_mouse->Release();
		m_keyboard = nullptr;
	}

	//�Q�[���p�b�h�̉��
	for (size_t i = 0; i < m_gamepads.size(); i++) {
		if (m_gamepads[i]) {
			m_gamepads[i]->Unacquire();
			m_gamepads[i]->Release();
		}
	}
	//�f�o�C�X���X�g���N���A
	m_gamepads.clear();
	delete[] m_currentGamepadsState;
	delete[] m_oldGamepadsState;

	//DirectInput�I�u�W�F�N�g�̉��
	if (m_directInput) {
		m_directInput->Release();
		m_directInput = nullptr;
	}
}

/****************************************************
* ���݂̃L�[�{�[�h����
*****************************************************/
bool Input::GetCurrentKeyboardDown(DWORD key) {
	return (m_currentKeyboardState[key] & 0x80);
}

/****************************************************
* ��O�̃L�[�{�[�h����
*****************************************************/
bool Input::GetOldKeyboardDown(DWORD key) {
	return (m_oldKeyboardState[key] & 0x80);
}

/****************************************************
* �}�E�X�̈ړ���
*****************************************************/
LONG2 Input::GetMouseDelta() {
	LONG2 mouseDelta;
	mouseDelta.x = m_currntMouseState.lX;
	mouseDelta.y = m_currntMouseState.lY;

	return mouseDelta;
}

/****************************************************
* ���݂̃}�E�X�̓���
*****************************************************/
bool Input::GetCurrentMouseDown(MOUSEKEY key) {
	return (m_currntMouseState.rgbButtons[key] & 0x80);
}

/****************************************************
* ��O�̃}�E�X�̓���
*****************************************************/
bool Input::GetOldMouseDown(MOUSEKEY key) {
	return (m_oldMouseState.rgbButtons[key] & 0x80);
}

/****************************************************
* �Q�[���p�b�h���X�e�B�b�N
*****************************************************/
LONG2 Input::GetLeftStick(int padIndex) {
	LONG2 stick = { 0, 0 };
	//�Q�[���p�b�h�����݂��邩
	if (GetExistsGamepad(padIndex)) {
		stick.x = m_currentGamepadsState[padIndex].lX;
		stick.y = m_currentGamepadsState[padIndex].lY;

		//�l��1000���傫��������
		if (fabsf((float)stick.x) > 1000) {
			stick.x = 0;
		}
		if (fabsf((float)stick.y) > 1000) {
			stick.y = 0;
		}
	}

	return stick;
}

/****************************************************
* �Q�[���p�b�h�E�X�e�B�b�N
*****************************************************/
LONG2 Input::GetRightStick(int padIndex) {
	LONG2 stick = { 0, 0 };
	//�Q�[���p�b�h�����݂��邩
	if (GetExistsGamepad(padIndex)) {
		stick.x = m_currentGamepadsState[padIndex].lZ;
		stick.y = m_currentGamepadsState[padIndex].lRz;

		//�l��1000���傫��������
		if (fabsf((float)stick.x) > 1000) {
			stick.x = 0;
		}
		if (fabsf((float)stick.y) > 1000) {
			stick.y = 0;
		}
	}
	
	return stick;
}

/****************************************************
* ���݂̏\���L�[
*****************************************************/
LONG Input::GetCurrentCrossKey(int padIndex) {
	//�Q�[���p�b�h�����݂��邩
	if (GetExistsGamepad(padIndex)) {
		//���͂���Ă��邩
		if (m_currentGamepadsState[padIndex].rgdwPOV[0] != 0xFFFFFFFF) {
			return m_currentGamepadsState[padIndex].rgdwPOV[0];
		}
	}

	return -1;
}

/****************************************************
* ��O�̏\���L�[
*****************************************************/
LONG Input::GetOldCrossKey(int padIndex) {
	//�Q�[���p�b�h�����݂��邩
	if (GetExistsGamepad(padIndex)) {
		//���͂���Ă��邩
		if (m_oldGamepadsState[padIndex].rgdwPOV[0] != 0xFFFFFFFF) {
			return m_oldGamepadsState[padIndex].rgdwPOV[0];
		}
	}

	return -1;
}

/****************************************************
* ���݂̃p�b�h�{�^��
*****************************************************/
bool Input::GetCurrentGamepadDown(int key, int padIndex) {
	bool isDown = false;

	//�Q�[���p�b�h�����݂��邩
	if (GetExistsGamepad(padIndex)) {
		isDown = m_currentGamepadsState[padIndex].rgbButtons[key];
	}

	return isDown;
}

/****************************************************
* ��O�̃p�b�h�{�^��
*****************************************************/
bool Input::GetOldGamepadDown(int key, int padIndex) {
	bool isDown = false;

	//�Q�[���p�b�h�����݂��邩
	if (GetExistsGamepad(padIndex)) {
		isDown = m_oldGamepadsState[padIndex].rgbButtons[key];
	}

	return isDown;
}

/****************************************************
* �Q�[���p�b�h���ڑ�����Ă��邩
*****************************************************/
bool Input::GetExistsGamepad(int padIndex) {
	bool existsGamePad = false;

	if (padIndex >= 0 && padIndex < m_gamepads.size()) {
		existsGamePad = true;
	}

	return existsGamePad;
}

/****************************************************
* �ڑ�����Ă���Q�[���p�b�h�̑���
*****************************************************/
int Input::GetGamepadMax() {
	return (int)m_gamepads.size();
}

/****************************************************
* ���������Q�[���p�b�h������
*****************************************************/
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef) {
	LPDIRECTINPUTDEVICE8 gamePad = nullptr;

	Input* input = (Input*)pvRef;

	//�Q�[���p�b�h�f�o�C�X�̍쐬
	if (FAILED(input->m_directInput->CreateDevice(lpddi->guidInstance, &gamePad, nullptr))) {
		return DIENUM_CONTINUE;
	}

	//�f�o�C�X�̃f�[�^�^��ݒ�
	if (FAILED(gamePad->SetDataFormat(&c_dfDIJoystick))) {
		gamePad->Release();
		return DIENUM_CONTINUE;
	}

	//�����[�h���Βl���[�h�Ƃ��Đݒ�
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;	//��Βl���[�h�̎w��
	//�����[�h�ύX
	if (FAILED(gamePad->SetProperty(DIPROP_AXISMODE, &diprop.diph))) {
		return DIENUM_CONTINUE;
	}

	//���͈̔͐ݒ�
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	//X���̐ݒ�
	diprg.diph.dwObj = DIJOFS_X;
	if (FAILED(gamePad->SetProperty(DIPROP_RANGE, &diprg.diph))) {
		return DIENUM_CONTINUE;
	}
	//Y���̐ݒ�
	diprg.diph.dwObj = DIJOFS_Y;
	if (FAILED(gamePad->SetProperty(DIPROP_RANGE, &diprg.diph))) {
		return DIENUM_CONTINUE;
	}
	//Z���̐ݒ�
	diprg.diph.dwObj = DIJOFS_Z;
	if (FAILED(gamePad->SetProperty(DIPROP_RANGE, &diprg.diph))) {
		return DIENUM_CONTINUE;
	}
	//RX���̐ݒ�
	diprg.diph.dwObj = DIJOFS_RX;
	if (FAILED(gamePad->SetProperty(DIPROP_RANGE, &diprg.diph))) {
		return DIENUM_CONTINUE;
	}
	//RY���̐ݒ�
	diprg.diph.dwObj = DIJOFS_RY;
	if (FAILED(gamePad->SetProperty(DIPROP_RANGE, &diprg.diph))) {
		return DIENUM_CONTINUE;
	}
	//RZ���̐ݒ�
	diprg.diph.dwObj = DIJOFS_RZ;
	if (FAILED(gamePad->SetProperty(DIPROP_RANGE, &diprg.diph))) {
		return DIENUM_CONTINUE;
	}

	//�������[�h��ݒ�
	HWND hWnd = FindWindowW(GetWindowClass(), nullptr);
	if (FAILED(gamePad->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE))) {
		gamePad->Release();
		return DIENUM_CONTINUE;
	}

	//�f�o�C�X���g�p�\�ɂ���
	gamePad->Acquire();	//�|�[�����O���J�n����

	//���X�g�ɃQ�[���p�b�h��ǉ�
	input->m_gamepads.push_back(gamePad);
	
	//���s���đ��̃f�o�C�X����
	return DIENUM_CONTINUE;
}
