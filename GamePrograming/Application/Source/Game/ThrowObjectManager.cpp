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
#include "time.h"

/****************************************************
* 投げるオブジェクト初期化
*****************************************************/
ThrowObjectManager::ThrowObjectManager() {

	// 仮で発生させているオブジェクト。本来はフェーズを継承したClassRoomなどのコンストラクタでPushObjectを使って登録する
	m_throwObjects.push_back(new WoodenBox(SCREEN_WIDTH * 0.5f - 250.0f, SCREEN_HEIGHT * 0.5f + 200, 0.0f));
	m_throwObjects.push_back(new Kokeshi(SCREEN_WIDTH * 0.5f + 250.0f, SCREEN_HEIGHT * 0.5f + 200, 0.0f));
	m_throwObjects.push_back(new Bear(SCREEN_WIDTH * 0.5f + 100.0f, SCREEN_HEIGHT * 0.5f + 200, 0.0f));

	m_currentFrame = 0.0f;

	srand((unsigned int)time(NULL));
}

/****************************************************
* 投げるオブジェクト終了
*****************************************************/
ThrowObjectManager::~ThrowObjectManager() {
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


	// 時間になったらモノを追加
	if (m_currentFrame >= 60) {

		int lottery_num = rand() % m_lotteryObjects.size(); // モノの抽選

		// 座標の抽選
		XMFLOAT2 Coordinate;
		Coordinate.x = rand() % (1700 - 200 + 1) + 200; // 200~1700の値から抽選
		Coordinate.y = SCREEN_HEIGHT * 0.5f + 200;

		switch (lottery_num) {
		case 0:
			m_throwObjects.push_back(new WoodenBox(Coordinate.x, Coordinate.y, 0.0f));
			break;

		case 1:
			m_throwObjects.push_back(new Bear(Coordinate.x, Coordinate.y, 0.0f));
			break;

		case 2:
			m_throwObjects.push_back(new Kokeshi(Coordinate.x, Coordinate.y, 0.0f));
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

void ThrowObjectManager::PushLotteryObject(ThrowObject* pPushObjects)
{
	m_lotteryObjects.push_back(pPushObjects);
}
