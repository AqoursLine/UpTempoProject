#pragma once

enum STAGE
{
	STAGE_CLASSROOM,//名前はしらん:<
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
};

class SaveData
{
public:
	SaveData();
	~SaveData();

	//セッター
	static void SetTotalPlayer(int maxnum) { m_totalPlayer = maxnum; }
	static void SetControlPlayer(int cnum) { m_controlPlayer = cnum; }
	static void SetWinPlayer(int wplayer) { m_winPlayer = wplayer; }
	static void SetStage(STAGE stage) {m_stageNum = stage;}
	static void SetPlayerData(PlayerData pData) {m_playerData.push_back(pData);}
	static void SetPlayerRank(int playerNum) { m_playerRank.push_back(playerNum); }
	
	//ゲッター		//値いじりたい場合は参照型にするかも
	static int GetTotalPlayer(void) { return m_totalPlayer; }//コピーをreturn
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

	static int m_totalPlayer;					//総プレイヤー数
	static int m_controlPlayer;					//操作プレイヤー数
	static int m_winPlayer;						//勝利したプレイヤー
	static STAGE m_stageNum;					//ステージ番号
	static std::list<PlayerData> m_playerData;	//プレイヤーデータ
	static std::vector<int> m_playerRank;		//プレイヤー順位

};

//.hだけでもよかったかも

