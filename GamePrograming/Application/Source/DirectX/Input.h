/******************************************************
* input.h		DirectInput設定
* 制作者：ミヤタジョウジ
* 作成日：2024/10/17
* 最終更新日：2024/10/22
*******************************************************/
#pragma once

/****************************************************
* 定数宣言
*****************************************************/
constexpr int KEY_MAX = 256;

/****************************************************
* 列挙体宣言
*****************************************************/
enum MOUSEKEY {
	MOUSEKEY_LEFT = 0,
	MOUSEKEY_RIGHT,
	MOUSEKEY_CENTER,
	MOUSEKEY_SIDE
};

/****************************************************
* 2次元ロング構造体
*****************************************************/
struct LONG2 {
	LONG2() { x = 0, y = 0;};
	LONG2(LONG inix, LONG iniy) : x(inix), y(iniy) {}

	LONG x;
	LONG y;

	//足し算
	LONG2 operator+ (const LONG2& l2) {
		LONG2 tmp(x + l2.x, y + l2.y);
		return tmp;
	}

	//引き算
	LONG2 operator- (const LONG2& l2) {
		LONG2 tmp(x - l2.x, y + l2.y);
		return tmp;
	}

	//掛け算
	LONG2 operator* (const float& f) {
		LONG2 tmp((LONG)(x * f), (LONG)(y * f));
		return tmp;
	}
};

/****************************************************
* インプット
*****************************************************/
class Input {
public:
	//DirectInput初期化
	bool Initialize(const HINSTANCE& hInstance, const HWND& hWnd);
	//Input更新
	void UpdateInput();
	//Input終了
	void Finalize();

	//現在のキーボード入力
	bool GetCurrentKeyboardDown(DWORD key);
	//一つ前のキーボード入力
	bool GetOldKeyboardDown(DWORD key);
	//マウス移動量
	LONG2 GetMouseDelta();
	//現在のマウス入力
	bool GetCurrentMouseDown(MOUSEKEY key);
	//一つ前のマウスの入力
	bool GetOldMouseDown(MOUSEKEY key);
	//ゲームパッド左スティック
	LONG2 GetLeftStick(int padIndex);
	//ゲームパッド右スティック
	LONG2 GetRightStick(int padIndex);
	//現在の十字キー
	LONG GetCurrentCrossKey(int padIndex);
	//一つ前の十字キー
	LONG GetOldCrossKey(int padIndex);
	//現在のパッドボタン
	bool GetCurrentGamepadDown(int key, int padIndex);
	//一つ前のパッドボタン
	bool GetOldGamepadDown(int key, int padIndex);

	//パッドが接続されているか
	bool GetExistsGamepad(int padIndex);
	//パッドの総数を取得
	int GetGamepadMax();

	//パッド列挙
	friend BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
private:
	//DirectInputオブジェクト
	LPDIRECTINPUT8 m_directInput = nullptr;

	//キーボードデバイス
	LPDIRECTINPUTDEVICE8 m_keyboard = nullptr;
	//今のキーボードの状態
	BYTE m_currentKeyboardState[KEY_MAX] = {};
	//一つ前キーボードの状態
	BYTE m_oldKeyboardState[KEY_MAX] = {};

	//マウスデバイス
	LPDIRECTINPUTDEVICE8 m_mouse = nullptr;
	//今のマウスの状態
	DIMOUSESTATE m_currntMouseState = {};
	//一つ前のマウスの状態
	DIMOUSESTATE m_oldMouseState = {};

	//ゲームパッドリスト
	std::vector<LPDIRECTINPUTDEVICE8> m_gamepads;
	//今のゲームパッドの状態
	DIJOYSTATE* m_currentGamepadsState = nullptr;
	//一つ前のゲームパッドの状態
	DIJOYSTATE* m_oldGamepadsState = nullptr;

};