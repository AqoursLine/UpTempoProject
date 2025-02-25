/******************************************************
* CharacterSelect.h		キャラクター選択
* 制作者：ミヤタジョウジ　
* 作成日：2025/1/16
* 最終更新日：2025/1/17
*******************************************************/
#pragma once
#include "Game/Select.h"
#include <list>
#include "Game/SaveData.h"

enum PLAYERSTATE
{
	SWITCH_PLAYER,
	SWITCH_CPU,
	SWITCH_NULL,
};

enum SELECTSTATE
{
	CHARASELECT_STATE,
	ANIMATION_STATE,
	RETURNANIMATION_STATE,
	FINISH_STATE,
};

struct Area {
	float x_min, x_max;
	float y_min, y_max;
};

class CharacterSelect : public Select {
public:
	CharacterSelect();
	~CharacterSelect();

	void Update() override;
	void Draw() override;

	void MoveCursor();
	void PlayerCursorUpdate();
	bool CPUCursorUpdate(bool cpuBeingControlled, bool lastcpu);

	void ResetSelection(int player);
	bool SelectCPUCharacter(int playerNum);
	void CancelCPUSelection();
	
	bool IsCursorOverIcon(XMFLOAT2 cursorPos, const Area& area);	// カーソルの衝突判定
	void SwitchPlayerState(int i);
	bool LastCPUSearch();
	
private:
	int m_totalPlayer;		// 総プレイヤー人数
	int m_controlPlayer;	// 操作プレイヤー人数
	std::list<CHARACTOR> m_CharacNum;  // プレイヤー毎の操作キャラクター番号

	int m_padIndex[4];						// コントローラの識別番号
	int m_playerCharaNum[4] = { 6,6,6,6 };	// 
	int m_totalCPU;							// 総CPU
	int m_lastCPU;
	bool m_CPURun;
	bool m_isCPU;


	bool m_iconflg[4][6];					// アイコンフラグ
	bool m_selectflg[6];					// キャラ選択フラグ
	bool m_padSelectflg[4];					// プレイヤー選択フラグ

	PLAYERSTATE m_splayer[4];				// プレイヤー状態

	SELECTSTATE m_charaSelState;			// ステート管理

	std::vector<Area> iconAreas;
	std::vector<Area> splayerAreas;

	Texture m_cursor[4];
	XMFLOAT2 m_cursorPos[4];
	XMFLOAT2 m_cursorSize;

	Texture m_playerBg[6];
	Texture m_character[7];
	Texture m_charaicon[6];
	Texture m_heading;
	Texture m_changebutton[6];
};



