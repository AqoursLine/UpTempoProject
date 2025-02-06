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

	constexpr int VER_MAX = 8;
	constexpr int HOR_MAX = 15;
	
	float height = SCREEN_HEIGHT / VER_MAX;
	float width = SCREEN_WIDTH / HOR_MAX;

	int uvNum = 0;

	//天井
	for (int i = 0; i < HOR_MAX; i++) {
		m_fieldObjects.push_back(new FieldObject(XMFLOAT2(width * 0.5f + width * i, size * 0.5f), 0.0f, XMFLOAT2(width, size), uvNum));
		uvNum++;
	}

	//左
	for (int i = 0; i < VER_MAX; i++) {
		m_fieldObjects.push_back(new FieldObject(XMFLOAT2(size * 0.5f, height * 0.5f + height * i), 0.0f, XMFLOAT2(size, height), uvNum));
		uvNum++;
	}

	//右
	for (int i = 0; i < VER_MAX; i++) {
		m_fieldObjects.push_back(new FieldObject(XMFLOAT2(SCREEN_WIDTH - size * 0.5f, height * 0.5f + height * i), 0.0f, XMFLOAT2(size, height), uvNum));
		uvNum++;
	}

	//床
	for (int i = 0; i < HOR_MAX; i++) {
		m_fieldObjects.push_back(new Ground(XMFLOAT2(width * 0.5f + width * i, SCREEN_HEIGHT - size * 0.5f), 0.0f, XMFLOAT2(width, size), uvNum));
		uvNum++;
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

/****************************************************
* フィールド作り直し
*****************************************************/
void FieldManager::ReCreateField(float sizeTop, float sizeLeft, float sizeRight, float sizeButtom) {
	for (auto fieldObject : m_fieldObjects) {
		delete fieldObject;
	}

	m_fieldObjects.clear();

	constexpr int VER_MAX = 8;
	constexpr int HOR_MAX = 15;

	float height = SCREEN_HEIGHT / VER_MAX;
	float width = SCREEN_WIDTH / HOR_MAX;

	int uvNum = 0;

	//天井
	for (int i = 0; i < HOR_MAX; i++) {
		m_fieldObjects.push_back(new FieldObject(XMFLOAT2(width * 0.5f + width * i, sizeTop * 0.5f), 0.0f, XMFLOAT2(width, sizeTop), uvNum));
		uvNum++;
	}

	//左
	for (int i = 0; i < VER_MAX; i++) {
		m_fieldObjects.push_back(new FieldObject(XMFLOAT2(sizeLeft * 0.5f, height * 0.5f + height * i), 0.0f, XMFLOAT2(sizeLeft, height), uvNum));
		uvNum++;
	}

	//右
	for (int i = 0; i < VER_MAX; i++) {
		m_fieldObjects.push_back(new FieldObject(XMFLOAT2(SCREEN_WIDTH - sizeRight * 0.5f, height * 0.5f + height * i), 0.0f, XMFLOAT2(sizeRight, height), uvNum));
		uvNum++;
	}

	//床
	for (int i = 0; i < HOR_MAX; i++) {
		m_fieldObjects.push_back(new Ground(XMFLOAT2(width * 0.5f + width * i, SCREEN_HEIGHT - sizeButtom * 0.5f), 0.0f, XMFLOAT2(width, sizeButtom), uvNum));
		uvNum++;
	}
}


