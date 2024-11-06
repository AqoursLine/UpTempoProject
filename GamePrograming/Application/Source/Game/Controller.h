/******************************************************
* Controller.h		コントローラー設定
* 制作者：ミヤタジョウジ
* 作成日：2024/10/17
* 最終更新日：2024/10/22
*******************************************************/
#pragma once
#include "DirectX/Input.h"

/****************************************************
* 列挙体宣言
*****************************************************/
//ゲームパッド十字キー
enum GAMEPAD_CROSS {
	GAMEPAD_CROSS_UP = 0,
	GAMEPAD_CROSS_RIGHT,
	GAMEPAD_CROSS_DOWN,
	GAMEPAD_CROSS_LEFT,
};
//ゲームパッドボタン
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
* コントローラークラス
*****************************************************/
class Controller {
public:
	//コントローラーを初期化
	bool Initialize(HINSTANCE hInstance, HWND hWnd);
	//コントローラーを更新
	void UpdateController();
	//コントローラーを終了
	void Finalize();

	//キーボード押したまま
	const bool GetKeyboardPress(DWORD key);
	//キーボード押した瞬間
	const bool GetKeyboardTrigger(DWORD key);
	//キーボード離した瞬間
	const bool GetKeyboardRelease(DWORD key);

	//マウスボタン押したまま
	const bool GetMousePress(MOUSEKEY key);
	//マウスボタン押した瞬間
	const bool GetMouseTrigger(MOUSEKEY key);
	//マウスボタン離した瞬間
	const bool GetMouseRelease(MOUSEKEY key);
	//マウス水平移動量
	const LONG GetMouseDeltaHorizontal();
	//マウス垂直移動量
	const LONG GetMouseDeltaVertical();
	//マウス移動感度設定
	const void SetMouseSensitivity(float sensitivity);

	//ゲームパッド十字キー押したまま
	const bool GetGamepadCrossPress(GAMEPAD_CROSS key, int padIndex);
	//ゲームパッド十字キー押した瞬間
	const bool GetGamepadCrossTrigger(GAMEPAD_CROSS key, int padIndex);
	//ゲームパッド十字キー離した瞬間
	const bool GetGamepadCrossRelease(GAMEPAD_CROSS key, int padIndex);
	//ゲームパッド左スティック水平角度
	const LONG GetLeftStickHorizontal(int padIndex);
	//ゲームパッド左スティック垂直角度
	const LONG GetLeftStickVertical(int padIndex);
	//ゲームパッド右スティック水平角度
	const LONG GetRightStickHorizontal(int padIndex);
	//ゲームパッド右スティック垂直角度
	const LONG GetRightStickVertical(int padIndex);
	//ゲームパッドボタン押したまま
	const bool GetGamepadButtonPress(GAMEPAD_BUTTON key, int padIndex);
	//ゲームパッドボタン押した瞬間
	const bool GetGamepadButtonTrigger(GAMEPAD_BUTTON key, int padIndex);
	//ゲームパッドボタン離した瞬間
	const bool GetGamepadButtonRelease(GAMEPAD_BUTTON key, int padIndex);

	//ゲームパッドスティック感度設定
	void SetStickSensitivity(float sensitivity, int padIndex);
	//ゲームパッドハンドル取得
	const int GetGamepadHandle();
	//ゲームパッドハンドル解放
	void ReleaseGamepadHandle(int i);

private:
	//DirectInput
	Input m_dInput;

	//マウス感度
	float m_mouseSensitivity = 0;
	
	//ゲームパッド総数
	int m_gamepadMax = 0;
	//ゲームパッド使用済み
	bool* m_isUsed = nullptr;

	//十字キー計算
	bool CalCross(LONG angle, GAMEPAD_CROSS key);

	//このクラスも、どこからでもアクセスできるようにシングルトンパターン化
	//唯一のインスタンス用のポインタ
	static inline Controller* s_instance;
	//コンストラクタはprivateにする
	Controller() {}
public:
	//インスタンス生成
	static void CreateInstance() {
		DeleteInstance();

		s_instance = new Controller();
	}
	//インスタンス削除
	static void DeleteInstance() {
		if (s_instance != nullptr) {
			delete s_instance;
			s_instance = nullptr;
		}
	}
	//唯一のインスタンスを取得
	static Controller& GetInstance() {
		return *s_instance;
	}
};

#define CTRL Controller::GetInstance()
