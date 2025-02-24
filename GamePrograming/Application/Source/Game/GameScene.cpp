/******************************************************
* GameScene.h		ゲームシーン管理
* 制作者：センバソラ
* 作成日：2024/1/4
* 最終更新日：2024/1/4
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameScene.h"
#include "Game/PhaseClassRoom.h"
#include "Game/PhaseGame.h"
#include "Game/PhaseOcean.h"
#include "Game/PhaseAmusementPark.h"
#include "DirectX/Audio.h"

/****************************************************
* ゲームシーン初期化
*****************************************************/
GameScene::GameScene() {
	std::random_device rd;
	m_mt = new std::mt19937(rd());

	m_camera = new Camera();

	m_ef = new EffectManager();

	m_phaseNum = 0;

	classRoomSound = AUDIO.LoadWaveFile("Data/Sound/BGM/KAMIKAZE_ATTACK.wav");
	oceanSound = AUDIO.LoadWaveFile("Data/Sound/BGM/wind_feelings_1.wav");
	gameSound = AUDIO.LoadWaveFile("Data/Sound/BGM/ドキドキあそび広場.wav");
	parkSound = AUDIO.LoadWaveFile("Data/Sound/BGM/ドードドド・スタンピード.wav");

	AUDIO.SetVolume(classRoomSound, 0.1f);
	AUDIO.SetVolume(oceanSound, 0.1f);
	AUDIO.SetVolume(gameSound, 0.1f);
	AUDIO.SetVolume(parkSound, 0.1f);

	//1/4　センバ追加
	STAGE stage = SaveData::GetStageNum();
	switch (stage) {
	case STAGE_CLASSROOM:
		m_phase = new PhaseClassRoom(m_phaseNum);
		AUDIO.PlayAudio(classRoomSound, 0);
		break;
	case STAGE_OCEAN:
		m_phase = new PhaseOcean(m_phaseNum);
		AUDIO.PlayAudio(oceanSound, 0);
		break;
	case STAGE_GAME:
		m_phase = new PhaseGame(m_phaseNum);
		AUDIO.PlayAudio(gameSound, 0);
		break;
	case STAGE_PARK:
		m_phase = new PhaseAmusementPark(m_phaseNum);
		AUDIO.PlayAudio(parkSound, 0);
		break;
	default:
		m_phase = nullptr;
		break;
	}
	
}

/****************************************************
* ゲームシーン更新
*****************************************************/
void GameScene::Update() {
	m_phase->Update();
	m_camera->Update();
	m_ef->Update();

	if (m_phase->GetIsFinished()) {
		m_isFinished = true;
	}
}

/****************************************************
* ゲームシーン描画
*****************************************************/
void GameScene::Draw() {
	m_camera->Draw();
	m_phase->Draw();
	m_ef->Draw();
}

/****************************************************
* ゲームシーン終了
*****************************************************/
GameScene::~GameScene() {
	if (m_phase) delete m_phase;
	if (m_camera) delete m_camera;
	if (m_mt) delete m_mt;
	if (m_ef) delete m_ef;

	STAGE stage = SaveData::GetStageNum();
	switch (stage) {
	case STAGE_CLASSROOM:
		AUDIO.StopAudio(classRoomSound);
		break;
	case STAGE_OCEAN:
		AUDIO.StopAudio(oceanSound);
		break;
	case STAGE_GAME:
		AUDIO.StopAudio(gameSound);
		break;
	case STAGE_PARK:
		AUDIO.StopAudio(parkSound);
		break;
	default:
		m_phase = nullptr;
		break;
	}
}


