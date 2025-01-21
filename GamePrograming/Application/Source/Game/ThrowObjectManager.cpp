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
#include "Game/Note.h"
#include "Game/Platform.h"
#include "Game/Houki.h"
#include "Game/PC.h"
#include "Game/Whale.h"
#include "Game/Anchor.h"
#include "Game/Coral.h"
#include "Game/Barrel.h"
#include "Game/Shell.h"
#include "Game/EffectManager.h"

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


	// 時間になったらモノを追加
	if (m_currentFrame >= 120) {

		int lottery_num = rand() % m_lotteryObjects.size(); // モノの抽選

		// 座標の抽選
		XMFLOAT2 Coordinate;
		Coordinate.x = rand() % (1700 - 200 + 1) + 200; // 200~1700の値から抽選
		Coordinate.y = SCREEN_HEIGHT * 0.5f + 200;

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
			default:
				break;
		}



		m_currentFrame = 0.0f; // フレームをリセット
	}

	m_currentFrame++;
}

/****************************************************
* 投げるオブジェクト描画
*****************************************************/
void ThrowObjectManager::Draw() {
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