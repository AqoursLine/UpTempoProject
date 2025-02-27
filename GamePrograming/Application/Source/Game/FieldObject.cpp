/******************************************************
* FieldObject.cpp		フィールドオブジェクト
* 制作者：ミヤタジョウジ
* 作成日：2024/11/14
* 最終更新日：2024/11/14
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "Game/FieldObject.h"
#include "Camera.h"

/****************************************************
* フィールドオブジェクト初期化
*****************************************************/
FieldObject::FieldObject(const XMFLOAT2& pos, float rot, const XMFLOAT2& size, const XMFLOAT2& offset, int uvNum, const FIELD_DIRECTION fieldDirection) : m_pos(pos), m_rot(rot), m_size(size), m_fieldDirection(fieldDirection){
	if (m_pos.x == 14) {
		m_pos.x = (m_pos.x + 1) * offset.x - (size.x * 0.5f);
	} else {
		m_pos.x = m_pos.x * offset.x + (size.x * 0.5f);
	}
	m_pos.y = m_pos.y * offset.y +(size.y * 0.5f);

	b2Vec2 b2pos = Physics::ConvertDXtoB2Float2(m_pos);
	Physics::CreateBody(&m_body, b2pos.x, b2pos.y, m_rot, false, this);

	b2Vec2 b2size = Physics::ConvertDXtoB2Float2(m_size);
	Physics::CreateFixture(&m_body, b2size.x, b2size.y);

	SetTag("Field");

	m_tex.emplace_back(new Texture());
	m_tex.back()->Load(L"Data/Texture/DividedOutFrame.png");
	m_tex.emplace_back(new Texture());
	m_tex.back()->Load(L"Data/Texture/CrackDividedOutFrame.png");



	m_texSize = XMFLOAT2(245.0f, 245.0f);

	m_uvSize.x = 1.0f / 10;
	m_uvSize.y = 1.0f / 5;

	m_uvPos.x = (uvNum % 10) * m_uvSize.x;
	m_uvPos.y = (uvNum / 10) * m_uvSize.y;

	m_maxHp = m_hp = 5;
}

/****************************************************
* フィールドオブジェクト終了
*****************************************************/
FieldObject::~FieldObject() {
	Physics::GetWorld()->DestroyBody(m_body);
}

/****************************************************
* フィールドオブジェクト更新
*****************************************************/
void FieldObject::Update() {
	if (m_hp <= m_maxHp * 0.5f) {
		m_texNum = 1;
	}

	if (m_hp <= 0) {
		SetIsDelete();
	}
}

/****************************************************
* フィールドオブジェクト描画
*****************************************************/
void FieldObject::Draw() {
	D3D.Draw2D(*(m_tex[m_texNum]), m_pos, m_texSize, 0.0f, m_uvPos, m_uvSize);
}

/****************************************************
* ダメージ
*****************************************************/
void FieldObject::Attack(int attack) {
	switch (m_fieldDirection)
	{
	case TOP:
		Camera::Shake(XMFLOAT2(0.0f, -10.0f), 30);
		break;
	case LEFT:
		Camera::Shake(XMFLOAT2(10.0f, 0.0f), 30);
		break;
	case RIGHT:
		Camera::Shake(XMFLOAT2(10.0f, 0.0f), 30);
		break;
	case BOTTOM:
		Camera::Shake(XMFLOAT2(0.0f, -10.0f), 30);
		break;
	case LEFTCORNER:
		Camera::Shake(XMFLOAT2(10.0f, -10.0f), 30);
		break;
	case RIGHTCORNER:
		Camera::Shake(XMFLOAT2(10.0f, -10.0f), 30);
		break;
	default:
		break;
	}
	//Camera::Shake(XMFLOAT2(10.0f, -10.0f), 30);
	m_hp -= attack;
}


