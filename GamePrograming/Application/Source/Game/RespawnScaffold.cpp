/******************************************************
* R_Scaffold.cpp	????
* ?????Fmurayama
* ?쐬???F2024/01/23
* ?ŏI?X?V???F
* ?|?[???͕`?悵?Ȃ??B??Ƃ?Ɣw?i?ɂ????̂?g??????
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"
#include "Game/RespawnScaffold.h"


/****************************************************
* ??????
*****************************************************/
R_Scaffold::R_Scaffold(float x, float y, float r,int pnum) : ThrowObject(x, y, r) {
	
	m_resPNum = pnum;

	m_isCollision = false;
	m_cnt = 0;
	//?e?N?X?`???ݒ?//?e?X?g?p?ݒ?
	m_uv.x = 0.0;
	m_uv.y = 0.4f;
	m_texSize.x = 1.0f;
	m_texSize.y = 0.4f;
	
	//?T?C?Y?ݒ?
	float aspect = m_texSize.x / m_texSize.y;
	float height = 100.0f;
	//?T?C?Y?̔䗦
	m_size = XMFLOAT2(height * aspect, height);

	//?|?W?V?????ϊ?
	b2Vec2 b2pos = Physics::ConvertDXtoB2Float2(m_pos);
	//?{?f?B?쐬
	Physics::CreateBody(&m_body, b2pos.x, b2pos.y, r, false, this);

	//?T?C?Y?ϊ?
	b2Vec2 b2size = Physics::ConvertDXtoB2Float2(m_size);
	//?????蔻????
	Physics::CreateFixture(&m_body, b2size.x, b2size.y, 0.0f);

	//?e?N?X?`??
	m_tex.Load(L"Data/Texture/asiba.png");//?e?X?g?p?e?N?X?`??

	//?d??
	m_weight = WEIGHT_HEAVY;

}

/****************************************************
* ???イ????
*****************************************************/
R_Scaffold::~R_Scaffold() {

}

void R_Scaffold::Update()
{
	ThrowObject::Update();
	if (m_fixed)
	{
		if (!m_isCollision)
			m_cnt++;

		if (m_cnt > 5)//?T?͓K??
		{
			b2Fixture* fixture = (m_body)->GetFixtureList();
			fixture->SetDensity(1.0f);
			fixture->SetFriction(0.3f);
			fixture->SetRestitution(0.0f);
			m_body->ResetMassData();
			m_fixed = false;
		}
	}
}



void R_Scaffold::HoldTiming()
{

	b2Fixture* fixture = (m_body)->GetFixtureList();
	fixture->SetDensity(1.0f);
	fixture->SetFriction(0.3f);
	fixture->SetRestitution(0.0f);
	m_body->ResetMassData();
	m_fixed = false;
}


void R_Scaffold::OnCollisionEnter(GameObject* collision)
{
	if (!GetIsThrow())
	{
		if (collision->CompareTag("Player")&& ((Player*)collision)->GetPlayerNum() == m_resPNum)
		{
			m_isCollision = true;
			
		}
	}
	ThrowObject::OnCollisionEnter(collision);
	
}

void R_Scaffold::OnCollisionExit(GameObject* collision)
{
	
	if (collision->CompareTag("Player") && ((Player*)collision)->GetPlayerNum() == m_resPNum)
	{
		m_isCollision = false;
		m_cnt = 0;
	}
	
}
