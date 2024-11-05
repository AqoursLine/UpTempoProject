/******************************************************
* Controller.		コントローラー設定
* 制作者：ミヤタジョウジ
* 作成日：2024/10/17
* 最終更新日：2024/10/22
*******************************************************/
#include "framework.h"
#include "Source/DirectX/DirectX.h"
#include "DirectXMathConvert.inl"
#include "Controller.h"

/****************************************************
* コントローラー初期化
*****************************************************/
bool Controller::Initialize(HINSTANCE hInstance, HWND hWnd) {
	m_dInput.Initialize(hInstance, hWnd);

	//マウス感度初期化
	m_mouseSensitivity = 1;

	m_gamepadMax = m_dInput.GetGamepadMax();
	if (m_gamepadMax) {
		m_stickSensitivity = new float[m_gamepadMax];
		m_isUsed = new bool[m_gamepadMax];
		for (int i = 0; i < m_gamepadMax; i++) {
			m_stickSensitivity[i] = 1.0f;
			m_isUsed[i] = false;
		}
	}

	return false;
}

/****************************************************
* コントローラー更新
*****************************************************/
void Controller::UpdateController() {
	m_dInput.UpdateInput();
}

/****************************************************
* コントローラー終了
*****************************************************/
void Controller::Finalize() {
	//ゲームパッド削除
	delete[] m_stickSensitivity;
	delete[] m_isUsed;

	//Inputクラスを削除
	m_dInput.Finalize();
}

/****************************************************
* キーボード押したまま
*****************************************************/
const bool Controller::GetKeyboardPress(DWORD key) {
	return m_dInput.GetCurrentKeyboardDown(key);
}
/****************************************************
* キーボード押した瞬間
*****************************************************/
const bool Controller::GetKeyboardTrigger(DWORD key) {
	bool isTrigger = false;

	isTrigger = !m_dInput.GetOldKeyboardDown(key) && m_dInput.GetCurrentKeyboardDown(key);

	return isTrigger;
}
/****************************************************
* キーボード離した瞬間
*****************************************************/
const bool Controller::GetKeyboardRelease(DWORD key) {
	bool isRelease = false;

	isRelease = m_dInput.GetOldKeyboardDown(key) && !m_dInput.GetCurrentKeyboardDown(key);

	return isRelease;
}

/****************************************************
* マウスボタン押したまま
*****************************************************/
const bool Controller::GetMousePress(MOUSEKEY key) {
	return m_dInput.GetCurrentMouseDown(key);
}
/****************************************************
* マウスボタン押した瞬間
*****************************************************/
const bool Controller::GetMouseTrigger(MOUSEKEY key) {
	bool isTrigger = false;

	isTrigger = !m_dInput.GetOldMouseDown(key) && m_dInput.GetCurrentMouseDown(key);

	return isTrigger;
}
/****************************************************
* マウスボタン離した瞬間
*****************************************************/
const bool Controller::GetMouseRelease(MOUSEKEY key) {
	bool isRelease = false;

	isRelease = m_dInput.GetCurrentMouseDown(key) && !m_dInput.GetCurrentMouseDown(key);

	return isRelease;
}
/****************************************************
* マウス水平移動量
*****************************************************/
const LONG Controller::GetMouseDeltaHorizontal() {
	return LONG(m_dInput.GetMouseDelta().x * m_mouseSensitivity);
}
/****************************************************
* マウス垂直移動量
*****************************************************/
const LONG Controller::GetMouseDeltaVertical() {
	return LONG(m_dInput.GetMouseDelta().y * m_mouseSensitivity);
}
/****************************************************
* マウス感度設定
*****************************************************/
const void Controller::SetMouseSensitivity(float sensitivity) {
	m_mouseSensitivity = sensitivity;
}

/****************************************************
* ゲームパッド十字キー押したまま
*****************************************************/
const bool Controller::GetGamepadCrossPress(GAMEPAD_CROSS key, int padIndex) {
	bool isPress = false;
	if (m_dInput.GetExistsGamepad(padIndex)) {
		isPress = CalCross(m_dInput.GetCurrentCrossKey(padIndex), key);
	}
	return isPress;
}
/****************************************************
* ゲームパッド十字キー押した瞬間
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
* ゲームパッド十字キー離した瞬間
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
* 十字キー計算
*****************************************************/
bool Controller::CalCross(LONG angle, GAMEPAD_CROSS key) {
	bool isPress = false;

	//angleがマイナスなら
	if (angle < 0) {
		return false;
	}

	//数値から入力計算
	float rad = XMConvertToRadians((float)(angle) / 100.0f);
	float x = sinf(rad);
	float y = cosf(rad);

	//キーに対応
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
* ゲームパッド左スティック水平角度
*****************************************************/
const LONG Controller::GetLeftStickHorizontal(int padIndex) {
	LONG angle = 0;

	//取得するパッドがあるか
	if (m_dInput.GetExistsGamepad(padIndex)) {
		angle = m_dInput.GetLeftStick(padIndex).x;
		//あそび判定
		if (fabsf((float)angle) < 200) {
			angle = 0;
		}
		//感度補正
		angle *= m_stickSensitivity[padIndex];
	}

	return angle;
}
/****************************************************
* ゲームパッド左スティック垂直角度
*****************************************************/
const LONG Controller::GetLeftStickVertical(int padIndex) {
	LONG angle = 0;

	//取得するパッドがあるか
	if (m_dInput.GetExistsGamepad(padIndex)) {
		angle = m_dInput.GetLeftStick(padIndex).y;
		//あそび判定
		if (fabsf((float)angle) < 200) {
			angle = 0;
		}
		//感度補正
		angle *= m_stickSensitivity[padIndex];
	}

	return angle;
}
/****************************************************
* ゲームパッド右スティック水平角度
*****************************************************/
const LONG Controller::GetRightStickHorizontal(int padIndex) {
	LONG angle = 0;

	//取得するパッドがあるか
	if (m_dInput.GetExistsGamepad(padIndex)) {
		angle = m_dInput.GetRightStick(padIndex).x;
		//あそび判定
		if (fabsf((float)angle) < 200) {
			angle = 0;
		}
		//感度補正
		angle *= m_stickSensitivity[padIndex];
	}

	return angle;
}
/****************************************************
* ゲームパッド右スティック垂直角度
*****************************************************/
const LONG Controller::GetRightStickVertical(int padIndex) {
	LONG angle = 0;

	//取得するパッドがあるか
	if (m_dInput.GetExistsGamepad(padIndex)) {
		angle = m_dInput.GetRightStick(padIndex).y;
		//あそび判定
		if (fabsf((float)angle) < 200) {
			angle = 0;
		}
		//感度補正
		angle *= m_stickSensitivity[padIndex];
	}

	return angle;
}
/****************************************************
* ゲームパッドボタン押したまま
*****************************************************/
const bool Controller::GetGamepadButtonPress(int key, int padIndex) {
	bool isPress = false;
	if (m_dInput.GetExistsGamepad(padIndex)) {
		isPress = m_dInput.GetCurrentGamepadDown(key, padIndex);
	}
	return isPress;
}
/****************************************************
* ゲームパッドボタン押した瞬間
*****************************************************/
const bool Controller::GetGamepadButtonTrigger(int key, int padIndex) {
	bool isTrigger = false;
	if (m_dInput.GetExistsGamepad(padIndex)) {
		isTrigger = !m_dInput.GetOldGamepadDown(key, padIndex) && m_dInput.GetCurrentGamepadDown(key, padIndex);
	}
	return isTrigger;
}
/****************************************************
* ゲームパッドボタン離した瞬間
*****************************************************/
const bool Controller::GetGamepadButtonRelease(int key, int padIndex) {
	bool isRelease = false;
	if (m_dInput.GetExistsGamepad(padIndex)) {
		isRelease = m_dInput.GetOldGamepadDown(key, padIndex) && !m_dInput.GetCurrentGamepadDown(key, padIndex);
	}
	return isRelease;
}

/****************************************************
* ゲームパッドスティック感度設定
*****************************************************/
void Controller::SetStickSensitivity(float sensitivity, int padIndex) {
	if (m_dInput.GetExistsGamepad(padIndex)) {
		m_stickSensitivity[padIndex] = sensitivity;
	}
}

/****************************************************
* ゲームパッドハンドル取得
*****************************************************/
const int Controller::GetGamepadHandle() {
	for (int i = 0; i < m_gamepadMax; i++) {
		if (!m_isUsed[i]) {
			return i;
		}
	}

	return -1;
}

/****************************************************
* ゲームパッドハンドル解放
*****************************************************/
void Controller::ReleaseGamepadHandle(int i) {
	if (m_dInput.GetExistsGamepad(i)) {
		m_isUsed[i] = false;
	}
}
