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

#include "Game/EffectManager.h"

std::vector<StageObject> StageObjectManager::m_stageObjectData;

StageObjectManager::StageObjectManager()
{
	m_repopCnt = nullptr;
	m_standby = nullptr;
}

StageObjectManager::~StageObjectManager()
{
	for (int i = 0; i < m_stageObjectData.size(); i++)
	{
		if (m_stageObjects[i])
		{
			m_stageObjects[i]->Finalize();
			delete m_stageObjects[i];
		}

	}

	delete[] m_stageObjects;
	m_stageObjectData.clear();
	delete[] m_repopCnt;
	delete[] m_standby;
}

void StageObjectManager::AddStageObject(STAGEOBJECT_ID id, float x, float y, float r, int repopTime, int m_spare)
{
	m_stageObjectData.push_back(StageObject(id, x, y, r, repopTime, m_spare));
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
		switch (m_stageObjectData[i].m_objID) {//この処理関数化すべきかも
		case S_HORSE_FRONT:
			m_stageObjects[i] = (new Horse(m_stageObjectData[i].m_x, m_stageObjectData[i].m_y,
				m_stageObjectData[i].m_r, true));
			break;
		case S_HORSE_BACK:
			m_stageObjects[i] = (new Horse(m_stageObjectData[i].m_x, m_stageObjectData[i].m_y,
				m_stageObjectData[i].m_r, false));
			break;

		case S_FERRISWHEEL:
			m_stageObjects[i] = (new FerrisWheel(m_stageObjectData[i].m_x, m_stageObjectData[i].m_y,
				m_stageObjectData[i].m_r));
			break;
		case S_GONDOLA:
			m_stageObjects[i] = (new Gondola(m_stageObjectData[i].m_x, m_stageObjectData[i].m_y,
				m_stageObjectData[i].m_r));
			break;
		case S_LAMP_LEFT:
			m_stageObjects[i] = (new Lamp(m_stageObjectData[i].m_x, m_stageObjectData[i].m_y,
				m_stageObjectData[i].m_r,true));
			break;
		case S_LAMP_RIGHT:
			m_stageObjects[i] = (new Lamp(m_stageObjectData[i].m_x, m_stageObjectData[i].m_y,
				m_stageObjectData[i].m_r, false));
			break;
		}
		
		m_repopCnt[i] = 0;
		m_standby[i] = false;
	}
}

void StageObjectManager::Update()
{
	if (firstFrame)
	{
		StageObjectManager::Initialize();
		firstFrame = false;
	}

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

			if (m_repopCnt[i] >= m_stageObjectData[i].m_repopTime - 40 && m_standby[i] == false)
			{
				EffectManager::CreateEffect(SpawnEffect, XMFLOAT2(m_stageObjectData[i].m_x, m_stageObjectData[i].m_y),
					XMFLOAT2(300, 300), 0, 30);
				m_standby[i] = true;
			}

			if (m_repopCnt[i] >= m_stageObjectData[i].m_repopTime)
			{
				switch (m_stageObjectData[i].m_objID) {
				case S_HORSE_FRONT:
					m_stageObjects[i] = (new Horse(m_stageObjectData[i].m_x, m_stageObjectData[i].m_y,
						m_stageObjectData[i].m_r, true));
					break;
				case S_HORSE_BACK:
					m_stageObjects[i] = (new Horse(m_stageObjectData[i].m_x, m_stageObjectData[i].m_y,
						m_stageObjectData[i].m_r, false));
					break;
				case S_FERRISWHEEL:
					m_stageObjects[i] = (new FerrisWheel(m_stageObjectData[i].m_x, m_stageObjectData[i].m_y,
						m_stageObjectData[i].m_r));
					break;
				case S_GONDOLA:
					m_stageObjects[i] = (new Gondola(m_stageObjectData[i].m_x, m_stageObjectData[i].m_y,
						m_stageObjectData[i].m_r));
					break;
				case S_LAMP_LEFT:
					m_stageObjects[i] = (new Lamp(m_stageObjectData[i].m_x, m_stageObjectData[i].m_y,
						m_stageObjectData[i].m_r, true));
					break;
				case S_LAMP_RIGHT:
					m_stageObjects[i] = (new Lamp(m_stageObjectData[i].m_x, m_stageObjectData[i].m_y,
						m_stageObjectData[i].m_r, false));
					break;
				}
				m_repopCnt[i] = 0;
				m_standby[i] = false;
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

