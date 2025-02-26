#pragma once


#include "Game/player.h"


class EnemyCpu : public Player
{
public:
	EnemyCpu(XMFLOAT2 startpos, int pnum);
	~EnemyCpu();

	void Update() override;
	void OnCollisionEnter(GameObject* collision) override;

	// センサーのAABBを取得してクエリ
	void CheckFixturesInSensor(b2Fixture* sensor);


private:
	int m_moveDir;
	int m_frameCnt;

	int m_enterCnt = -1;
	int m_enterCoolTime = 5;
	bool m_jumpFlag = false;
	int m_jumpCoolCnt = 0;

	int ReversKey = 0;

	int m_targetRestCnt = 0;
	ThrowObject* m_targetObject = nullptr;
	
	b2Body* m_senBody = nullptr;

};

class QueryCallback : public b2QueryCallback {
public:
	std::vector<b2Fixture*> foundFixtures;

	bool ReportFixture(b2Fixture* fixture) override {
		foundFixtures.push_back(fixture);
		return true;  // trueを返すと、クエリは続行されます。
	}

};

