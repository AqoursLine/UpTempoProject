/******************************************************
* Coral.cpp	?T???S
* ?????F?J???}?^?g?E
* ?쐬???F2024/12/05
* ?ŏI?X?V???F2024/12/05
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"
#include "Game/Coral.h"


/****************************************************
* ?m?[?g??????
*****************************************************/
Coral::Coral(float x, float y, float r) : ThrowObject(x, y, r) {
	//?e?N?X?`???ݒ?
	m_uv.x = 0.23f;
	m_uv.y = 0.15f;
	m_texSize.x = 0.47f;
	m_texSize.y = 0.68f;

	//?T?C?Y?ݒ?
	float aspect = m_texSize.x / m_texSize.y;
	float height = 100;
	m_size = XMFLOAT2(height * aspect, height);

	//?|?W?V?????ϊ?
	b2Vec2 b2pos = Physics::ConvertDXtoB2Float2(m_pos);
	//?{?f?B?쐬
	Physics::CreateBody(&m_body, b2pos.x, b2pos.y, r, true, this);

	//?T?C?Y?ϊ?
	b2Vec2 b2size = Physics::ConvertDXtoB2Float2(m_size);
	//?????蔻????
	Physics::CreateFixture(&m_body, b2size.x, b2size.y, 1.0f);

	//?e?N?X?`??
	m_tex.Load(L"Data/Texture/coral.png");

	//?d??
	m_weight = WEIGHT_NORMAL;

}

/****************************************************
* ?m?[?g?I??
*****************************************************/
Coral::~Coral() {}
