/******************************************************
* input.cpp		DirectInput設定
* 制作者：ミヤタジョウジ
* 作成日：2024/10/17
* 最終更新日：2024/10/22
*******************************************************/
#include "framework.h"
#include "DirectX.h"
#include "Input.h"
#include "Application.h"
#include <algorithm>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

/****************************************************
* プロトタイプ宣言
*****************************************************/
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

/****************************************************
* インプット初期化
*****************************************************/
bool Input::Initialize(const HINSTANCE& hInstance, const HWND& hWnd) {
	//DirectInput8オブジェクトの作成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_directInput, nullptr))) {
		return false;
	}

	//キーボードデバイスの作成
	if (FAILED(m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, nullptr))) {
		return false;
	}
	//キーボードデバイスのデータ形式を設定
	if (FAILED(m_keyboard->SetDataFormat(&c_dfDIKeyboard))) {
		return false;
	}
	//キーボードデバイスの協調モードを設定
	if (FAILED(m_keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) {
		return false;
	}
	//キーボードデバイスを使用可能にする
	m_keyboard->Acquire();

	//マウスデバイスの作成
	if (FAILED(m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, nullptr))) {
		return false;
	}
	//マウスデバイスのデータ形式を設定
	if (FAILED(m_mouse->SetDataFormat(&c_dfDIMouse))) {
		return false;
	}
	//マウスデバイスの協調モードを設定
	if (FAILED(m_mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) {
		return false;
	}
	//マウスデバイスを使用可能にする
	m_mouse->Acquire();

	//ゲームパッド列挙
	if (FAILED(m_directInput->EnumDevices(DI8DEVCLASS_GAMECTRL, DeviceFindCallBack, (LPVOID)(this), DIEDFL_ATTACHEDONLY))) {
		return false;
	}
	//ゲームパッド状態初期化
	m_currentGamepadsState = new DIJOYSTATE[m_gamepads.size()];
	m_oldGamepadsState = new DIJOYSTATE[m_gamepads.size()];

	return true;
}

/****************************************************
* インプット更新
*****************************************************/
void Input::UpdateInput() {
	//ひとつ前の状態を保存
	std::copy(std::begin(m_currentKeyboardState), std::end(m_currentKeyboardState), m_oldKeyboardState);
	m_oldMouseState = m_currntMouseState;
	for (int i = 0; i < m_gamepads.size(); i++) {
		m_oldGamepadsState[i] = m_currentGamepadsState[i];
	}

	//キーボードの状態を取得
	HRESULT hr = m_keyboard->GetDeviceState(sizeof(m_currentKeyboardState), (LPVOID)&m_currentKeyboardState);
	if (FAILED(hr)) {
		//デバイスが一時的に失われた場合、再度使用可能にする
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
			m_keyboard->Acquire();
		}
	}

	//マウスの状態を取得
	hr = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_currntMouseState);
	if (FAILED(hr)) {
		//デバイスが一時的に失われた場合、再度使用可能にする
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
			m_mouse->Acquire();
		}
	}

	//ゲームパッドの状態を取得
	for (size_t i = 0; i < m_gamepads.size(); i++) {
		m_gamepads[i]->Poll();

		HRESULT hr = m_gamepads[i]->GetDeviceState(sizeof(DIJOYSTATE), &m_currentGamepadsState[i]);
		if (FAILED(hr)) {
			//デバイスが失われた場合、再度使用可能にする
			if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
				m_gamepads[i]->Acquire();
				m_gamepads[i]->Poll();
			}
			continue;
		}
	}
}

/****************************************************
* インプット終了
*****************************************************/
void Input::Finalize() {
	//キーボードデバイスの解放
	if (m_keyboard) {
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = nullptr;
	}

	//マウスデバイスの解放
	if (m_mouse) {
		m_mouse->Unacquire();
		m_mouse->Release();
		m_keyboard = nullptr;
	}

	//ゲームパッドの解放
	for (size_t i = 0; i < m_gamepads.size(); i++) {
		if (m_gamepads[i]) {
			m_gamepads[i]->Unacquire();
			m_gamepads[i]->Release();
		}
	}
	//デバイスリストをクリア
	m_gamepads.clear();
	delete[] m_currentGamepadsState;
	delete[] m_oldGamepadsState;

	//DirectInputオブジェクトの解放
	if (m_directInput) {
		m_directInput->Release();
		m_directInput = nullptr;
	}
}

/****************************************************
* 現在のキーボード入力
*****************************************************/
bool Input::GetCurrentKeyboardDown(DWORD key) {
	return (m_currentKeyboardState[key] & 0x80);
}

/****************************************************
* 一つ前のキーボード入力
*****************************************************/
bool Input::GetOldKeyboardDown(DWORD key) {
	return (m_oldKeyboardState[key] & 0x80);
}

/****************************************************
* マウスの移動量
*****************************************************/
LONG2 Input::GetMouseDelta() {
	LONG2 mouseDelta;
	mouseDelta.x = m_currntMouseState.lX;
	mouseDelta.y = m_currntMouseState.lY;

	return mouseDelta;
}

/****************************************************
* 現在のマウスの入力
*****************************************************/
bool Input::GetCurrentMouseDown(MOUSEKEY key) {
	return (m_currntMouseState.rgbButtons[key] & 0x80);
}

/****************************************************
* 一つ前のマウスの入力
*****************************************************/
bool Input::GetOldMouseDown(MOUSEKEY key) {
	return (m_oldMouseState.rgbButtons[key] & 0x80);
}

/****************************************************
* ゲームパッド左スティック
*****************************************************/
LONG2 Input::GetLeftStick(int padIndex) {
	LONG2 stick = { 0, 0 };
	//ゲームパッドが存在するか
	if (GetExistsGamepad(padIndex)) {
		stick.x = m_currentGamepadsState[padIndex].lX;
		stick.y = m_currentGamepadsState[padIndex].lY;

		//値が1000より大きかったら
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
* ゲームパッド右スティック
*****************************************************/
LONG2 Input::GetRightStick(int padIndex) {
	LONG2 stick = { 0, 0 };
	//ゲームパッドが存在するか
	if (GetExistsGamepad(padIndex)) {
		stick.x = m_currentGamepadsState[padIndex].lZ;
		stick.y = m_currentGamepadsState[padIndex].lRz;

		//値が1000より大きかったら
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
* 現在の十字キー
*****************************************************/
LONG Input::GetCurrentCrossKey(int padIndex) {
	//ゲームパッドが存在するか
	if (GetExistsGamepad(padIndex)) {
		//入力されているか
		if (m_currentGamepadsState[padIndex].rgdwPOV[0] != 0xFFFFFFFF) {
			return m_currentGamepadsState[padIndex].rgdwPOV[0];
		}
	}

	return -1;
}

/****************************************************
* 一つ前の十字キー
*****************************************************/
LONG Input::GetOldCrossKey(int padIndex) {
	//ゲームパッドが存在するか
	if (GetExistsGamepad(padIndex)) {
		//入力されているか
		if (m_oldGamepadsState[padIndex].rgdwPOV[0] != 0xFFFFFFFF) {
			return m_oldGamepadsState[padIndex].rgdwPOV[0];
		}
	}

	return -1;
}

/****************************************************
* 現在のパッドボタン
*****************************************************/
bool Input::GetCurrentGamepadDown(int key, int padIndex) {
	bool isDown = false;

	//ゲームパッドが存在するか
	if (GetExistsGamepad(padIndex)) {
		isDown = m_currentGamepadsState[padIndex].rgbButtons[key];
	}

	return isDown;
}

/****************************************************
* 一つ前のパッドボタン
*****************************************************/
bool Input::GetOldGamepadDown(int key, int padIndex) {
	bool isDown = false;

	//ゲームパッドが存在するか
	if (GetExistsGamepad(padIndex)) {
		isDown = m_oldGamepadsState[padIndex].rgbButtons[key];
	}

	return isDown;
}

/****************************************************
* ゲームパッドが接続されているか
*****************************************************/
bool Input::GetExistsGamepad(int padIndex) {
	bool existsGamePad = false;

	if (padIndex >= 0 && padIndex < m_gamepads.size()) {
		existsGamePad = true;
	}

	return existsGamePad;
}

/****************************************************
* 接続されているゲームパッドの総数
*****************************************************/
int Input::GetGamepadMax() {
	return (int)m_gamepads.size();
}

/****************************************************
* 発見したゲームパッド初期化
*****************************************************/
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef) {
	LPDIRECTINPUTDEVICE8 gamePad = nullptr;

	Input* input = (Input*)pvRef;

	//ゲームパッドデバイスの作成
	if (FAILED(input->m_directInput->CreateDevice(lpddi->guidInstance, &gamePad, nullptr))) {
		return DIENUM_CONTINUE;
	}

	//デバイスのデータ型を設定
	if (FAILED(gamePad->SetDataFormat(&c_dfDIJoystick))) {
		gamePad->Release();
		return DIENUM_CONTINUE;
	}

	//軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;	//絶対値モードの指定
	//軸モード変更
	if (FAILED(gamePad->SetProperty(DIPROP_AXISMODE, &diprop.diph))) {
		return DIENUM_CONTINUE;
	}

	//軸の範囲設定
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	//X軸の設定
	diprg.diph.dwObj = DIJOFS_X;
	if (FAILED(gamePad->SetProperty(DIPROP_RANGE, &diprg.diph))) {
		return DIENUM_CONTINUE;
	}
	//Y軸の設定
	diprg.diph.dwObj = DIJOFS_Y;
	if (FAILED(gamePad->SetProperty(DIPROP_RANGE, &diprg.diph))) {
		return DIENUM_CONTINUE;
	}
	//Z軸の設定
	diprg.diph.dwObj = DIJOFS_Z;
	if (FAILED(gamePad->SetProperty(DIPROP_RANGE, &diprg.diph))) {
		return DIENUM_CONTINUE;
	}
	//RX軸の設定
	diprg.diph.dwObj = DIJOFS_RX;
	if (FAILED(gamePad->SetProperty(DIPROP_RANGE, &diprg.diph))) {
		return DIENUM_CONTINUE;
	}
	//RY軸の設定
	diprg.diph.dwObj = DIJOFS_RY;
	if (FAILED(gamePad->SetProperty(DIPROP_RANGE, &diprg.diph))) {
		return DIENUM_CONTINUE;
	}
	//RZ軸の設定
	diprg.diph.dwObj = DIJOFS_RZ;
	if (FAILED(gamePad->SetProperty(DIPROP_RANGE, &diprg.diph))) {
		return DIENUM_CONTINUE;
	}

	//協調モードを設定
	HWND hWnd = FindWindowW(GetWindowClass(), nullptr);
	if (FAILED(gamePad->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE))) {
		gamePad->Release();
		return DIENUM_CONTINUE;
	}

	//デバイスを使用可能にする
	gamePad->Acquire();	//ポーリングを開始する

	//リストにゲームパッドを追加
	input->m_gamepads.push_back(gamePad);
	
	//続行して他のデバイスも列挙
	return DIENUM_CONTINUE;
}
