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
}

/****************************************************
* 投げるオブジェクト描画
*****************************************************/
void ThrowObjectManager::Draw() {
	for (auto throwObject : m_throwObjects) {
		throwObject->Draw();
	}
}
