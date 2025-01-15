/******************************************************
* TitleScene.h		タイトルシーン管理
* 制作者：ミヤタジョウジ
* 作成日：2024/1/14
* 最終更新日：2024/1/14
*******************************************************/
#pragma once
#include "Game/Scene.h"

class TitleScene : public Scene {
public:
	TitleScene();
	~TitleScene();

	void Update() override;
	void Draw() override;

private:
	Texture m_startTex;
	Texture m_quitTex;
	Texture m_titleChoose;

	XMFLOAT2 m_pos;
	XMFLOAT2 m_size;

	float m_distance = 200.0f;
	int m_choose = 0;

	int m_gamepadMax = 0;

	Movie m_test;
};

