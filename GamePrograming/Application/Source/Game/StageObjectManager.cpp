/******************************************************
* StageObjectManager.cpp
* 制作者：murayama
* 作成日：2025/02/15
* 最終更新日：
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"
#include "Game/StageObjectManager.h"

#include "Game/FerrisWheel.h"
#include "Game/Gondola.h"
#include "Game/Horse.h"
#include "Game/Lamp.h"
#include "Game/Moon.h"
#include "Game/Board.h"
#include "Game/Cloud.h"
#include "Game/Grass.h"

#include "Game/EffectManager.h"

std::vector<StageObjectData> StageObjectManager::m_stageObjectData;

StageObjectManager::StageObjectManager()
{
	m_repopCnt = nullptr;
	m_standby = nullptr;
}

StageObjectManager::~StageObjectManager()
{
	for (int i = 0; i < m_ObjectMax; i++)
	{
		if (m_stageObjects[i])
		{
			m_stageObjects[i]->Finalize();
			delete m_stageObjects[i];
		}

	}

	if (m_stageObjects) delete[] m_stageObjects;
	m_stageObjectData.clear();
	if (m_repopCnt) delete[] m_repopCnt;
	if (m_standby) delete[] m_standby;
}

void StageObjectManager::AddStageObject(STAGEOBJECT_ID id, float x, float y, float r, int repopTime, int m_spare)
{
	m_stageObjectData.push_back(StageObjectData(id, x, y, r, repopTime, m_spare));
}


void StageObjectManager::Initialize()
{
	//配列作成
	m_ObjectMax = static_cast<int>(m_stageObjectData.size());//毎回関数呼ぶよりよさげ
	m_stageObjects = new ThrowObject* [m_ObjectMax];
	m_repopCnt = new int[m_ObjectMax];
	m_standby = new bool[m_ObjectMax];

	//データをもとにオブジェクトの生成
	for (int i = 0; i < m_ObjectMax; i++)
	{
		CreateObject(i);
	}
}

void StageObjectManager::PhysicsUpdate() {
	for (int i = 0; i < m_ObjectMax; i++) {
		if (m_stageObjects[i]) {
			m_stageObjects[i]->PhysicsUpdate();
		}
	}
}

void StageObjectManager::Update()
{
	//ステージで追加し終わった時にイニシャライズを呼ぶようにする
	//if (firstFrame)
	//{
	//	StageObjectManager::Initialize();
	//	firstFrame = false;
	//}

	for (int i = 0; i < m_ObjectMax; i++)
	{
		if (m_stageObjects[i])
		{
			m_stageObjects[i]->Update();

			if (m_stageObjects[i]->GetIsDelete())//けすよー
			{
				// ヒットエフェクト
				EffectManager::CreateEffect(ObjectHitOther, m_stageObjects[i]->GetPos(), XMFLOAT2(600.0f, 600.0f), 0.0f);
				m_stageObjects[i]->Finalize();
				delete m_stageObjects[i];
				m_stageObjects[i] = nullptr;
			}
		}
		else//消えてる場合の処理
		{

			if (m_repopCnt[i] >= m_stageObjectData[i].m_repopTime - 70 && m_standby[i] == false)
			{
				EffectManager::CreateEffect(SpawnEffect, XMFLOAT2(m_stageObjectData[i].m_x, m_stageObjectData[i].m_y),
					XMFLOAT2(300, 300), 0, 60);
				m_standby[i] = true;
			}

			if (m_repopCnt[i] >= m_stageObjectData[i].m_repopTime)
			{
				CreateObject(i);

				// モノ出現エフェクトを発生
				EffectManager::CreateEffect(ThingsSpawn, XMFLOAT2(m_stageObjectData[i].m_x, m_stageObjectData[i].m_y), XMFLOAT2(400.0f, 400.0f), 0.0f);
			}

			else
			{
				m_repopCnt[i]++;
			}
		}

	}

}

void StageObjectManager::Draw()
{
	for (int i = 0; i < m_ObjectMax; i++)
	{
		if (m_stageObjects[i])
		{
			m_stageObjects[i]->Draw();
		}
	}
}

int StageObjectManager::GetObjectNum(const std::string& type)
{
	int num = 0;

	for (int i = 0; i < m_ObjectMax; i++) {

		if (m_stageObjects[i]) {

			if (m_stageObjects[i]->CompareType(type) && m_stageObjects[i]->CompareTag(type)) {
				num++;
			}
		}
		
	}

	return num;
}

void StageObjectManager::CreateObject(int objNum) {
	switch (m_stageObjectData[objNum].m_objID) {//この処理関数化すべきかも
		case S_HORSE_FRONT:
			m_stageObjects[objNum] = (new Horse(m_stageObjectData[objNum].m_x, m_stageObjectData[objNum].m_y,
				m_stageObjectData[objNum].m_r, true));
			break;
		case S_HORSE_BACK:
			m_stageObjects[objNum] = (new Horse(m_stageObjectData[objNum].m_x, m_stageObjectData[objNum].m_y,
				m_stageObjectData[objNum].m_r, false));
			break;

		case S_FERRISWHEEL:
			m_stageObjects[objNum] = (new FerrisWheel(m_stageObjectData[objNum].m_x, m_stageObjectData[objNum].m_y,
				m_stageObjectData[objNum].m_r));
			break;
		case S_GONDOLA:
			m_stageObjects[objNum] = (new Gondola(m_stageObjectData[objNum].m_x, m_stageObjectData[objNum].m_y,
				m_stageObjectData[objNum].m_r, m_stageObjectData[objNum].m_spare, m_stageObjects[0]));
			break;
		case S_LAMP_LEFT:
			m_stageObjects[objNum] = (new Lamp(m_stageObjectData[objNum].m_x, m_stageObjectData[objNum].m_y,
				m_stageObjectData[objNum].m_r, true));
			break;
		case S_LAMP_RIGHT:
			m_stageObjects[objNum] = (new Lamp(m_stageObjectData[objNum].m_x, m_stageObjectData[objNum].m_y,
				m_stageObjectData[objNum].m_r, false));
			break;
		case S_MOON:
			m_stageObjects[objNum] = (new Moon(m_stageObjectData[objNum].m_x, m_stageObjectData[objNum].m_y,
				m_stageObjectData[objNum].m_r));
			break;
		case S_BOARD:
			m_stageObjects[objNum] = (new Board(m_stageObjectData[objNum].m_x, m_stageObjectData[objNum].m_y,
				m_stageObjectData[objNum].m_r));
			break;
		case S_CLOUD:
			m_stageObjects[objNum] = (new Cloud(m_stageObjectData[objNum].m_x, m_stageObjectData[objNum].m_y,
				m_stageObjectData[objNum].m_r));
			break;
		case S_GRASS:
			m_stageObjects[objNum] = (new Grass(m_stageObjectData[objNum].m_x, m_stageObjectData[objNum].m_y,
				m_stageObjectData[objNum].m_r));
			break;
	}

	m_repopCnt[objNum] = 0;
	m_standby[objNum] = false;

}
