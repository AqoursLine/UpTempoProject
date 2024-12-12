/******************************************************
* Field.cpp	フィールド
* 制作者：ミヤタジョウジ
* 作成日：2024/11/05
* 最終更新日：2024/11/06
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "Game/Field.h"
#include "Game/Phase.h"


/****************************************************
* フィールド初期化
*****************************************************/
Field::Field() {
	m_HP = 75;

	m_hpTex.Load("Data/Texture/score_num.png");
}

/****************************************************
* フィールド終了
*****************************************************/
Field::~Field() {
}

/****************************************************
* フィールド更新
*****************************************************/
void Field::Update() {
}

/****************************************************
* フィールド描画
*****************************************************/
void Field::Draw() {
	D3D.Draw2D(m_bgTex, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	//残りｈｐ描画
	for (int i = 0; i < 3; i++) {
		int tmp = (int)(m_HP / (int)powf(10, i)) % 10;

		float u = (1.0f / 10.0f) * tmp;

		D3D.Draw2D(m_hpTex, SCREEN_WIDTH * 0.5f - i * 50, 200, 50, 100, 0.0f, u, 0.0f, (1.0f / 10.0f), 1.0f);
	}
}

/****************************************************
* ダメージ
*****************************************************/
void Field::Attack(int attack) {
	m_HP -= attack;
	if (m_HP <= 0) {
		Phase::ChangeState(PHASESTATE_FINISH);
	}
}

/****************************************************
* 背景ロード
*****************************************************/
void Field::LoadBackGround(std::string fileName) {
	std::string filePath = "Data/Texture/";

	filePath += fileName;

	m_bgTex.Load(filePath);
}
