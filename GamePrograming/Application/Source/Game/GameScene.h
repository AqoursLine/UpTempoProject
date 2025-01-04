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
#include <random>

class GameScene : public Scene {
public:
	GameScene();
	~GameScene();

	void Update() override;
	void Draw() override;



private:
	Phase* m_phase = nullptr;

	int m_phaseNum;

	std::mt19937* m_mt;

	bool m_isFinished=false;//1/4　センバ追加
};

