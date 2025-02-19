/******************************************************
* GameScene.h		?Q?[???V?[???Ǘ?
* ?????F?~???^?W???E?W
* ?쐬???F2024/10/22
* ?ŏI?X?V???F2024/10/22
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
};
