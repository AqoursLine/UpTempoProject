/******************************************************
* Field.cpp	�t�B�[���h
* ����ҁF�~���^�W���E�W
* �쐬���F2024/11/05
* �ŏI�X�V���F2024/11/06
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "Game/Field.h"

/****************************************************
* �t�B�[���h������
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
* �t�B�[���h�I��
*****************************************************/
Field::~Field() {
}

/****************************************************
* �t�B�[���h�X�V
*****************************************************/
void Field::Update() {

}

/****************************************************
* �t�B�[���h�`��
*****************************************************/
void Field::Draw() {
	XMFLOAT2 pos = Physics::ConvertB2toDXFloat2(m_body->GetPosition());
	float rot = m_body->GetAngle();
	D3D.Draw2D(tex, pos.x, pos.y, m_size.x, m_size.y, rot, 0.0f, 0.0f, 1.0f, 1.0f);
}
