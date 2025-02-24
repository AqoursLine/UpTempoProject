/******************************************************
* GameScene.h		ゲームシーン管理
* 制作者：ミヤタジョウジ
* 作成日：2024/10/22
* 最終更新日：2024/10/22
*******************************************************/
#pragma once
#include "Game/Scene.h"
#include "Game/Phase.h"
#include "SaveData.h"
#include "Game/EffectManager.h"
#include <random>


class GameScene : public Scene {
public:
	GameScene();
	~GameScene();

	void Update() override;
	void Draw() override;

private:
	Phase* m_phase = nullptr;

	EffectManager* m_ef = nullptr;

	int m_phaseNum;

	std::mt19937* m_mt;

	int classRoomSound;	// 教室のBGM
	int oceanSound;		// 海のBGM
	int gameSound;		// ゲームのBGM
	int parkSound;		// 遊園地のBGM
};



