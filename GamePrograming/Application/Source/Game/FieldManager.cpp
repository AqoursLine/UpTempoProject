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
#include "Game/Corner.h"

#include <iostream>
#include "Game/csv.h"
#include <codecvt>

/****************************************************
* フィールド管理初期化
*****************************************************/
FieldManager::FieldManager() {
	m_fieldObjects.clear();

	constexpr float CEILING_WIDTH = 1920.0f / 15;
	constexpr float CEILING_HEIGHT = 82.0f;
	constexpr float WALL_WIDTH = 74.0f;
	constexpr float WALL_HEIGHT = 1080.0f / 10;
	constexpr float GROUND_WIDTH = 1920.0f / 15;
	constexpr float GROUND_HEIGHT = 126.0f;

	io::CSVReader<4> in("Data/CSV/Field.csv");
	in.read_header(io::ignore_extra_column, "Type", "uvNum", "objX", "objY");

	int type;
	int uvNum;
	float objX, objY;

	while (in.read_row(type, uvNum, objX, objY)) {

		switch (type) {
			//上
			case 1:
				m_fieldObjects.push_back(new FieldObject(XMFLOAT2(objX, objY), 0.0f, XMFLOAT2(CEILING_WIDTH, CEILING_HEIGHT), uvNum, TOP));
				break;
			//左
			case 2:
				m_fieldObjects.push_back(new FieldObject(XMFLOAT2(objX, objY), 0.0f, XMFLOAT2(WALL_WIDTH, WALL_HEIGHT), uvNum, LEFT));
				break;
			//右
			case 3:
				m_fieldObjects.push_back(new FieldObject(XMFLOAT2(objX, objY), 0.0f, XMFLOAT2(WALL_WIDTH, WALL_HEIGHT), uvNum, RIGHT));
				break;
			//下
			case 4:
				m_fieldObjects.push_back(new Ground(XMFLOAT2(objX, objY), 0.0f, XMFLOAT2(GROUND_WIDTH, GROUND_HEIGHT), uvNum, BOTTOM));
				break;
			//コーナー
			case 5:
				m_fieldObjects.push_back(new Corner(XMFLOAT2(objX, objY), 0.0f, XMFLOAT2(CEILING_WIDTH, CEILING_HEIGHT), XMFLOAT2(WALL_WIDTH, WALL_HEIGHT), uvNum, CORNER));
				break;
			default:
				break;
		}
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

	constexpr int VER_MAX = 10;
	constexpr int HOR_MAX = 15;

	float height = SCREEN_HEIGHT / VER_MAX;
	float width = SCREEN_WIDTH / HOR_MAX;

	//左
	//for (int i = 0; i < VER_MAX; i++) {
	//	m_fieldObjects.push_back(new FieldObject(XMFLOAT2(sizeLeft * 0.5f, height * 0.5f + height * i), 0.0f, XMFLOAT2(sizeLeft, height)));
	//}

	////右
	//for (int i = 0; i < VER_MAX; i++) {
	//	m_fieldObjects.push_back(new FieldObject(XMFLOAT2(SCREEN_WIDTH - sizeRight * 0.5f, height * 0.5f + height * i), 0.0f, XMFLOAT2(sizeRight, height)));
	//}

	////床
	//for (int i = 0; i < HOR_MAX; i++) {
	//	m_fieldObjects.push_back(new Ground(XMFLOAT2(width * 0.5f + width * i, SCREEN_HEIGHT - sizeButtom * 0.5f), 0.0f, XMFLOAT2(width, sizeButtom)));
	//}

	////天井
	//for (int i = 0; i < HOR_MAX; i++) {
	//	m_fieldObjects.push_back(new FieldObject(XMFLOAT2(width * 0.5f + width * i, sizeTop * 0.5f), 0.0f, XMFLOAT2(width, sizeTop)));
	//}

}

