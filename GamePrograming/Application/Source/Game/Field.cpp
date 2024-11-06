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

/****************************************************
* フィールド初期化
*****************************************************/
Field::Field() {
	b2Vec2 pos = Physics::ConvertDXtoB2Float2(XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f + 400.0f));
	Physics::CreateBody(&m_body, pos.x, pos.y, 0.0f, false, this);

	m_size = XMFLOAT2(1000, 100);
	b2Vec2 b2size = Physics::ConvertDXtoB2Float2(m_size);
	Physics::CreateFixture(&m_body, b2size.x, b2size.y);

	SetTag("Ground");

	tex.Load("Data/Texture/Logo.png");
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
	XMFLOAT2 pos = Physics::ConvertB2toDXFloat2(m_body->GetPosition());
	float rot = m_body->GetAngle();
	D3D.Draw2D(tex, pos.x, pos.y, m_size.x, m_size.y, rot, 0.0f, 0.0f, 1.0f, 1.0f);
}
