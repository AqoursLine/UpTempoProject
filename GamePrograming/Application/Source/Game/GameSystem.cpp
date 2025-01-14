/******************************************************
* GameSystem.cpp		ゲームシステムを管理
* 制作者：ミヤタジョウジ
* 作成日：2024/10/5
* 最終更新日：2024/10/22
*******************************************************/
#include "framework.h"
//DirectXクラスを使えるようにする
#include "DirectX/DirectX.h"
#include <box2d/box2d.h>
//GameSystemクラスを使えるようにする
#include "GameSystem.h"
//コントローラーを使えるようにする
#include "Controller.h"
//ゲームシーン
#include "GameScene.h"
//タイトルシーン
#include "Game/TitleScene.h"
//セーブデータ
#include "Game/SaveData.h"

/******************************************************
* ゲームの初期化
*******************************************************/
void GameSystem::Initialize() {
	//セーブデータをとりあえず設定
	SaveData::SetTotalPlayer(2);
	SaveData::SetStage(STAGE_CLASSROOM);

	//シーンを作成
	m_sceneNum = SCENE_TITLE;
	ChangeScene(m_sceneNum);

	//マトリクス初期化
	D3D.SetWorldViewProjection2D();

	//時間計測開始
	m_oldTime = timeGetTime();
}

/******************************************************
* ゲームの更新
*******************************************************/
void GameSystem::Excute() {
	//時間更新
	DWORD currentTime = timeGetTime();

	m_deltaTime = currentTime - m_oldTime;
	m_elapsedTime += m_deltaTime;

	m_oldTime = currentTime;

	//コントローラーを更新
	CTRL.UpdateController();

	//更新
	m_scene->Update();

	//描画
	m_scene->Draw();

	if (m_scene->GetISEnd()) {
		m_isEnd = true;
		return;
	}

	if (m_scene->GetIsFinished()) {
		m_sceneNum = static_cast<SCENES>(m_sceneNum + 1);
		ChangeScene(m_sceneNum);
	}
}

/******************************************************
* ゲームの終了
*******************************************************/
void GameSystem::Finalize() {
//	m_scene->FinalizeGameObject();

	if (m_scene) {
		delete m_scene;
	}


	CTRL.Finalize();
}

/******************************************************
* シーン遷移
*******************************************************/
void GameSystem::ChangeScene(SCENES scene) {
	if (m_scene) {
		delete m_scene;
	}

	switch (scene) {
		case SCENE_TITLE:
			m_scene = new TitleScene();
			break;
		case SCENE_GAME:
			m_scene = new GameScene();
			break;
		case SCENE_RESULT:
			break;
		default:
			break;
	}
}

/******************************************************
* 経過時間取得
*******************************************************/
const float GameSystem::GetElapsedTime() const {
	return (m_elapsedTime / 1000.0f);
}

/******************************************************
* １フレーム時間取得
*******************************************************/
const float GameSystem::GetDletaTime() const {
	return (m_deltaTime / 1000.0f);
}
