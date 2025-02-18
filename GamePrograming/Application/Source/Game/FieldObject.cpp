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
FieldObject::FieldObject(const XMFLOAT2& pos, float rot, const XMFLOAT2& size, const std::wstring& fileName, const XMFLOAT2& texPos) : m_pos(pos), m_rot(rot), m_size(size), m_texPos(texPos) {
	b2Vec2 b2pos = Physics::ConvertDXtoB2Float2(m_pos);
	Physics::CreateBody(&m_body, b2pos.x, b2pos.y, m_rot, false, this);

	b2Vec2 b2size = Physics::ConvertDXtoB2Float2(m_size);
	Physics::CreateFixture(&m_body, b2size.x, b2size.y);

	SetTag("Field");

	std::wstring filePath = L"Data/Texture/DividedOutFrame/" + fileName + L".png";

	m_tex.Load(filePath);

	m_texSize = XMFLOAT2(194.5132, 194.5132);

	m_hp = 10;
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
	if (m_hp <= 0) {
		SetIsDelete();
	}
}

/****************************************************
* フィールドオブジェクト描画
*****************************************************/
void FieldObject::Draw() {
	D3D.Draw2D(m_tex, m_texPos, m_texSize);
}

/****************************************************
* ダメージ
*****************************************************/
void FieldObject::Attack(int attack) {
	switch (m_fieldDirection)
	{
	case TOP:
		break;
	case LEFT:
		break;
	case RIGHT:
		break;
	case BOTTOM:
		break;
	case CORNER:
		break;
	default:
		break;
	}
	Camera::Shake(XMFLOAT2(10.0f, -10.0f), 30);
	m_hp -= attack;
}
