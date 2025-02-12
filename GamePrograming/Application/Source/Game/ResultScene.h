/******************************************************
* ResultScene.h		リザルトシーン管理
* 制作者：ミヤタジョウジ
* 作成日：2024/1/14
* 最終更新日：2024/1/14
*******************************************************/
#pragma once
#include "Game/Scene.h"

enum RESULTSTATE {
	RESULT_RESULT = 0,
	RESULT_WAIT,
	RESULT_TRANSITION,
};

class ResultScene : public Scene {
public:
	ResultScene();
	~ResultScene();

	void Update() override;
	void Draw() override;
private:
	//リザルトタイトルテクスチャ
	Texture m_resultTex;
	//タイトルに戻る
	Texture m_goTitleTex;

	//ステート
	RESULTSTATE m_state;

	//ステート関数
	void Result();
	void Wait();
	void Transition();
};

