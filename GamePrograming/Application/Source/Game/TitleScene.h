/******************************************************
* TitleScene.h		タイトルシーン管理
* 制作者：ミヤタジョウジ
* 作成日：2024/1/14
* 最終更新日：2024/1/14
*******************************************************/
#pragma once
#include "Game/Scene.h"
#include "Game/TitleBackGroundMovie.h"
#include "Game/Transition.h"

enum TITLESTATE {
	TITLE_RUN = 0,
	TITLE_START,
	TITLE_TRANSITION,
	TITLE_LOGO,
	TITLE_OP,
};

class TitleScene : public Scene {
public:
	TitleScene();
	~TitleScene();

	void Update() override;
	void Draw() override;

private:
	//ロゴ
	Texture m_logoTex;

	//選択肢テクスチャ
	Texture m_startTex;
	Texture m_quitTex;
	Texture m_titleChoose;

	//描画用
	XMFLOAT2 m_pos;
	XMFLOAT2 m_size;

	//選択肢の距離
	float m_distance = 200.0f;
	//選択している番号
	int m_choose = 0;

	//ステート
	TITLESTATE m_state;

	Transition m_IN_transition;
	Transition m_OUT_transition;

	//動画
	VideoTexture m_logo;
	int m_logoSound;
	VideoTexture m_op;

	//ステート関数
	void Run();
	void Start();
	void Transition();
	void Logo();
	void Opening();


	// 背景動画
	std::unique_ptr<BackGroundMovie> m_backMovie;

	//SE用変数
	int m_soundNum;
	int m_decisionSound;// 決定音

};



