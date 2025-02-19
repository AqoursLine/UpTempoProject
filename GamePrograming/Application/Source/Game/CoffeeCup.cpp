/******************************************************
* CoffeeCup.cpp	???[?q?|?J?b?v
* ?????@?i?J?K?????I??
* ?쐬???F2024/12/29
* ?ŏI?X?V???F2024/12/29
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"

#include "CoffeeCup.h"

CoffeeCup::CoffeeCup(float x, float y, float r) :ThrowObject(x, y, r)
{
	//?e?N?X?`???ݒ?
	m_uv.x = 0.1f;
	m_uv.y = 0.1f;
	m_texSize.x = 0.87f;
	m_texSize.y = 0.75f;

	//?T?C?Y?ݒ?
	float aspect = m_texSize.x / m_texSize.y;
	float height = 100.0f;
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
	m_tex.Load(L"Data/Texture/CoffeeCup.png");
	
	//?d??
	m_weight = WEIGHT_HEAVY;
}

CoffeeCup::~CoffeeCup()
{
}
