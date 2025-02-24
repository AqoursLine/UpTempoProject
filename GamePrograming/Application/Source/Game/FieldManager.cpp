/******************************************************
* FieldManager.cpp	フィールド管理
* 制作者：ミヤタジョウジ
* 作成日：2024/11/12
* 最終更新日：2024/11/12
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "DirectX/Audio.h"
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

	constexpr float CEILING_WIDTH = 128.0f;
	constexpr float CEILING_HEIGHT = 82.0f;
	constexpr float WALL_WIDTH = 73.0f;
	constexpr float WALL_HEIGHT = 106.0f;
	constexpr float GROUND_WIDTH = 128.0f;
	constexpr float GROUND_HEIGHT = 125.0f;

	io::CSVReader<6> in("Data/CSV/Field.csv");
	in.read_header(io::ignore_extra_column, "Type", "Name", "texX", "texY", "objX", "objY");

	soundNum = AUDIO.LoadWaveFile("Data/Sound/SE/つるはしで壁を破壊2.wav");

	AUDIO.SetVolume(soundNum, 1.0f);

	int type;
	std::string name;
	float texX, texY;
	float objX, objY;

	while (in.read_row(type, name, texX, texY, objX, objY)) {
		// nameの末尾の改行を削除
		if (!name.empty() && name.back() == '\n') {
			name.pop_back();
		}

		int wideSize = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, nullptr, 0);
		std::wstring fileName;
		fileName.resize(wideSize - 1);
		MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, &fileName[0], wideSize);

		switch (type) {
			//上
			case 1:
				m_fieldObjects.push_back(new FieldObject(XMFLOAT2(objX, objY), 0.0f, XMFLOAT2(CEILING_WIDTH, CEILING_HEIGHT), fileName, XMFLOAT2(texX, texY),TOP));
				break;
			//左
			case 2:
				m_fieldObjects.push_back(new FieldObject(XMFLOAT2(objX, objY), 0.0f, XMFLOAT2(WALL_WIDTH, WALL_HEIGHT), fileName, XMFLOAT2(texX, texY),LEFT));
				break;
			//右
			case 3:
				m_fieldObjects.push_back(new FieldObject(XMFLOAT2(objX, objY), 0.0f, XMFLOAT2(WALL_WIDTH, WALL_HEIGHT), fileName, XMFLOAT2(texX, texY), RIGHT));
				break;
			//下
			case 4:
				m_fieldObjects.push_back(new Ground(XMFLOAT2(objX, objY), 0.0f, XMFLOAT2(GROUND_WIDTH, GROUND_HEIGHT), fileName, XMFLOAT2(texX, texY),BOTTOM));
				break;
			//コーナー
			case 5:
				m_fieldObjects.push_back(new Corner(XMFLOAT2(objX, objY), 0.0f, XMFLOAT2(CEILING_WIDTH, CEILING_HEIGHT), XMFLOAT2(WALL_WIDTH, WALL_HEIGHT), fileName, XMFLOAT2(texX, texY),CORNER));
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

			AUDIO.StopAudio(soundNum);

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

