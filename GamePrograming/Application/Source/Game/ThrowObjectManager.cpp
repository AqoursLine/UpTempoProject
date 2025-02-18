/******************************************************
* ThrowObjectManager.cpp	投げるオブジェクト管理
* 制作者：ミヤタジョウジ
* 作成日：2024/11/12
* 最終更新日：2024/11/12
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"
#include "Game/ThrowObjectManager.h"
#include "Game/WoodenBox.h"
#include "Game/Kokeshi.h"
#include "Game/Bear.h"
#include "Game/Apple.h"
#include "Game/Shield.h"
#include "Game/Coin.h"
#include "Game/Sword.h"
#include "Game/Slime.h"
#include "Game/Board.h"
#include "Game/Cloud.h"
#include "Game/Scaffold.h"
#include "Game/Grass.h"
#include "Game/Moon.h"
#include "Game/Note.h"
#include "Game/Platform.h"
#include "Game/Houki.h"
#include "Game/PC.h"
#include "Game/Whale.h"
#include "Game/Anchor.h"
#include "Game/Coral.h"
#include "Game/Barrel.h"
#include "Game/Shell.h"
#include "Game/ABCblock.h"
#include "Game/Balloon.h"
#include "Game/Clown.h"
#include "Game/CoffeeCup.h"
#include "Game/Ferriswheel.h"
#include "Game/MerrygoroundBear.h"
#include "Game/Horse.h"
#include "Game/RespawnScaffold.h"

#include "Game/EffectManager.h"

std::list<ThrowObject*> ThrowObjectManager::m_throwObjects;

/****************************************************
* 投げるオブジェクト初期化
*****************************************************/
ThrowObjectManager::ThrowObjectManager() {
	m_throwObjects.push_back(new WoodenBox(SCREEN_WIDTH * 0.5f - 250.0f, SCREEN_HEIGHT * 0.5f + 200, 0.0f));
	m_throwObjects.push_back(new Kokeshi(SCREEN_WIDTH * 0.5f + 250.0f, SCREEN_HEIGHT * 0.5f + 200, 0.0f));
	m_throwObjects.push_back(new Bear(SCREEN_WIDTH * 0.5f + 100.0f, SCREEN_HEIGHT * 0.5f + 200, 0.0f));
}

/****************************************************
* 投げるオブジェクト終了
*****************************************************/
ThrowObjectManager::~ThrowObjectManager() {
	for (auto throwObject : m_throwObjects) {
		throwObject->Finalize();
	}

	for (auto throwObject : m_throwObjects) {
		delete throwObject;
	}

	m_throwObjects.clear();
}

/****************************************************
* 投げるオブジェクト更新
*****************************************************/
void ThrowObjectManager::Update() {
	//
	m_stageObjectManager.Update();

	for (auto throwObject : m_throwObjects) {
		throwObject->Update();
	}

	for (auto itr = m_throwObjects.begin(); itr != m_throwObjects.end();) {
		if ((*itr)->GetIsDelete()) {
			ThrowObject* tmp = (*itr);

			// ヒットエフェクト
			EffectManager::CreateEffect(ObjectHitOther, tmp->GetPos(), XMFLOAT2(600.0f, 600.0f), 0.0f);

			itr = m_throwObjects.erase(itr);
			tmp->Finalize();
			delete tmp;
		} else {
			++itr;
		}
	}


	
	constexpr int effectDrawTime = 30;//スポーンエフェクトのパターン数が30だから30が無難？

	// 時間になったら追加準備
	if (m_currentFrame >= m_spawnTime - effectDrawTime - 10 && m_standby == false) {
		m_spawnNum = rand() % SPAWN_OBJECT_MAX;//0～最大-1
		int spawnDistance = 1500 / (m_spawnNum + 1);
		for (int i = 0; i < m_spawnNum + 1; i++)
		{
			m_spwnPos[i].x = rand() % (spawnDistance + 1) + (200 + spawnDistance * i); // 200~1700の値から抽選
			m_spwnPos[i].y = SCREEN_HEIGHT * 0.5f + 200;
			EffectManager::CreateEffect(SpawnEffect, XMFLOAT2(m_spwnPos[i].x, m_spwnPos[i].y), XMFLOAT2(300, 300), 0, effectDrawTime);
		}
		m_standby = true;
	}

	// 時間になったらモノを追加
	if (m_currentFrame >= m_spawnTime) {

		for (int i = 0; i < m_spawnNum + 1; i++)
		{
			int lottery_num = rand() % m_lotteryObjects.size(); // モノの抽選

			// 座標の設定
			XMFLOAT2 Coordinate = m_spwnPos[i];//この処理いらんけどめんどい

			switch (m_lotteryObjects[lottery_num]) {
			case KOKESHI:
				m_throwObjects.push_back(new Kokeshi(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case BEAR:
				m_throwObjects.push_back(new Bear(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case WOODENBOX:
				m_throwObjects.push_back(new WoodenBox(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case APPLE:
				m_throwObjects.push_back(new Apple(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case COIN:
				m_throwObjects.push_back(new Coin(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case SLIME:
				m_throwObjects.push_back(new Slime(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case SWORD:
				m_throwObjects.push_back(new Sword(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case SHIELD:
				m_throwObjects.push_back(new Shield(Coordinate.x, Coordinate.y, 0.0f));
				break;

			case MOON:
				m_throwObjects.push_back(new Moon(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case SCAFFOLD:
				m_throwObjects.push_back(new Scaffold(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case GRASS:
				m_throwObjects.push_back(new Grass(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case BOARD:
				m_throwObjects.push_back(new Board(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case CLOUD:
				m_throwObjects.push_back(new Cloud(Coordinate.x, Coordinate.y, 0.0f));
				break;


			case NOTE:
				m_throwObjects.push_back(new Note(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case PC:
				m_throwObjects.push_back(new PersonalComputer(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case HOUKI:
				m_throwObjects.push_back(new Houki(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case PLATFORM:
				m_throwObjects.push_back(new Platform(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case TEACHER:
				break;
			case SHELL:
				m_throwObjects.push_back(new Shell(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case BARREL:
				m_throwObjects.push_back(new Barrel(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case CORAL:
				m_throwObjects.push_back(new Coral(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case ANCHOR:
				m_throwObjects.push_back(new Anchor(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case WHALE:
				m_throwObjects.push_back(new Whale(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case ABCBLOCK:
				m_throwObjects.push_back(new Abcblock(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case BALLOON:
				m_throwObjects.push_back(new Balloon(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case CLOWN:
				m_throwObjects.push_back(new Clown(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case COFFEECUP:
				m_throwObjects.push_back(new CoffeeCup(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case FERRISWHEEL:
				m_throwObjects.push_back(new Ferriswheel(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case MERRYGOROUNDBEAR:
				m_throwObjects.push_back(new MerrygoroundBear(Coordinate.x, Coordinate.y, 0.0f));
				break;
			case HORSEFRONT:
				m_throwObjects.push_back(new Horse(Coordinate.x, Coordinate.y, 0.0f, true));
				break;
			case HORSEBACK:
				m_throwObjects.push_back(new Horse(Coordinate.x, Coordinate.y, 0.0f, false));
				break;


			default:
				break;
			}
		}


		m_currentFrame = 0.0f; // フレームをリセット
		m_standby = false;
		m_spawnNum = 0;
		m_spawnTime = rand() % (360 + 1) + 120;
	}

	m_currentFrame++;
}

/****************************************************
* 投げるオブジェクト描画
*****************************************************/
void ThrowObjectManager::Draw() {
	//
	m_stageObjectManager.Draw();

	for (auto throwObject : m_throwObjects) {
		throwObject->Draw();
	}
}

/****************************************************
* リストの最後尾にモノを追加する
*****************************************************/
void ThrowObjectManager::PushLotteryObject(const THROWOBJECT_ID& ObjectID) {
	m_lotteryObjects.push_back(ObjectID);
}

void ThrowObjectManager::PushRespawnScaffold(float x, float y, int pnum)
{
	m_throwObjects.push_back(new R_Scaffold(x, y, 0.0f, pnum));
}
