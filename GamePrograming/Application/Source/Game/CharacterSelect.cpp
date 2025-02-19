#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "Game/CharacterSelect.h"
#include "Game/Controller.h"
#include <algorithm>

// CPU?I??̍ۂ?1P?̃R???g???[???[?𓯂??t???[???Ŏg?????߁A?L?[?g???K?[???????v???X?Ɠ????????ɂȂ??Ă??܂??B
// ???̂??߁A???I???????PU?I??̏???????̃t???[???܂ł??Ȃ??悤?ɂ??????̃t???O?B
bool g_isKeyReleased = true;

CharacterSelect::CharacterSelect() {

	m_totalPlayer = 4;
	m_controlPlayer = CTRL.GetGamepadMax();
	// ?f?o?b?O?p
	//m_controlPlayer = 2;

	m_CPURun = false;
	m_totalCPU = 0;
	m_lastCPU = 0;

	// ?ڑ????Ă??????g???[????SWITCH_PLAYER
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

	if (m_totalCPU != 0)
		m_isCPU = true;
	else
		m_isCPU = false;
	

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
		//?R???g???[???[?擾
		m_padIndex[a] = CTRL.GetGamepadHandle();
	}

	// ?A?C?R???͈̔? 2??3
	iconAreas = {
	{(SCREEN_WIDTH * (1.0f / 7)) - 135.0f, (SCREEN_WIDTH * (1.0f / 7)) + 130.0f, 230.0f, 600.0f},
	{((SCREEN_WIDTH * (1.0f / 7)) * 2) - 135.0f, ((SCREEN_WIDTH * (1.0f / 7)) * 2) + 130.0f, 230.0f, 600.0f},
	{((SCREEN_WIDTH * (1.0f / 7)) * 3) - 135.0f, ((SCREEN_WIDTH * (1.0f / 7)) * 3) + 130.0f, 230.0f, 600.0f},
	{((SCREEN_WIDTH * (1.0f / 7)) * 4) - 135.0f, ((SCREEN_WIDTH * (1.0f / 7)) * 4) + 130.0f, 230.0f, 600.0f},
	{((SCREEN_WIDTH * (1.0f / 7)) * 5) - 135.0f, ((SCREEN_WIDTH * (1.0f / 7)) * 5) + 130.0f, 230.0f, 600.0f},
	{((SCREEN_WIDTH * (1.0f / 7)) * 6) - 135.0f, ((SCREEN_WIDTH * (1.0f / 7)) * 6) + 130.0f, 230.0f, 600.0f},
	};

	// ?v???C???[???؂????{?^???͈̔? 16??9
	splayerAreas =
	{
		{ (SCREEN_WIDTH * (1.0f / 5) * 2) - 188.0f, (SCREEN_WIDTH * (1.0f / 5) * 2) + 15.0f, 735.0f, 865.0f },	// 2P
		{ (SCREEN_WIDTH * (1.0f / 5) * 3) - 188.0f, (SCREEN_WIDTH * (1.0f / 5) * 3) + 15.0f, 735.0f, 865.0f },	// 3P
		{ (SCREEN_WIDTH * (1.0f / 5) * 4) - 188.0f, (SCREEN_WIDTH * (1.0f / 5) * 4) + 15.0f, 735.0f, 865.0f }	// 4P
	};

	m_cursorPos[0] = XMFLOAT2(1100.0f, 850.0f);
	m_cursorPos[1] = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	m_cursorPos[2] = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	m_cursorPos[3] = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	m_cursorSize = XMFLOAT2(150.0f, 150.0f);

	
	
	

	// ?J?[?\???e?N?X?`??
	m_cursor[0].Load(L"Data/Texture/hand1.png");
	m_cursor[1].Load(L"Data/Texture/hand2.png");
	m_cursor[2].Load(L"Data/Texture/hand3.png");
	m_cursor[3].Load(L"Data/Texture/hand4.png");

	// ?L?????N?^?[?e?N?X?`??
	m_character[0].Load(L"Data/Texture/ikemenicon.png");
	m_character[1].Load(L"Data/Texture/nekketsuicon.png");
	m_character[2].Load(L"Data/Texture/foxicon.png");
	m_character[3].Load(L"Data/Texture/womanicon.png");
	m_character[4].Load(L"Data/Texture/bisyoujoicon.png");
	m_character[5].Load(L"Data/Texture/chasarinicon.png");

	// ?L?????N?^?[?A?C?R???e?N?X?`??
	m_charaicon[0].Load(L"Data/Texture/ikemenicon1.png");
	m_charaicon[1].Load(L"Data/Texture/nekketsuicon1.png");
	m_charaicon[2].Load(L"Data/Texture/foxicon1.png");
	m_charaicon[3].Load(L"Data/Texture/womanicon1.png");
	m_charaicon[4].Load(L"Data/Texture/bisyoujoicon1.png");
	m_charaicon[5].Load(L"Data/Texture/chasarinicon1.png");

	// ?v???C???[?e?N?X?`???w?i
	m_playerBg[0].Load(L"Data/Texture/player1Bg.png");
	m_playerBg[1].Load(L"Data/Texture/player2Bg.png");
	m_playerBg[2].Load(L"Data/Texture/player3Bg.png");
	m_playerBg[3].Load(L"Data/Texture/player4Bg.png");
	m_playerBg[4].Load(L"Data/Texture/player5Bg.png");
	m_playerBg[5].Load(L"Data/Texture/player6Bg.png");

	// ?v???C???[???؂????{?^??
	m_changebutton[0].Load(L"Data/Texture/button1.png");		//1P
	m_changebutton[1].Load(L"Data/Texture/button2.png");		//2P
	m_changebutton[2].Load(L"Data/Texture/button3.png");		//3P
	m_changebutton[3].Load(L"Data/Texture/button4.png");		//4P
	m_changebutton[4].Load(L"Data/Texture/button5.png");		//CPU
	m_changebutton[5].Load(L"Data/Texture/button6.png");		//?Ȃ?

	m_heading.Load(L"Data/Texture/CharacterSelect.png");

	m_backGroundTex.Load(L"Data/Texture/CharacterSelectBg.png");
}

CharacterSelect::~CharacterSelect()
{
	
	
	// ?????????N?^?[?ԍ???Z?b?g
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

	// ?v???C???[?f?[?^?ۑ?
	for (int i = 0; i < 4; i++)
	{
		if (m_splayer[i] == SWITCH_PLAYER)
			SaveData::SetPlayerData(PlayerData{ *(std::next(m_CharacNum.begin(), i)), i ,true,i + 1 });
		else if (m_splayer[i] == SWITCH_CPU)
			SaveData::SetPlayerData(PlayerData{ *(std::next(m_CharacNum.begin(), i)), -1 ,false,i + 1 });

	}

	SaveData::SetTotalPlayer(m_totalPlayer);
	SaveData::SetControlPlayer(m_controlPlayer);
	// ?R???g???[???̉?
	for (int i = 0; i < 4; i++)
	{
		CTRL.ReleaseGamepadHandle(m_padIndex[i]);
	}
}

void CharacterSelect::Update() {

	/****************************************
	* 1/17 ?S?? ?J???}?^?g?E
	****************************************/
	bool cpuBeingControlled = false;
	bool lastCPU = LastCPUSearch();

	if (m_totalCPU != 0)
		m_isCPU = true;
	else
		m_isCPU = false;

	// ?J?[?\???X?V
	MoveCursor();
	PlayerCursorUpdate();
	// CPU?????????V
	if(m_isCPU)
	{
		if (!cpuBeingControlled)
		{
			cpuBeingControlled = CPUCursorUpdate(cpuBeingControlled, lastCPU);
		}
	}

	/*********************************************/

	//?Ƃ肠?????G???^?[?L?[???????????
	for(int i = 0; i < 4;i++)
	{
		if (CTRL.GetKeyboardTrigger(DIK_RETURN) || (m_padSelectflg[0] && m_padSelectflg[1] && m_padSelectflg[2] &&
			m_padSelectflg[3]) && CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_TRIANGLE, i)) {
			m_isFinished = true;
		}
	}


	g_isKeyReleased = true;
}

void CharacterSelect::Draw() {
	//?w?i?`??
	D3D.Draw2D(m_backGroundTex, XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));

	/****************************************
	* 1/17 ?S?? ?J???}?^?g?E
	****************************************/
	D3D.Draw2D(m_heading, XMFLOAT2(SCREEN_WIDTH / 2,100.0f ), XMFLOAT2(800.0f, 150.0f));
	

	for (int i = 0; i < 6; i++)
	{
		// ?L?????w?i?`??
		if(!m_selectflg[i] && m_iconflg[0][i] || m_iconflg[1][i] || m_iconflg[2][i] || m_iconflg[3][i])
		{
			D3D.Draw2D(m_character[i], XMFLOAT2((SCREEN_WIDTH * (1.0f / 7)) * (i + 1), 400.0f), XMFLOAT2(550.0f, 550.0f));
		}
		
		// ?L?????`??
		D3D.Draw2D(m_character[i], XMFLOAT2((SCREEN_WIDTH * (1.0f / 7)) * (i + 1), 400.0f), XMFLOAT2(500.0f, 500.0f));
	}

	// ?v???C???[?̑????????N?^?[
	for (int i = 0; i < 4; i++)
	{
		switch (m_splayer[i])
		{
		case SWITCH_PLAYER:
			// ?v???C???[?L?????w?i
			D3D.Draw2D(m_playerBg[i], XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)), 900.0f), XMFLOAT2(300.0f, 300.0f));
			if (m_padSelectflg[i])
			{
				// ?v???C???[???I??????L?????N?^?[
				D3D.Draw2D(m_charaicon[m_playerCharaNum[i]], XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)), 900.0f), XMFLOAT2(270.0f, 270.0f));
			}
			// ?v???C???[?؂????{?^??
			D3D.Draw2D(m_changebutton[i], XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)) - 100.0f, 800.0f), XMFLOAT2(160.0f, 90.0f));
			break;
		case SWITCH_CPU:
			// ?v???C???[?L?????w?i
			D3D.Draw2D(m_playerBg[4], XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)), 900.0f), XMFLOAT2(300.0f, 300.0f));
			if (m_padSelectflg[i])
			{
				// CPU???I??????L?????N?^?[
				D3D.Draw2D(m_charaicon[m_playerCharaNum[i]], XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)), 900.0f), XMFLOAT2(270.0f, 270.0f));
			}

			// ?v???C???[?؂????{?^??
			D3D.Draw2D(m_changebutton[4], XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)) - 100.0f, 800.0f), XMFLOAT2(160.0f, 90.0f));

			break;
		case SWITCH_NULL:
			// ?v???C???[?L?????w?i
			D3D.Draw2D(m_playerBg[5], XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)), 900.0f), XMFLOAT2(300.0f, 300.0f));
			// ?v???C???[?؂????{?^??
			D3D.Draw2D(m_changebutton[5], XMFLOAT2(SCREEN_WIDTH * (1.0f / 5 * (i + 1)) - 100.0f, 800.0f), XMFLOAT2(160.0f, 90.0f));
			break;
		}
	}

	// ?J?[?\???`??
	for (int i = 0; i < 4; i++)
	{
		if(m_splayer[i] == SWITCH_PLAYER)
		D3D.Draw2D(m_cursor[i], m_cursorPos[i], m_cursorSize);
	}
	
	// ?S?Ẵv???C???[???L?????I???I???????Ƃ??ɕ`??
	if ((m_padSelectflg[0] && m_padSelectflg[1] && m_padSelectflg[2] && m_padSelectflg[3]))
	{
		
	}

}

void CharacterSelect::MoveCursor()
{
	for (int i = 0; i < 4; i++)
	{
		if (m_splayer[i] != SWITCH_PLAYER)
			continue;

		float cursorVel_x = (float)CTRL.GetLeftStickHorizontal(i);
		float cursorVel_y = (float)CTRL.GetLeftStickVertical(i);

		m_cursorPos[i].x += cursorVel_x * 0.03f;
		m_cursorPos[i].y += cursorVel_y * 0.03f;

		// ?O?ɔ??o???Ȃ??悤?ɂ???
		m_cursorPos[i].x = std::clamp(m_cursorPos[i].x, 65.0f, SCREEN_WIDTH - 65.0f);
		m_cursorPos[i].y = std::clamp(m_cursorPos[i].y, 65.0f, SCREEN_HEIGHT - 65.0f);
	}
}

void CharacterSelect::PlayerCursorUpdate()
{
	
	for (int i = 0; i < 4; i++) {

		

		if (m_splayer[i] != SWITCH_PLAYER)
			continue; // ?v???C???[?̂ݏ???

		// ?v???C???[?̏??؂???
		SwitchPlayerState(i);

		// 1P??CPU?L?????I?𒆂Ȃ珈?????Ȃ?
		if(i == 0 && m_CPURun)
			continue;

		// ?A?C?R???ƃJ?[?\???̏Փ˃`?F?b?N
		for (size_t j = 0; j < iconAreas.size(); ++j) 
		{
			const Area& Areas = iconAreas[j];
			if (IsCursorOverIcon(m_cursorPos[i], Areas) && !m_selectflg[j]) {
				m_iconflg[i][j] = true;

				if (CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CIRCLE, i) && !m_padSelectflg[i]) {
					m_selectflg[j] = true;
					m_padSelectflg[i] = true;
					m_playerCharaNum[i] = static_cast<int>(j);
				}
			}
			else {
				m_iconflg[i][j] = false;
			}
		}

		// ?L?????Z??????
		if (CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CROSS, i) && m_padSelectflg[i]) 
		{
			ResetSelection(i);
		}
	}

}

bool CharacterSelect::CPUCursorUpdate(bool cpuBeingControlled, bool lastCPU)
{	
	if (!m_padSelectflg[0])
	{
		return false;
	}

	for (int i = 1; i < 4; i++) 
	{
		if(m_splayer[i] != SWITCH_CPU)
			continue;
		if(!lastCPU)
		{
			if (m_padSelectflg[i])
				continue;
		}
		
		if (cpuBeingControlled)
			continue;

		m_CPURun = true;
		cpuBeingControlled = true;

		if (SelectCPUCharacter(i)) 
		{
			m_CPURun = false;
			break;
		}
	}

	if (CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CROSS, 0) && g_isKeyReleased) 
	{
		CancelCPUSelection();
		g_isKeyReleased = false;
	}
	
	return cpuBeingControlled;
}

// ?v???C???[?̃L?????폜
void CharacterSelect::ResetSelection(int player)
{
	m_selectflg[m_playerCharaNum[player]] = false;
	m_padSelectflg[player] = false;
	m_playerCharaNum[player] = 6;
}

// CPU?̃L?????I??
bool CharacterSelect::SelectCPUCharacter(int playerNum)
{
	for (size_t j = 0; j < iconAreas.size(); ++j) {
		const Area& Areas = iconAreas[j];
		if (IsCursorOverIcon(m_cursorPos[0], Areas) && !m_selectflg[j]) {
			m_iconflg[0][j] = true;

			if (CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CIRCLE, 0) && !m_padSelectflg[playerNum]) {
				m_selectflg[j] = true;
				m_padSelectflg[playerNum] = true;
				m_playerCharaNum[playerNum] = static_cast<int>(j);
				return true;
			}
		}
		else {
			m_iconflg[0][j] = false;
		}
	}
	return false;
}

// CPU?̃L?????폜
void CharacterSelect::CancelCPUSelection()
{
	for (int i = 3; i >= 0; i--)
	{
		if (m_splayer[i] == SWITCH_CPU && m_padSelectflg[i])
		{
			ResetSelection(i);
			m_CPURun = false;
			break;
		}
		if (i == 0)
		{
			ResetSelection(i);
			m_CPURun = false;
			break;
		}
	}
}

// ?J?[?\???̏Փ˔???
bool CharacterSelect::IsCursorOverIcon(XMFLOAT2 cursorPos, const Area& area)
{
	if (cursorPos.x >= area.x_min && cursorPos.x <= area.x_max &&
		cursorPos.y >= area.y_min && cursorPos.y <= area.y_max)
	{
		return true;
	}
	return false;
}

// ?v???C???[?X?e?[?g?؂???
void CharacterSelect::SwitchPlayerState(int i)
{
	
	for (size_t j = 0; j < splayerAreas.size(); ++j) 
	{
		const Area& sArea = splayerAreas[j];
		
		if (IsCursorOverIcon(m_cursorPos[i], sArea) && CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CIRCLE, i))
		{
			bool debug = false;
			if (m_padSelectflg[0])
			{
				debug = m_padSelectflg[0];
			}
			switch (m_splayer[j + 1])
			{
			case SWITCH_PLAYER:
				m_splayer[j + 1] = SWITCH_CPU;
				m_totalCPU++;
				m_selectflg[m_playerCharaNum[j + 1]] = false;
				m_padSelectflg[j + 1] = false;
				m_playerCharaNum[j + 1] = 6;

				break;
			case SWITCH_CPU:
				m_splayer[j + 1] = SWITCH_NULL;
				m_selectflg[m_playerCharaNum[j + 1]] = false;
				m_padSelectflg[j + 1] = true;
				m_playerCharaNum[j + 1] = 6;
				m_CPURun = false;
				m_totalCPU--;
				m_totalPlayer--;
				break;
			case SWITCH_NULL:
				m_splayer[j + 1] = SWITCH_PLAYER;
				m_padSelectflg[j + 1] = false;
				m_totalPlayer++;
				break;
			}
			if (debug)
			{
				m_padSelectflg[0] = debug;
			}
		}
	}
}

bool CharacterSelect::LastCPUSearch()
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
	
	if (m_totalCPU != 0 && m_totalCPU == CPUCount)
	{
		return true;
	}

	return false;
}
