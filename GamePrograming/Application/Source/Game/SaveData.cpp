#include "framework.h"
#include "SaveData.h"

//static‚Ì‚â‚Â
int SaveData::m_totalPlayer;
int SaveData::m_controlPlayer;
int SaveData::m_winPlayer;
STAGE SaveData::m_stageNum;
std::list<PlayerData> SaveData::m_playerData;
std::vector<int> SaveData::m_playerRank;

SaveData::SaveData() {
	//‰Šú’l‚Í‚µ‚ç‚ñ
	m_totalPlayer = 0;
	m_controlPlayer = 0;
	m_winPlayer = 0;
	m_stageNum = STAGE_CLASSROOM;

	m_playerData.clear();
	m_playerRank.clear();

	
	std::list<CHARACTOR> tmp;

	SetPlayerData(PlayerData{*(std::next(tmp.begin(), 1)), 1});
}

SaveData::~SaveData()
{
}



