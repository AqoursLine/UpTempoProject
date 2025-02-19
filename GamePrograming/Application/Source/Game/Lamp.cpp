/******************************************************
* Lamp.cpp	?u????
* ?????Fmurayama
* ?쐬???F2024/01/23
* ?ŏI?X?V???F
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"
#include "Game/Lamp.h"


/****************************************************
* ??????
*****************************************************/
Lamp::Lamp(float x, float y, float r,bool left) : ThrowObject(x, y, r) {

	m_hp = 3;//?????˂????痎?????邩
	m_sizeAdjust = 1.05f;

	//?e?N?X?`???ݒ?//?????ɂ??Ă邩??f?????ʂ????ǒ??????邩??
	if (left)
	{//?????̃e?N?X?`??
		m_uv.x = 0.02f;
		m_uv.y = 0.3f;
		m_texSize.x = 0.96f;
		m_texSize.y = 0.24f;
	}
	else
	{
		m_uv.x = 0.02f;
		m_uv.y = 0.3f;
		m_texSize.x = 0.96f;
		m_texSize.y = 0.24f;
	}

	//?T?C?Y?ݒ?	?|?W?V?????ϊ??Ɠ????蔻??????R?????g?A?E?g???Ă??[?h?ɂ????????̕?????????????
	float aspect = m_texSize.x / m_texSize.y;
	float height = 120.0f;
	m_size = XMFLOAT2(height * aspect, height);

	//?|?W?V?????ϊ?
	//b2Vec2 b2pos = Physics::ConvertDXtoB2Float2(XMFLOAT2( m_pos.x,m_pos.y + 25));//25??120?̏ꍇ
	b2Vec2 b2pos = Physics::ConvertDXtoB2Float2(XMFLOAT2( m_pos.x,m_pos.y));
	//?{?f?B?쐬
	Physics::CreateBody(&m_body, b2pos.x, b2pos.y, r, false, this);

	//?T?C?Y?ϊ?
	b2Vec2 b2size = Physics::ConvertDXtoB2Float2(m_size);
	//?????蔻????
	Physics::CreateFixture(&m_body, b2size.x, b2size.y, 1.0f);//*0.5?Ő??̕????̔???????????
	//Physics::CreateFixture(&m_body, b2size.x, b2size.y * 0.45, 1.0f);//*0.5?Ő??̕????̔???????????


	
	//?e?N?X?`??
	if(left)
		m_tex.Load(L"Data/Texture/LampLeft.png");
	else
		m_tex.Load(L"Data/Texture/LampRight.png");

	//?d??
	m_weight = WEIGHT_HEAVY;

	SetTag("Lamp");

}

/****************************************************
* ???イ????
*****************************************************/
Lamp::~Lamp() {

}

/****************************************************
* Update
*****************************************************/
void Lamp::Update() {

	
	
	if (m_hp <= 0 && first)
	{
		m_fixed = false;
		m_body->SetType(b2_dynamicBody);
		b2Fixture* fixture = (m_body)->GetFixtureList();
		fixture->SetDensity(0.3f);//?d?????Ĕ??Ȃ?????0.
		fixture->SetFriction(0.3f);
		fixture->SetRestitution(0.0f);
		m_body->ResetMassData();
		SetTag("ThrowObject");
		first = false;
	
	}
	if (m_fixed)
	{
	}
	else
	{
		ThrowObject::Update();
	}

}

void Lamp::OnCollisionEnter(GameObject* collision)
{

	if (m_fixed)
	{
		if (collision->CompareTag("ThrowObject") && ((ThrowObject*)collision)->GetIsThrow()) 
		{
			m_hp -= 1;
			((ThrowObject*)collision)->SetIsThrow(false);
			((ThrowObject*)collision)->SetIsDeleteStandBy(true);
		}
	}
	else
	{
		ThrowObject::OnCollisionEnter(collision);
	}
	
}
