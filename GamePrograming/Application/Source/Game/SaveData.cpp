#include "SaveData.h"

//static‚Ì‚â‚Â
int SaveData::m_totalPlayer;
int SaveData::m_controlPlayer;
int SaveData::m_winPlayer;
STAGE SaveData::m_stageNum;
std::list<PlayerData> SaveData::m_playerData;

SaveData::SaveData()
{
	//‰Šú’l‚Í‚µ‚ç‚ñ
	m_totalPlayer = 0;
	m_controlPlayer = 0;
	m_winPlayer = 0;
	m_stageNum = STAGE_CLASSROOM;

	m_playerData.clear();

	
}

SaveData::~SaveData()
{
}



