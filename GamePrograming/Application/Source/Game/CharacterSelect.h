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

enum SWITCHPLAYER
{
	SWITCH_PLAYER,
	SWITCH_CPU,
	SWITCH_NULL,
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

	void CursorUpdate();
	
	

private:
	int m_totalPlayer;		// 総プレイヤー人数
	int m_controlPlayer;	// 操作プレイヤー人数
	std::list<CHARACTOR> m_CharacNum;  // プレイヤー毎の操作キャラクター番号

	int m_playerCharaNum[4] = { 0,0,0,0 };

	bool m_iconflg[4][6];					// アイコンフラグ
	bool m_selectflg[6];					// キャラ選択フラグ
	bool m_padSelectflg[4];					// プレイヤー選択フラグ
	
	SWITCHPLAYER m_splayer[4];

	std::vector<Area> iconAreas;
	std::vector<Area> splayerAreas;

	Texture m_cursor[4];
	XMFLOAT2 m_cursorPos[4];
	XMFLOAT2 m_cursorSize;

	
	Texture m_character[6];
	
};

