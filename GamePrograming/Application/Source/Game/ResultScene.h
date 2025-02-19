/******************************************************
* ResultScene.h		リザルトシーン管理
* 制作者：ミヤタジョウジ
* 作成日：2024/1/14
* 最終更新日：2024/1/14
*******************************************************/
#pragma once
#include "Game/Scene.h"
#include "Game/ResultCharacter.h"

enum RESULTSTATE {
	RESULT_START = 0,
	RESULT_RESULT,
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
	//リザルト背景
	Texture m_resultBgTex;

	//ステート
	RESULTSTATE m_state;

	ResultCharacter* m_resultCharacter = nullptr;

	//ステートカウント
	int m_stateCount = 0;

	//ステート関数
	void Start();
	void Result();
	void Wait();
	void Transition();
};


