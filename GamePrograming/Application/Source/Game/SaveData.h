�ｿ#pragma once

enum STAGE
{
	STAGE_CLASSROOM,//蜷榊燕縺ｯ縺励ｉ繧:<
	STAGE_OCEAN,
	STAGE_GAME,
	STAGE_PARK,
};

enum CHARACTOR
{
	CHARACTOR_01 = 1,
	CHARACTOR_02,
	CHARACTOR_03,
	CHARACTOR_04,
	CHARACTOR_05,
	CHARACTOR_06,
};

struct PlayerData
{
	CHARACTOR charactorNum;	//キャラクター番号
	int PadNum;				//コントローラー番号
	bool Isplayer;			//プレイヤー(true)かCPU(false)
	int PlayerNum;			//プレイヤー番号
};

class SaveData
{
public:
	SaveData();
	~SaveData();

	//繧ｻ繝�ち繝ｼ
	static void SetTotalPlayer(int maxnum) { m_totalPlayer = maxnum; }
	static void SetControlPlayer(int cnum) { m_controlPlayer = cnum; }
	static void SetWinPlayer(int wplayer) { m_winPlayer = wplayer; }
	static void SetStage(STAGE stage) {m_stageNum = stage;}
	static void SetPlayerData(PlayerData pData) {m_playerData.push_back(pData);}
	static void SetPlayerRank(int playerNum) { m_playerRank.push_back(playerNum); }
	
	//繧ｲ繝�ち繝ｼ		//蛟､縺�§繧翫◆縺�ｴ蜷医�蜿ら�蝙九↓縺吶ｋ縺九ｂ
	static int GetTotalPlayer(void) { return m_totalPlayer; }//繧ｳ繝斐�繧池eturn
	static int GetControlPlayer(void) { return m_controlPlayer; }
	static int GetWinPlayer(void) { return m_winPlayer; }
	static STAGE GetStageNum(void) { return m_stageNum; }

	static PlayerData GetPlayerData(int pnum)
	{
		int count = 1;
		for (auto pData : m_playerData) {
			if (count == pnum)
				return pData;

			count++;
		}

		return PlayerData();
	}

	static int GetPlayerRank() {
		if (m_playerRank.empty()) {
			return -1;
		}

		int ret = m_playerRank.back();
		m_playerRank.pop_back();

		return ret;
	}
		
private:

	static int m_totalPlayer;					//邱上�繝ｬ繧､繝､繝ｼ謨ｰ
	static int m_controlPlayer;					//謫堺ｽ懊�繝ｬ繧､繝､繝ｼ謨ｰ
	static int m_winPlayer;						//蜍晏茜縺励◆繝励Ξ繧､繝､繝ｼ
	static STAGE m_stageNum;					//繧ｹ繝��繧ｸ逡ｪ蜿ｷ
	static std::list<PlayerData> m_playerData;	//繝励Ξ繧､繝､繝ｼ繝��繧ｿ
	static std::vector<int> m_playerRank;		//繝励Ξ繧､繝､繝ｼ鬆�ｽ

};

//.h縺縺代〒繧ゅｈ縺九▲縺溘°繧


