/******************************************************
* GameScene.h		ゲームシーン管理
* 制作者：ミヤタジョウジ
* 作成日：2024/10/22
* 最終更新日：2024/10/22
*******************************************************/
#pragma once
#include "Game/Scene.h"
#include "Game/Phase.h"


class GameScene : public Scene {
public:
	GameScene();
	~GameScene();

	void Update() override;
	void Draw() override;

	void ChangePhase();

private:
	Phase* m_phase = nullptr;

	int m_phaseNum;
};

