/******************************************************
* ResultScene.h		リザルトシーン管理
* 制作者：ミヤタジョウジ
* 作成日：2024/1/14
* 最終更新日：2024/1/14
*******************************************************/
#pragma once
#include "Game/Scene.h"

class ResultScene : public Scene {
public:
	ResultScene();
	~ResultScene();

	void Update() override;
	void Draw() override;
private:
};

