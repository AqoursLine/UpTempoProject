/******************************************************
* FieldObject.h	フィールドオブジェクト
* 制作者：ミヤタジョウジ
* 作成日：2024/11/07
* 最終更新日：2024/11/07
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "Game/FieldObject.h"

/****************************************************
* フィールドオブジェクト初期化
*****************************************************/
FieldObject::FieldObject(FIELDOBJECTTYPE type, float x, float y, float w, float h, float r) {
	m_type = type;

	m_pos = XMFLOAT2(x, y);
	m_rot = r;

	b2Vec2 pos = Physics::ConvertDXtoB2Float2(m_pos);
	Physics::CreateBody(&m_body, pos.x, pos.y, r, false, this);

	m_size = XMFLOAT2(w, h);
	b2Vec2 size = Physics::ConvertDXtoB2Float2(m_size);

	Physics::CreateFixture(&m_body, size.x, size.y);

	float density = 1.0f;

	switch (m_type) {
		case FIELDOBJECTTYPE_NORMAL:
			density = 1.0f;
			break;
		case FIELDOBJECTTYPE_HEAVY:
			density = 2.0f;
			break;
		case FIELDOBJECTTYPE_LIGHT:
			density = 0.5f;
			break;
	}

	//密度を変更
	b2Fixture* fixture = m_body->GetFixtureList();
	if (fixture) {
		fixture->SetDensity(density);
		m_body->ResetMassData();
	}

	//タグセット
	SetTag("FieldObject");

	//テクスチャ
	m_tex.Load("Data/Texture/wooden_box.png");
}

/****************************************************
* フィールドオブジェクト終了
*****************************************************/
FieldObject::~FieldObject() {
}

/****************************************************
* フィールドオブジェクト更新
*****************************************************/
void FieldObject::Update() {
	m_pos = Physics::ConvertB2toDXFloat2(m_body->GetPosition());
	m_rot = m_body->GetAngle();
}

/****************************************************
* フィールドオブジェクト描画
*****************************************************/
void FieldObject::Draw() {
	D3D.Draw2D(m_tex, m_pos.x, m_pos.y, m_size.x, m_size.y, m_rot, 0.2f, 0.3f, 0.6f, 0.6f);

}

/****************************************************
* フィールドオブジェクト投げる
*****************************************************/
void FieldObject::Throw(float vx, float vy) {
	Physics::GetWorld()->DestroyJoint(m_joint);
	m_body->ApplyLinearImpulseToCenter(b2Vec2(vx, vy), true);
}

/****************************************************
* フィールドオブジェクト持つ
*****************************************************/
void FieldObject::Hold(b2Body* playerBody) {
	b2WeldJointDef weldJointDef;
	weldJointDef.bodyA = playerBody;
	weldJointDef.bodyB = m_body;
	weldJointDef.localAnchorA.Set(0.0f, 0.0f);
	weldJointDef.localAnchorB.Set(0.0f, 0.0f);
	weldJointDef.referenceAngle = 0.0f;
	m_joint = (b2WeldJoint*)Physics::GetWorld()->CreateJoint(&weldJointDef);

	m_body->SetType(b2_dynamicBody);
}
