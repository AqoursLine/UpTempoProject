/******************************************************
* Controller.		?R???g???[???[?ݒ?
* ?????F?~???^?W???E?W
* ?쐬???F2024/10/17
* ?ŏI?X?V???F2024/10/22
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "DirectXMathConvert.inl"
#include "Controller.h"

/****************************************************
* ?R???g???[???[??????
*****************************************************/
bool Controller::Initialize(HINSTANCE hInstance, HWND hWnd) {
	m_dInput.Initialize(hInstance, hWnd);

	//?}?E?X???x??????
	m_mouseSensitivity = 1;

	m_gamepadMax = m_dInput.GetGamepadMax();
	if (m_gamepadMax) {
		m_isUsed = new bool[m_gamepadMax];
		for (int i = 0; i < m_gamepadMax; i++) {
			m_isUsed[i] = false;
		}
	}

	return false;
}

/****************************************************
* ?R???g???[???[?X?V
*****************************************************/
void Controller::UpdateController() {
	m_dInput.UpdateInput();
}

/****************************************************
* ?R???g???[???[?I??
*****************************************************/
void Controller::Finalize() {
	//?Q?[???p?b?h?폜
	delete[] m_isUsed;

	//Input?N???X??폜
	m_dInput.Finalize();
}

/****************************************************
* ?L?[?{?[?h???????܂?
*****************************************************/
const bool Controller::GetKeyboardPress(DWORD key) {
	return m_dInput.GetCurrentKeyboardDown(key);
}
/****************************************************
* ?L?[?{?[?h???????u??
*****************************************************/
const bool Controller::GetKeyboardTrigger(DWORD key) {
	bool isTrigger = false;

	isTrigger = !m_dInput.GetOldKeyboardDown(key) && m_dInput.GetCurrentKeyboardDown(key);

	return isTrigger;
}
/****************************************************
* ?L?[?{?[?h???????u??
*****************************************************/
const bool Controller::GetKeyboardRelease(DWORD key) {
	bool isRelease = false;

	isRelease = m_dInput.GetOldKeyboardDown(key) && !m_dInput.GetCurrentKeyboardDown(key);

	return isRelease;
}

/****************************************************
* ?}?E?X?{?^?????????܂?
*****************************************************/
const bool Controller::GetMousePress(MOUSEKEY key) {
	return m_dInput.GetCurrentMouseDown(key);
}
/****************************************************
* ?}?E?X?{?^?????????u??
*****************************************************/
const bool Controller::GetMouseTrigger(MOUSEKEY key) {
	bool isTrigger = false;

	isTrigger = !m_dInput.GetOldMouseDown(key) && m_dInput.GetCurrentMouseDown(key);

	return isTrigger;
}
/****************************************************
* ?}?E?X?{?^?????????u??
*****************************************************/
const bool Controller::GetMouseRelease(MOUSEKEY key) {
	bool isRelease = false;

	isRelease = m_dInput.GetCurrentMouseDown(key) && !m_dInput.GetCurrentMouseDown(key);

	return isRelease;
}
/****************************************************
* ?}?E?X?????ړ???
*****************************************************/
const LONG Controller::GetMouseDeltaHorizontal() {
	return LONG(m_dInput.GetMouseDelta().x * m_mouseSensitivity);
}
/****************************************************
* ?}?E?X?????ړ???
*****************************************************/
const LONG Controller::GetMouseDeltaVertical() {
	return LONG(m_dInput.GetMouseDelta().y * m_mouseSensitivity);
}
/****************************************************
* ?}?E?X???x?ݒ?
*****************************************************/
const void Controller::SetMouseSensitivity(float sensitivity) {
	m_mouseSensitivity = sensitivity;
}

/****************************************************
* ?Q?[???p?b?h?\???L?[???????܂?
*****************************************************/
const bool Controller::GetGamepadCrossPress(GAMEPAD_CROSS key, int padIndex) {
	bool isPress = false;
	if (m_dInput.GetExistsGamepad(padIndex)) {
		isPress = CalCross(m_dInput.GetCurrentCrossKey(padIndex), key);
	}
	return isPress;
}
/****************************************************
* ?Q?[???p?b?h?\???L?[???????u??
*****************************************************/
const bool Controller::GetGamepadCrossTrigger(GAMEPAD_CROSS key, int padIndex) {
	bool isTrigger = false;
	if (m_dInput.GetExistsGamepad(padIndex)) {
		isTrigger = CalCross(m_dInput.GetOldCrossKey(padIndex), key);
		isTrigger = !isTrigger && CalCross(m_dInput.GetCurrentCrossKey(padIndex), key);
	}
	return isTrigger;
}
/****************************************************
* ?Q?[???p?b?h?\???L?[???????u??
*****************************************************/
const bool Controller::GetGamepadCrossRelease(GAMEPAD_CROSS key, int padIndex) {
	bool isRelease = false;

	if (m_dInput.GetExistsGamepad(padIndex)) {
		isRelease = CalCross(m_dInput.GetOldCrossKey(padIndex), key);
		isRelease = isRelease && !CalCross(m_dInput.GetCurrentCrossKey(padIndex), key);
	}

	return isRelease;
}
/****************************************************
* ?\???L?[?v?Z
*****************************************************/
bool Controller::CalCross(LONG angle, GAMEPAD_CROSS key) {
	bool isPress = false;

	//angle???}?C?i?X?Ȃ?
	if (angle < 0) {
		return false;
	}

	//???l?????͌v?Z
	float rad = XMConvertToRadians((float)(angle) / 100.0f);
	float x = sinf(rad);
	float y = cosf(rad);

	//?L?[?ɑΉ?
	switch (key) {
		case GAMEPAD_CROSS_UP:
			isPress = y > 0.1f;
			break;
		case GAMEPAD_CROSS_RIGHT:
			isPress = x > 0.1f;
			break;
		case GAMEPAD_CROSS_DOWN:
			isPress = y < -0.1f;
			break;
		case GAMEPAD_CROSS_LEFT:
			isPress = x < -0.1f;
			break;
	}

	return isPress;
}
/****************************************************
* ?Q?[???p?b?h???X?e?B?b?N?????p?x
*****************************************************/
const LONG Controller::GetLeftStickHorizontal(int padIndex) {
	LONG angle = 0;

	//?擾?????b?h?????邩
	if (m_dInput.GetExistsGamepad(padIndex)) {
		angle = m_dInput.GetLeftStick(padIndex).x;
		//?????є???
		if (fabsf((float)angle) < 200) {
			angle = 0;
		}
	}

	return angle;
}
/****************************************************
* ?Q?[???p?b?h???X?e?B?b?N?????p?x
*****************************************************/
const LONG Controller::GetLeftStickVertical(int padIndex) {
	LONG angle = 0;

	//?擾?????b?h?????邩
	if (m_dInput.GetExistsGamepad(padIndex)) {
		angle = m_dInput.GetLeftStick(padIndex).y;
		//?????є???
		if (fabsf((float)angle) < 200) {
			angle = 0;
		}
	}

	return angle;
}
/****************************************************
* ?Q?[???p?b?h?E?X?e?B?b?N?????p?x
*****************************************************/
const LONG Controller::GetRightStickHorizontal(int padIndex) {
	LONG angle = 0;

	//?擾?????b?h?????邩
	if (m_dInput.GetExistsGamepad(padIndex)) {
		angle = m_dInput.GetRightStick(padIndex).x;
		//?????є???
		if (fabsf((float)angle) < 200) {
			angle = 0;
		}
	}

	return angle;
}
/****************************************************
* ?Q?[???p?b?h?E?X?e?B?b?N?????p?x
*****************************************************/
const LONG Controller::GetRightStickVertical(int padIndex) {
	LONG angle = 0;

	//?擾?????b?h?????邩
	if (m_dInput.GetExistsGamepad(padIndex)) {
		angle = m_dInput.GetRightStick(padIndex).y;
		//?????є???
		if (fabsf((float)angle) < 200) {
			angle = 0;
		}
	}

	return angle;
}
/****************************************************
* ?Q?[???p?b?h?{?^?????????܂?
*****************************************************/
const bool Controller::GetGamepadButtonPress(GAMEPAD_BUTTON key, int padIndex) {
	bool isPress = false;
	if (m_dInput.GetExistsGamepad(padIndex)) {
		isPress = m_dInput.GetCurrentGamepadDown(key, padIndex);
	}
	return isPress;
}
/****************************************************
* ?Q?[???p?b?h?{?^?????????u??
*****************************************************/
const bool Controller::GetGamepadButtonTrigger(GAMEPAD_BUTTON key, int padIndex) {
	bool isTrigger = false;
	if (m_dInput.GetExistsGamepad(padIndex)) {
		isTrigger = !m_dInput.GetOldGamepadDown(key, padIndex) && m_dInput.GetCurrentGamepadDown(key, padIndex);
	}
	return isTrigger;
}
/****************************************************
* ?Q?[???p?b?h?{?^?????????u??
*****************************************************/
const bool Controller::GetGamepadButtonRelease(GAMEPAD_BUTTON key, int padIndex) {
	bool isRelease = false;
	if (m_dInput.GetExistsGamepad(padIndex)) {
		isRelease = m_dInput.GetOldGamepadDown(key, padIndex) && !m_dInput.GetCurrentGamepadDown(key, padIndex);
	}
	return isRelease;
}

/****************************************************
* ?Q?[???p?b?h?n???h???擾
*****************************************************/
const int Controller::GetGamepadHandle() {
	for (int i = 0; i < m_gamepadMax; i++) {
		if (!m_isUsed[i]) {
			m_isUsed[i] = true;
			return i;
		}
	}

	return -1;
}

/****************************************************
* ?Q?[???p?b?h?n???h????
*****************************************************/
void Controller::ReleaseGamepadHandle(int i) {
	if (m_dInput.GetExistsGamepad(i)) {
		m_isUsed[i] = false;
	}
}
