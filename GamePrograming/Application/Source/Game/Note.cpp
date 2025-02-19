/******************************************************
* Note.cpp	?m?[?g
* ?????F?J???}?^?g?E
* ?쐬???F2024/12/05
* ?ŏI?X?V???F2024/12/05
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"
#include "Game/Note.h"


/****************************************************
* ?m?[?g??????
*****************************************************/
Note::Note(float x, float y, float r) : ThrowObject(x, y, r) {
	//?e?N?X?`???ݒ?
	m_uv.x = 0.3f;
	m_uv.y = 0.1f;
	m_texSize.x = 0.53f;
	m_texSize.y = 0.7f;

	//?T?C?Y?ݒ?
	float aspect = m_texSize.x / m_texSize.y;
	float height = 80;
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
	m_tex.Load(L"Data/Texture/note.png");

	//?d??
	m_weight = WEIGHT_LIGHT;
}

/****************************************************
* ?m?[?g?I??
*****************************************************/
Note::~Note() {}

/****************************************************
* ?m?[?g?X?V
*****************************************************/
void Note::Update() {
	ThrowObject::Update();

	if (m_HitStop.GetIsHitStop()) {
		return;
	}

	//???x?????????ɂȂ???????
	if (m_body->GetLinearVelocity().y > 0) {
		//?{??J??

	}
}
