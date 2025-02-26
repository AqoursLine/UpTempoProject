/******************************************************
* Player.cpp		プレイヤーまねーじゃー
* 制作者：ミヤタジョウジ?
* 作成日：2024/11/05 ?
* 最終更新日：2024/11/05 ?
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "Game/Controller.h"

#include "Game/PlayerManager.h"
#include "Game/SaveData.h"
#include "Phase.h"
#include "Game/EnemyCpu.h"

std::list<Player*> PlayerManager::m_players;

PlayerManager::PlayerManager(int phase)
{

	int playerMax = SaveData::GetTotalPlayer();
	for (int i = 0; i < playerMax; i++) {
		//生成する場所どこで設定する？
		//どうせフェーズごとにpos設定するんだからコンストラクタでのpos指定は仮と考えたい

		CreatePlayer(XMFLOAT2(100.0f + 200.0f * i, 300.0f), i + 1);//ちょっとずつずらして生成
	}
}


//プレイヤーの削除//プレイヤーの情報が消えていいタイミングに
PlayerManager::~PlayerManager()
{
	for (auto player : m_players) {
		delete player;
	}
	m_players.clear();
}


void PlayerManager::Update()
{
	float left = 20000;		//それぞれ端にいるプレイヤーの位置
	float right = -20000;   //初期値をありえない数値にして最初に絶対プレイヤーの座標が入るように
	float top = -10000;
	float bottom = 10000;

	XMFLOAT2 center;//中心座標
	XMFLOAT2 buf;

	for (auto player : m_players) {
		//プレイヤー更新
		player->Update();

		buf = player->GetPos();

		if (left > buf.x)
			left = buf.x;

		if (right < buf.x)
			right = buf.x;

		if (top < buf.y)
			top = buf.y;

		if (bottom > buf.y)
			bottom = buf.y;
	}

	//ぎりぎりにならないよう調整
	left -= 100;
	right += 100;
	top -= 100;
	bottom += 100;

	float width = right-left;
	float hight = bottom-top;

	center = XMFLOAT2(left + ((right - left) / 2.0f), top + ((bottom - top) / 2.0f));

	//残機がなくなったプレイヤーを削除
	for (auto itr = m_players.begin(); itr != m_players.end();) {
		if ((*itr)->GetIsDelete()) {
			Player* player = (*itr);
			itr = m_players.erase(itr);

			SaveData::SetPlayerRank(player->GetPlayerNum());
			for (auto player2 : m_players) {
				if (player == player2->GetTarget())
				{
					player2->SetNullTarget();
				}
			}

			delete player;
		} else {
			++itr;
		}
	}

	//最後の1人になったか
	if (m_players.size() <= 1) {
		Phase::ChangeState(PHASESTATE_FINISH);
		SaveData::SetPlayerRank(m_players.back()->GetPlayerNum());
	}
}

void PlayerManager::Draw()
{
	for (auto player : m_players) {
		player->Draw();
	}
}

void PlayerManager::CreatePlayer(XMFLOAT2 pos,int pnum)
{
	
	m_players.push_back(new Player(pos, pnum));
	
}
