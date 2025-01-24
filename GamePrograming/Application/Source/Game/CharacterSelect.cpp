#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "CharacterSelect.h"
#include "Game/Controller.h"
#include <algorithm>

#define CURSORSIZE 50.0f
#define CHARAPOSX SCREEN_WIDTH * (1.0f / 7)
#define CHARAPOSY 400.0f

CharacterSelect::CharacterSelect() {

	m_totalPlayer = 4;
	m_controlPlayer = 4;

	//コントローラー取得
	
	for (int a = 0; a < 4; a++)
	{
		for (int i = 0; i < 6; i++)
		{
			m_iconflg[a][i] = false;
			m_selectflg[i] = false;
		}
		m_padSelectflg[a] = false;
	}

	// アイコンの範囲
	iconAreas = {
	{CHARAPOSX - 135.0f, CHARAPOSX + 130.0f, CHARAPOSY - 170.0f, CHARAPOSY + 200.0f},
	{(CHARAPOSX * 2) - 135.0f, (CHARAPOSX * 2) + 130.0f, CHARAPOSY - 170.0f, CHARAPOSY + 200.0f},
	{(CHARAPOSX * 3) - 135.0f, (CHARAPOSX * 3) + 130.0f, CHARAPOSY - 170.0f, CHARAPOSY + 200.0f},
	{(CHARAPOSX * 4) - 135.0f, (CHARAPOSX * 4) + 130.0f, CHARAPOSY - 170.0f, CHARAPOSY + 200.0f},
	{(CHARAPOSX * 5) - 135.0f, (CHARAPOSX * 5) + 130.0f, CHARAPOSY - 170.0f, CHARAPOSY + 200.0f},
	{(CHARAPOSX * 6) - 135.0f, (CHARAPOSX * 6) + 130.0f, CHARAPOSY - 170.0f, CHARAPOSY + 200.0f},
	};

	// プレイヤー状態切り替えボタンの範囲
	splayerAreas = {

	};

	m_cursorPos[0] = XMFLOAT2(80.0f, 80.0f);
	m_cursorPos[1] = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	m_cursorPos[2] = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	m_cursorPos[3] = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	m_cursorSize = XMFLOAT2(CURSORSIZE, CURSORSIZE);

	// プレイヤー状態切り替えの初期化
	for (int i = 0; i < 4; i++)
	{
		if (i <= m_totalPlayer - 1)
		{
			if (i <= m_controlPlayer - 1)
			{
				m_splayer[i] = SWITCH_PLAYER;
			}
			else
			{
				m_splayer[i] = SWITCH_CPU;
			}
		}
		else
		{
			m_splayer[i] = SWITCH_NULL;
		}
	}

	// カーソルテクスチャ
	m_cursor[0].Load(L"Data/Texture/hand.png");
	m_cursor[1].Load(L"Data/Texture/hand.png");
	m_cursor[2].Load(L"Data/Texture/hand.png");
	m_cursor[3].Load(L"Data/Texture/hand.png");

	// キャラクターテクスチャ
	m_character[0].Load(L"Data/Texture/ikemenicon.png");
	m_character[1].Load(L"Data/Texture/nekketsuicon.png");
	m_character[2].Load(L"Data/Texture/foxicon.png");
	m_character[3].Load(L"Data/Texture/womanicon.png");
	m_character[4].Load(L"Data/Texture/bisyoujoicon.png");
	m_character[5].Load(L"Data/Texture/chasarinicon.png");

	

	m_backGroundTex.Load(L"Data/Texture/CharacterSelectBg.png");
}

CharacterSelect::~CharacterSelect() {
	
	SaveData savedata;
	// 操作キャラクター番号をセット
	for (int i = 0; i < m_totalPlayer; i++)
	{
		switch (m_playerCharaNum[i])
		{
		case 0:
			m_CharacNum.push_back(CHARACTOR_01);
			break;
		case 1:
			m_CharacNum.push_back(CHARACTOR_02);
			break;
		case 2:
			m_CharacNum.push_back(CHARACTOR_03);
			break;
		case 3:
			m_CharacNum.push_back(CHARACTOR_04);
			break;
		case 4:
			m_CharacNum.push_back(CHARACTOR_05);
			break;
		case 5:
			m_CharacNum.push_back(CHARACTOR_06);
			break;
		default:
			break;
		}
	}

	savedata.SetTotalPlayer(m_totalPlayer);
	savedata.SetControlPlayer(m_controlPlayer);
	for (int i = 0; i < m_totalPlayer; i++)
	{
		if(m_splayer[i] == SWITCH_PLAYER)
		savedata.SetPlayerData(PlayerData{ *(std::next(m_CharacNum.begin(), i)), i });
		else if (m_splayer[i] == SWITCH_CPU)
		savedata.SetPlayerData(PlayerData{ *(std::next(m_CharacNum.begin(), i)), -1 });

	}

	// コントローラの解放
}

void CharacterSelect::Update() {

	/****************************************
	* 1/17 担当 カワマタトウ
	****************************************/
	// カーソル更新
	CursorUpdate();
	

	/*********************************************/

	//とりあえずエンターキーを押したら終了
	if (CTRL.GetKeyboardTrigger(DIK_RETURN) || m_padSelectflg[0] == m_padSelectflg[1] == m_padSelectflg[2] ==
		m_padSelectflg[3] == true && CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_TRIANGLE, 0)) {
		m_isFinished = true;
	}

}

void CharacterSelect::Draw() {
	//背景描画
	D3D.Draw2D(m_backGroundTex, XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));


	/****************************************
	* 1/17 担当 カワマタトウ
	****************************************/
	


	for (int i = 0; i < 6; i++)
	{
		// キャラ背景描画
		if(m_iconflg[0][i] || m_iconflg[1][i] || m_iconflg[2][i] || m_iconflg[3][i])
		D3D.Draw2D(m_character[i], XMFLOAT2(CHARAPOSX * (i + 1), CHARAPOSY), XMFLOAT2(550.0f, 550.0f));

		// キャラ描画
		D3D.Draw2D(m_character[i], XMFLOAT2(CHARAPOSX * (i + 1), CHARAPOSY), XMFLOAT2(500.0f, 500.0f));
	}

	// プレイヤーの操作キャラクター
	for (int i = 0; i < 4; i++)
	{
		switch (m_splayer[i])
		{
		case SWITCH_PLAYER:
			// プレイヤーキャラ背景
			//D3D.Draw2D(, XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)), 900.0f), XMFLOAT2(500.0f, 500.0f));
			if (m_padSelectflg[i])
			{
				// プレイヤーが選択したキャラクター
				D3D.Draw2D(m_character[m_playerCharaNum[i]], XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)), 900.0f), XMFLOAT2(500.0f, 500.0f));
			}
			// プレイヤー切り替えボタン
			//D3D.Draw2D(, XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)), 900.0f), XMFLOAT2(500.0f, 500.0f));
			break;
		case SWITCH_CPU:
			// プレイヤーキャラ背景
			//D3D.Draw2D(, XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)), 900.0f), XMFLOAT2(500.0f, 500.0f));
			if (m_padSelectflg[i])
			{
				// CPUが選択したキャラクター
				D3D.Draw2D(m_character[m_playerCharaNum[i]], XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)), 900.0f), XMFLOAT2(500.0f, 500.0f));
			}
			// プレイヤー切り替えボタン
			//D3D.Draw2D(, XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)), 900.0f), XMFLOAT2(500.0f, 500.0f));

			break;
		case SWITCH_NULL:
			// プレイヤーキャラ背景
			//D3D.Draw2D(, XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)), 900.0f), XMFLOAT2(500.0f, 500.0f));
			// プレイヤー切り替えボタン
			//D3D.Draw2D(, XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)), 900.0f), XMFLOAT2(500.0f, 500.0f));
			break;
		}
	}

	


	// カーソル描画
	for (int i = 0; i < m_controlPlayer; i++)
	{
		if(m_splayer[i] == SWITCH_PLAYER)
		D3D.Draw2D(m_cursor[i], m_cursorPos[i], m_cursorSize);
	}
	
}

void CharacterSelect::CursorUpdate()
{
	for (int i = 0; i < m_controlPlayer; i++)
	{
		// プレイヤー状態がプレイヤーの場合
		if (m_splayer[i] == SWITCH_PLAYER)
		{
			float cursorVel_x = (float)CTRL.GetLeftStickHorizontal(i);
			float cursorVel_y = (float)CTRL.GetLeftStickVertical(i);

			m_cursorPos[i].x += cursorVel_x * 0.01;
			m_cursorPos[i].y += cursorVel_y * 0.01;

			// 外に飛び出さないようにする
			m_cursorPos[i].x = std::clamp(m_cursorPos[i].x, CURSORSIZE * 0.5f, SCREEN_WIDTH - CURSORSIZE * 0.5f);
			m_cursorPos[i].y = std::clamp(m_cursorPos[i].y, CURSORSIZE * 0.5f, SCREEN_HEIGHT - CURSORSIZE * 0.5f);



			// プレイヤー状態切り替え
			// 要修正！
			for (size_t j = 0; j < splayerAreas.size(); ++j) {
				const Area& sArea = splayerAreas[j];

				if (m_cursorPos[i].x >= sArea.x_min && m_cursorPos[i].x <= sArea.x_max &&
					m_cursorPos[i].y >= sArea.y_min && m_cursorPos[i].y <= sArea.y_max &&
					CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CIRCLE, i)) {

					switch (m_splayer[j])
					{
					case SWITCH_PLAYER:
						m_splayer[j] = SWITCH_CPU;
						break;
					case SWITCH_CPU:
						m_splayer[j] = SWITCH_NULL;
						m_selectflg[m_playerCharaNum[j]] = false;
						m_padSelectflg[j] = true;
						break;
					case SWITCH_NULL:
						m_splayer[j] = SWITCH_PLAYER;
						m_padSelectflg[j] = false;
						break;
					}
				}
			}



			// アイコンとカーソルが重なっていて〇ボタンが押されたときキャラ選択フラグをtrueにする
			for (size_t j = 0; j < iconAreas.size(); ++j) {
				const Area& area = iconAreas[j];

				// カーソルがアイコン範囲に入っているかチェック
				if (m_cursorPos[i].x >= area.x_min && m_cursorPos[i].x <= area.x_max &&
					m_cursorPos[i].y >= area.y_min && m_cursorPos[i].y <= area.y_max && !m_selectflg[j]) {
					// 重なっていたらフラグをtrueにする
					m_iconflg[i][j] = true;

					// ○ボタンが押されたらキャラクター選択フラグをtrueにする
					if (CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CIRCLE, i) && !m_selectflg[j] && !m_padSelectflg[i]) {
						m_selectflg[j] = true;
						m_padSelectflg[i] = true;
						m_playerCharaNum[i] = j;
					}
				}
				else
				{
					m_iconflg[i][j] = false; // 範囲外ならフラグをリセット
				}

				// ×ボタンが押されたらキャラクター選択フラグをfalseにする
				if (CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CROSS, i) && m_selectflg[j])
				{
					m_selectflg[j] = false;
					m_padSelectflg[i] = false;
				}
			}
		}

		// プレイヤーの状態がCPUで1Pがキャラを選択していない場合
		
		// プレイヤーの状態がCPUで1Pがキャラを選択している場合
		 
		// 1PがCPUのキャラを選択できるようにする

	}
}




