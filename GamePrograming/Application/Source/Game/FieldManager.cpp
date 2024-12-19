/******************************************************
* FieldManager.cpp	フィールド管理
* 制作者：ミヤタジョウジ
* 作成日：2024/11/12
* 最終更新日：2024/11/12
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "Game/FieldManager.h"
#include "Game/Ground.h"

/****************************************************
* フィールド管理初期化
*****************************************************/
FieldManager::FieldManager() {
	m_fieldObjects.clear();

	float size = 70.0f;

	constexpr int VER_MAX = 10;
	constexpr int HOR_MAX = 15;
	
	float height = SCREEN_HEIGHT / VER_MAX;
	float width = SCREEN_WIDTH / HOR_MAX;

	//左
	for (int i = 0; i < VER_MAX; i++) {
		m_fieldObjects.push_back(new FieldObject(XMFLOAT2(size * 0.5f, height * 0.5f + height * i), 0.0f, XMFLOAT2(size, height)));
	}

	//右
	for (int i = 0; i < VER_MAX; i++) {
		m_fieldObjects.push_back(new FieldObject(XMFLOAT2(SCREEN_WIDTH - size * 0.5f, height * 0.5f + height * i), 0.0f, XMFLOAT2(size, height)));
	}

	//床
	for (int i = 0; i < HOR_MAX; i++) {
		m_fieldObjects.push_back(new Ground(XMFLOAT2(width * 0.5f + width * i, SCREEN_HEIGHT - size * 0.5f), 0.0f, XMFLOAT2(width, size)));
	}

	//天井
	for (int i = 0; i < HOR_MAX; i++) {
		m_fieldObjects.push_back(new FieldObject(XMFLOAT2(width * 0.5f + width * i, size * 0.5f), 0.0f, XMFLOAT2(width, size)));
	}
}

/****************************************************
* フィールド管理終了
*****************************************************/
FieldManager::~FieldManager() {
	for (auto fieldObject : m_fieldObjects) {
		delete fieldObject;
	}
	
	m_fieldObjects.clear();
}

/****************************************************
* フィールド管理更新
*****************************************************/
void FieldManager::Update() {
	//オブジェクト更新
	for (auto fieldObject : m_fieldObjects) {
		fieldObject->Update();
	}

	//オブジェクト削除
	for (auto itr = m_fieldObjects.begin(); itr != m_fieldObjects.end();) {
		if ((*itr)->GetIsDelete()) {
			FieldObject* tmp = (*itr);
			itr = m_fieldObjects.erase(itr);
			delete tmp;
		} else {
			++itr;
		}
	}
}

/****************************************************
* フィールド管理描画
*****************************************************/
void FieldManager::Draw() {
	//オブジェクト描画
	for (auto fieldObject : m_fieldObjects) {
		fieldObject->Draw();
	}
}


