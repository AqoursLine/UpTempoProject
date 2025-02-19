/******************************************************
* Horse.cpp	????
* ?????Fmurayama
* ?쐬???F2024/01/23
* ?ŏI?X?V???F
* ?|?[???͕`?悵?Ȃ??B??Ƃ?Ɣw?i?ɂ????̂?g??????
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"
#include "Game/Horse.h"


/****************************************************
* ??????
*****************************************************/
Horse::Horse(float x, float y, float r,bool front) : ThrowObject(x, y, r) {
	m_cnt = 0;
	m_front = front;

	if (m_front)
	{
		//?e?N?X?`???ݒ?
		m_uv.x = 0.0175f;
		m_uv.y = 0.42f;
		m_texSize.x = 0.098f;
		m_texSize.y = 0.255f;
	}
	else
	{//?e?N?X?`???ݒ?
		m_uv.x = 0.24f;
		m_uv.y = 0.553f;
		m_texSize.x = 0.115f;
		m_texSize.y = 0.31;
	}

	//?T?C?Y?ݒ?
	float aspect = m_texSize.x / m_texSize.y;
	float height = 298.0f;//test?p?Ŏ??????ۂ̃|?[????l?????????傤?ǂ????T?C?Y
	//?T?C?Y?̔䗦
	m_size = XMFLOAT2(height * aspect, height);

	//?|?W?V?????ϊ?
	b2Vec2 b2pos = Physics::ConvertDXtoB2Float2(m_pos);
	//?{?f?B?쐬
	Physics::CreateBody(&m_body, b2pos.x, b2pos.y, r, true, this);

	//?T?C?Y?ϊ?
	b2Vec2 b2size = Physics::ConvertDXtoB2Float2(m_size);
	//?????蔻????
	Physics::CreateFixture(&m_body, b2size.x, b2size.y, 0.0f);

	//?e?N?X?`??
	if(m_front)
	m_tex.Load(L"Data/Texture/HorseFront.png");
	else
	{//??????
		m_tex.Load(L"Data/Texture/HorsePole.png");
		m_tex2.Load(L"Data/Texture/HorseBack.png");
	}

	//?d??
	m_weight = WEIGHT_HEAVY;

}

/****************************************************
* ???イ????
*****************************************************/
Horse::~Horse() {

}

/****************************************************
* Update
*****************************************************/
void Horse::Update() {
	ThrowObject::Update();

	
	if (m_fixed)
	{
		if (m_cnt >= 120)
		{//?ŏ??ɔz?u?????ʒu???????̍Œ??ʒu
			m_turn *= -1;
			m_cnt = 0;
		}
		m_body->SetLinearVelocity(b2Vec2(0, 0.5 * m_turn));//?^???Ă??鐔?l???????????[???͓K??

		m_cnt++;
	}

	
}

void Horse::HoldTiming()
{
	
		b2Fixture* fixture = (m_body)->GetFixtureList();
		fixture->SetDensity(1.0f);
		fixture->SetFriction(0.3f);
		fixture->SetRestitution(0.0f);
		m_body->ResetMassData();
		m_fixed = false;

		if (!m_front && first)
		{//?w?????̏ꍇ?|?[???Ȃ??̉摜?ɐ؂???
			m_uv.x = 0.495f;
			m_uv.y = 0.435f;
			m_texSize.x = 0.095f;
			m_texSize.y = 0.255f;
			m_tex = m_tex2;
			first = false;
		}
	
}
