#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/SaveData.h"

#include "Game/ChooseScene.h"
#include "Game/Controller.h"
#include "Game/CharacterSelect.h"
#include "Game/StageSelect.h"

ChooseScene::ChooseScene() {
	m_state = CHOOSE_START;
	m_select = new CharacterSelect();
	m_camera = new Camera();
}

ChooseScene::~ChooseScene() {
	if (m_select) delete m_select;
	if (m_camera) delete m_camera;
}

void ChooseScene::Update() {
	switch (m_state) {
		case CHOOSE_CHARACTER:
			Character();
			break;
		case CHOOSE_STAGE:
			Stage();
			break;
		case CHOOSE_START:
			Start();
			break;
		case CHOOSE_FINISH:
			Finish();
			break;
	}
}

void ChooseScene::Draw() {
	m_camera->Draw();

	//選択描画
	m_select->Draw();

	//トランジション途中→終了までの描画
	if (m_state == CHOOSE_START) {

	}

	//トランジション最初→途中までの描画
	if (m_state == CHOOSE_FINISH) {

	}
}

/******************************************************
* キャラクター選択操作
*******************************************************/
void ChooseScene::Character() {
	//キャラクター選択処理
	m_select->Update();

	//ステート変更
	if (m_select->GetIsFinished()) {
		//選択した情報を取得
		CharacterSelect* pCharacterSelect = static_cast<CharacterSelect*>(m_select);


		//ステージ選択へ
		delete m_select;
		m_select = new StageSelect();
		m_state = CHOOSE_STAGE;
	}

}

/******************************************************
* ステージ選択操作
*******************************************************/
void ChooseScene::Stage() {
	//ステージ選択処理
	m_select->Update();

	//ステート変更
	//とりあえずエンターキーで変更
	if (m_select->GetIsFinished()) {
		//選択した情報を取得
		StageSelect* pStageSelect = static_cast<StageSelect*>(m_select);

		//トランジション開始
		m_state = CHOOSE_FINISH;
	}
}

/******************************************************
* トランジション途中→終了時の操作
*******************************************************/
void ChooseScene::Start() {
	//トランジション再生処理

	//トランジション再生終了
	if (true) {
		m_state = CHOOSE_CHARACTER;
	}
}

/******************************************************
* トランジション最初→途中時の操作
*******************************************************/
void ChooseScene::Finish() {
	//トランジション再生処理

	//トランジション再生終了
	if (true) {
		m_isFinished = true;
	}
}


