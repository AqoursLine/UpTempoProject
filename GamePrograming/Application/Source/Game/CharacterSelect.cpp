#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "CharacterSelect.h"
#include "Game/Controller.h"
#include <algorithm>



//#define CHARAPOSX (SCREEN_WIDTH * (1.0f / 7))
//#define CHARAPOSY 400.0f
//#define CHANGEPOSX SCREEN_WIDTH * (1.0f / 5) 
//#define CHANGEPOSY 850.0f

// CPU選択の際に1Pのコントローラーを同じフレームで使うため、キートリガーが実質プレスと同じ挙動になってしまう。
// そのため、一回選択したらCPU選択の処理を次のフレームまでしないようにする。そのフラグ。
bool g_isKeyReleased = true;

CharacterSelect::CharacterSelect() {


	m_totalPlayer = 4; // 2
	//m_controlPlayer = SaveData::GetControlPlayer();
	// デバッグ用
	m_controlPlayer = 1;

	m_CPURun = false;
	m_totalCPU = 0;
	m_lastCPU = 0;

	// 接続しているコントローラ分SWITCH_PLAYER
	for (int i = 0; i < 4; i++)
	{
		if (i < m_controlPlayer)
		{
			m_splayer[i] = SWITCH_PLAYER;
		}
		else
		{
			m_splayer[i] = SWITCH_CPU;
			m_totalCPU++;
		}
	}
	

	for (int a = 0; a < 4; a++)
	{
		for (int i = 0; i < 6; i++)
		{
			m_iconflg[a][i] = false;
			m_selectflg[i] = false;
		}
		if (m_splayer[a] == SWITCH_NULL)
		{
			m_padSelectflg[a] = true;
		}
		else
		{
			m_padSelectflg[a] = false;
		}
		//コントローラー取得
		m_padIndex[a] = CTRL.GetGamepadHandle();
	}

	// アイコンの範囲 2対3
	iconAreas = {
	{(SCREEN_WIDTH * (1.0f / 7)) - 135.0f, (SCREEN_WIDTH * (1.0f / 7)) + 130.0f, 230.0f, 600.0f},
	{((SCREEN_WIDTH * (1.0f / 7)) * 2) - 135.0f, ((SCREEN_WIDTH * (1.0f / 7)) * 2) + 130.0f, 230.0f, 600.0f},
	{((SCREEN_WIDTH * (1.0f / 7)) * 3) - 135.0f, ((SCREEN_WIDTH * (1.0f / 7)) * 3) + 130.0f, 230.0f, 600.0f},
	{((SCREEN_WIDTH * (1.0f / 7)) * 4) - 135.0f, ((SCREEN_WIDTH * (1.0f / 7)) * 4) + 130.0f, 230.0f, 600.0f},
	{((SCREEN_WIDTH * (1.0f / 7)) * 5) - 135.0f, ((SCREEN_WIDTH * (1.0f / 7)) * 5) + 130.0f, 230.0f, 600.0f},
	{((SCREEN_WIDTH * (1.0f / 7)) * 6) - 135.0f, ((SCREEN_WIDTH * (1.0f / 7)) * 6) + 130.0f, 230.0f, 600.0f},
	};

	// プレイヤー状態切り替えボタンの範囲(仮)
	splayerAreas =
	{
		{ (SCREEN_WIDTH * (1.0f / 5) * 2) - 150.0f, (SCREEN_WIDTH * (1.0f / 5) * 2) + 100.0f, 700.0f, 900.0f },	// 2P
		{ (SCREEN_WIDTH * (1.0f / 5) * 3) - 150.0f, (SCREEN_WIDTH * (1.0f / 5) * 3) + 100.0f, 700.0f, 900.0f },	// 3P
		{ (SCREEN_WIDTH * (1.0f / 5) * 4) - 150.0f, (SCREEN_WIDTH * (1.0f / 5) * 4) + 100.0f, 700.0f, 900.0f }	// 4P
	};

	m_cursorPos[0] = XMFLOAT2(1100.0f, 850.0f);
	m_cursorPos[1] = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	m_cursorPos[2] = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	m_cursorPos[3] = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	m_cursorSize = XMFLOAT2(50.0f, 50.0f);

	
	
	

	// カーソルテクスチャ
	m_cursor[0].Load(L"Data/Texture/hand1.png");
	m_cursor[1].Load(L"Data/Texture/hand2.png");
	m_cursor[2].Load(L"Data/Texture/hand3.png");
	m_cursor[3].Load(L"Data/Texture/hand4.png");

	// キャラクターテクスチャ
	m_character[0].Load(L"Data/Texture/ikemenicon.png");
	m_character[1].Load(L"Data/Texture/nekketsuicon.png");
	m_character[2].Load(L"Data/Texture/foxicon.png");
	m_character[3].Load(L"Data/Texture/womanicon.png");
	m_character[4].Load(L"Data/Texture/bisyoujoicon.png");
	m_character[5].Load(L"Data/Texture/chasarinicon.png");

	// キャラクターアイコンテクスチャ
	m_charaicon[0].Load(L"Data/Texture/ikemenicon1.png");
	m_charaicon[1].Load(L"Data/Texture/nekketsuicon1.png");
	m_charaicon[2].Load(L"Data/Texture/foxicon1.png");
	m_charaicon[3].Load(L"Data/Texture/womanicon1.png");
	m_charaicon[4].Load(L"Data/Texture/bisyoujoicon1.png");
	m_charaicon[5].Load(L"Data/Texture/chasarinicon1.png");

	// プレイヤーテクスチャ背景
	m_playerBg[0].Load(L"Data/Texture/player1Bg.png");
	m_playerBg[1].Load(L"Data/Texture/player2Bg.png");
	//m_playerBg[2].Load(L"Data/Texture/player3Bg.png");
	//m_playerBg[3].Load(L"Data/Texture/player4Bg.png");
	m_playerBg[4].Load(L"Data/Texture/playerCPUBg.png");
	m_playerBg[5].Load(L"Data/Texture/playernullBg.png");

	// プレイヤー状態切り替えボタン
	//m_changebutton[0].Load(L"Data/Texture/.png");		//1P
	//m_changebutton[1].Load(L"Data/Texture/.png");		//2P
	//m_changebutton[2].Load(L"Data/Texture/.png");		//3P
	//m_changebutton[3].Load(L"Data/Texture/.png");		//4P
	//m_changebutton[4].Load(L"Data/Texture/.png");		//CPU
	//m_changebutton[5].Load(L"Data/Texture/.png");		//なし

	m_heading.Load(L"Data/Texture/CharacterSelect.png");

	m_backGroundTex.Load(L"Data/Texture/CharacterSelectBg.png");
}

CharacterSelect::~CharacterSelect()
{
	
	
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

	// プレイヤーデータ保存
	for (int i = 0; i < 4; i++)
	{
		if (m_splayer[i] == SWITCH_PLAYER)
			SaveData::SetPlayerData(PlayerData{ *(std::next(m_CharacNum.begin(), i)), i ,true,i + 1 });
		else if (m_splayer[i] == SWITCH_CPU)
			SaveData::SetPlayerData(PlayerData{ *(std::next(m_CharacNum.begin(), i)), -1 ,false,i + 1 });

	}

	SaveData::SetTotalPlayer(m_totalPlayer);
	SaveData::SetControlPlayer(m_controlPlayer);
	// コントローラの解放
	for (int i = 0; i < 4; i++)
	{
		CTRL.ReleaseGamepadHandle(m_padIndex[i]);
	}
}

void CharacterSelect::Update() {

	/****************************************
	* 1/17 担当 カワマタトウ
	****************************************/
	// カーソル更新
	CursorUpdate();
	

	/*********************************************/

	//とりあえずエンターキーを押したら終了
	if (CTRL.GetKeyboardTrigger(DIK_RETURN) || m_padSelectflg[0] == m_padSelectflg[1] && m_padSelectflg[2] &&
		m_padSelectflg[3] && CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_TRIANGLE, 0)) {
		m_isFinished = true;
	}


	g_isKeyReleased = true;
}

void CharacterSelect::Draw() {
	//背景描画
	D3D.Draw2D(m_backGroundTex, XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));

	/****************************************
	* 1/17 担当 カワマタトウ
	****************************************/
	D3D.Draw2D(m_heading, XMFLOAT2(SCREEN_WIDTH / 2,100.0f ), XMFLOAT2(800.0f, 150.0f));
	

	for (int i = 0; i < 6; i++)
	{
		// キャラ背景描画
		if(!m_selectflg[i] && m_iconflg[0][i] || m_iconflg[1][i] || m_iconflg[2][i] || m_iconflg[3][i])
		{
			D3D.Draw2D(m_character[i], XMFLOAT2((SCREEN_WIDTH * (1.0f / 7)) * (i + 1), 400.0f), XMFLOAT2(550.0f, 550.0f));
		}
		
		// キャラ描画
		D3D.Draw2D(m_character[i], XMFLOAT2((SCREEN_WIDTH * (1.0f / 7)) * (i + 1), 400.0f), XMFLOAT2(500.0f, 500.0f));
	}

	// プレイヤーの操作キャラクター
	for (int i = 0; i < 4; i++)
	{
		switch (m_splayer[i])
		{
		case SWITCH_PLAYER:
			// プレイヤーキャラ背景
			D3D.Draw2D(m_playerBg[0], XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)), 900.0f), XMFLOAT2(300.0f, 300.0f));
			if (m_padSelectflg[i])
			{
				// プレイヤーが選択したキャラクター
				D3D.Draw2D(m_charaicon[m_playerCharaNum[i]], XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)), 900.0f), XMFLOAT2(270.0f, 270.0f));
			}
			// プレイヤー切り替えボタン
			//D3D.Draw2D(m_changebutton[i], XMFLOAT2(CHANGEPOSX * i, CHANGEPOSY), XMFLOAT2(200.0f, 200.0f));
			D3D.Draw2D(m_playerBg[0], XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)) - 100.0f, 800.0f), XMFLOAT2(200.0f, 200.0f));
			break;
		case SWITCH_CPU:
			// プレイヤーキャラ背景
			D3D.Draw2D(m_playerBg[4], XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)), 900.0f), XMFLOAT2(300.0f, 300.0f));
			if (m_padSelectflg[i])
			{
				// CPUが選択したキャラクター
				D3D.Draw2D(m_charaicon[m_playerCharaNum[i]], XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)), 900.0f), XMFLOAT2(270.0f, 270.0f));
			}

			// プレイヤー切り替えボタン
			//D3D.Draw2D(m_changebutton[4], XMFLOAT2(CHANGEPOSX * i, CHANGEPOSY), XMFLOAT2(200.0f, 200.0f));
			D3D.Draw2D(m_playerBg[4], XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)) - 100.0f, 800.0f), XMFLOAT2(200.0f, 200.0f));

			break;
		case SWITCH_NULL:
			// プレイヤーキャラ背景
			D3D.Draw2D(m_playerBg[5], XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)), 900.0f), XMFLOAT2(300.0f, 300.0f));
			// 
			// プレイヤー切り替えボタン
			//D3D.Draw2D(m_changebutton[i], XMFLOAT2(CHANGEPOSX * i, CHANGEPOSY), XMFLOAT2(200.0f, 200.0f));
			D3D.Draw2D(m_playerBg[5], XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)) - 100.0f, 800.0f), XMFLOAT2(200.0f, 200.0f));
			break;
		}
	}

	


	// カーソル描画
	for (int i = 0; i < 4; i++)
	{
		if(m_splayer[i] == SWITCH_PLAYER)
		D3D.Draw2D(m_cursor[i], m_cursorPos[i], m_cursorSize);
	}
	
}




void CharacterSelect::CursorUpdate()
{
	bool cpuBeingControlled = false;
	for (int i = 0; i < 4; i++)
	{
		if (m_splayer[i] == SWITCH_NULL)
			continue;

		// プレイヤー状態がプレイヤーの場合
		if (m_splayer[i] == SWITCH_PLAYER)
		{

			float cursorVel_x = (float)CTRL.GetLeftStickHorizontal(i);
			float cursorVel_y = (float)CTRL.GetLeftStickVertical(i);

			m_cursorPos[i].x += cursorVel_x * 0.01;
			m_cursorPos[i].y += cursorVel_y * 0.01;

			// 外に飛び出さないようにする
			m_cursorPos[i].x = std::clamp(m_cursorPos[i].x, 25.0f, SCREEN_WIDTH - 25.0f);
			m_cursorPos[i].y = std::clamp(m_cursorPos[i].y, 25.0f, SCREEN_HEIGHT - 25.0f);


			// プレイヤー状態切り替え
			// 要修正！
			for (size_t j = 0; j < splayerAreas.size(); ++j) {
				const Area& sArea = splayerAreas[j];

				if (m_cursorPos[i].x >= sArea.x_min && m_cursorPos[i].x <= sArea.x_max &&
					m_cursorPos[i].y >= sArea.y_min && m_cursorPos[i].y <= sArea.y_max &&
					CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CIRCLE, i)) {

					switch (m_splayer[j + 1])
					{
					case SWITCH_PLAYER:
						m_splayer[j + 1] = SWITCH_CPU;
						m_totalCPU++;
						break;
					case SWITCH_CPU:
						m_splayer[j + 1] = SWITCH_NULL;
						m_selectflg[m_playerCharaNum[j + 1]] = false;
						m_padSelectflg[j + 1] = true;
						m_totalCPU--;
						m_totalPlayer--;
						break;
					case SWITCH_NULL:
						m_splayer[j + 1] = SWITCH_PLAYER;
						m_padSelectflg[j + 1] = false;
						m_totalPlayer++;
						break;
					}
				}
			}

			// 1PがCPUキャラを選択中ならcontinue
			if (i == 0 && m_CPURun)
			{
				continue;
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
					if (CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CIRCLE, i) && !m_padSelectflg[i]) {
						m_selectflg[j] = true;
						m_padSelectflg[i] = true;
						m_playerCharaNum[i] = j;
					}
				}
				else
				{
					m_iconflg[i][j] = false; // 範囲外ならフラグをリセット
				}
				
			}
			
			// ×ボタンが押されたらキャラクター選択フラグをfalseにする
			if (CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CROSS, i) && m_padSelectflg[m_playerCharaNum[i]]/* && m_splayer[i] == SWITCH_PLAYER*/)
			{
				m_selectflg[m_playerCharaNum[i]] = false;
				m_padSelectflg[i] = false;
				m_playerCharaNum[i] = 6;
			}
		}
		
		cpuBeingControlled = CPUSelect(i, cpuBeingControlled, CPUSearch());
		if (!g_isKeyReleased) return;
	}
}



bool CharacterSelect::CPUSelect(int playerNum, bool cpuBeingControlled, bool lastcpu)
{
	// プレイヤーの状態がCPUで1Pがキャラを選択していない場合
	if (!m_padSelectflg[0] || playerNum == 0)
	{
		return false;
	}
	if (lastcpu)
	{
		playerNum = m_lastCPU;
	}
	// プレイヤーの状態がCPUで1Pがキャラを選択している場合
	// 1PがCPUのキャラを選択できるようにする
	if (!m_padSelectflg[playerNum])
	{
		// 他のSWITCH_CPUが既に操作中ならスキップ
		if (cpuBeingControlled)
		{
			return true;
		}
		cpuBeingControlled = true;
		m_CPURun = true;

		// アイコンとカーソルが重なっていて〇ボタンが押されたときキャラ選択フラグをtrueにする
		for (size_t j = 0; j < iconAreas.size(); ++j)
		{
			const Area& area = iconAreas[j];

			// カーソルがアイコン範囲に入っているかチェック
			if (m_cursorPos[0].x >= area.x_min && m_cursorPos[0].x <= area.x_max &&
				m_cursorPos[0].y >= area.y_min && m_cursorPos[0].y <= area.y_max && !m_selectflg[j]) {
				// 重なっていたらフラグをtrueにする
				m_iconflg[0][j] = true;

				// ○ボタンが押されたらキャラクター選択フラグをtrueにする
				if (CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CIRCLE, 0) && !m_selectflg[j] && !m_padSelectflg[playerNum]) {
					m_selectflg[j] = true;
					m_padSelectflg[playerNum] = true;
					m_playerCharaNum[playerNum] = j;
					
				}
			}
			else
			{
				m_iconflg[0][j] = false; // 範囲外ならフラグをリセット
			}
		}
	}

	// ×ボタンが押されているかつ、前回クロスボタンでCPUを操作してから1フレームが経過していたらキャラクター選択フラグをfalseにする。
	if (CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CROSS, 0)&& g_isKeyReleased)
	{
		for (int i = 3; i >= 0; i--)
		{
			if (m_splayer[i] == SWITCH_CPU && m_padSelectflg[i])
			{
				m_selectflg[m_playerCharaNum[i]] = false;
				m_padSelectflg[i] = false;
				m_CPURun = false;
				break;
			}
			if (i == 0)
			{
				m_selectflg[m_playerCharaNum[0]] = false;
				m_padSelectflg[0] = false;
				m_CPURun = false;
				break;
			}
		}

		// キーをまだ離していないで。
		g_isKeyReleased = false;
	}

	// 下の処理いらんぜよ。
	//else if (CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CROSS, 0) && m_padSelectflg[playerNum] && g_isKeyReleased)
	//{
	//	m_selectflg[m_playerCharaNum[playerNum]] = false;
	//	m_padSelectflg[playerNum] = false;
	//	m_iconflg[playerNum][m_playerCharaNum[playerNum]] = false;
	//	m_CPURun = false;

	//	g_isKeyReleased = false;
	//}
	return cpuBeingControlled;
}


bool CharacterSelect::CPUSearch()
{
	int CPUCount = 0;
	for (int i = 1; i < 4; i++)
	{
		if (m_splayer[i] == SWITCH_CPU && m_padSelectflg[i])
		{
			CPUCount++;
			m_lastCPU = i;
		}
	}
	
	if (m_totalCPU == CPUCount)
	{
		return true;
	}

	return false;
}

