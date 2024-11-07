/******************************************************
* Ground.cpp	地面
* 制作者：ミヤタジョウジ
* 作成日：2024/11/07
* 最終更新日：2024/11/07
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"

#include "Game/Ground.h"

/****************************************************
* 地面初期化
*****************************************************/
Ground::Ground() {
	b2Vec2 pos = Physics::ConvertDXtoB2Float2(XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f + 300.0f));
	Physics::CreateBody(&m_body, pos.x, pos.y, 0.0f, false, this);

	m_size = XMFLOAT2(1000.0f, 10.0f);
	b2Vec2 size = Physics::ConvertDXtoB2Float2(m_size);
	Physics::CreateFixture(&m_body, size.x, size.y);

	m_tex.Load("Data/texture/Logo.png");

	SetTag("Ground");
}

/****************************************************
* 地面終了
*****************************************************/
Ground::~Ground() {

}

/****************************************************
* 地面更新
*****************************************************/
void Ground::Update() {

}

/****************************************************
* 地面描画
*****************************************************/
void Ground::Draw() {
	XMFLOAT2 pos = Physics::ConvertB2toDXFloat2(m_body->GetPosition());

	D3D.Draw2D(m_tex, pos.x, pos.y, m_size.x, m_size.y, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
}


