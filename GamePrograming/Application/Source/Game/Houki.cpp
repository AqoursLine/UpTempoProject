/******************************************************
* Houki.cpp	?
* ?????F?C?T?T?g??
* ?쐬???F2024/12/05
* ?ŏI?X?V???F2024/12/05
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"
#include "Houki.h"

/****************************************************
* ⴏ?????
*****************************************************/
Houki::Houki(float x, float y, float r) : ThrowObject(x, y, r) {
	//?e?N?X?`???ݒ?
	m_uv.x = 0.3f;
	m_uv.y = 0.05f;
	m_texSize.x = 0.5f;
	m_texSize.y = 0.9f;

	//?T?C?Y
	float aspect = m_texSize.x / m_texSize.y;
	float height = 120.0f;
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
	m_tex.Load(L"Data/Texture/Houki.png");

	//?d??
	m_weight = WEIGHT_NORMAL;


}

/****************************************************
* ⴏI??
*****************************************************/
Houki::~Houki() {

}

/****************************************************
* Update
*****************************************************/
void Houki::Update(){
	ThrowObject::Update();

	
	if (GetIsThrow() && m_first)
	{
		Physics::SetDensity(&m_body, 0.0f);
		m_first = false;
	}
	if (GetIsThrow()&&downFirst)
	{
		int a = m_pos.x - m_throwPos.x;
		int b = m_pos.y - m_throwPos.y;
		int c = a * a + b * b;

		int downDistance = 700;
		if ( c >= downDistance* downDistance)
		{
			Physics::SetDensity(&m_body, 1.0f);
			m_body->SetLinearVelocity(b2Vec2(0,5));
			downFirst = false;
		}
		
	}

}
