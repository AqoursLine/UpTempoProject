/******************************************************
* FieldObject.h	フィールドオブジェクト
* 制作者：ミヤタジョウジ
* 作成日：2024/11/07
* 最終更新日：2024/11/07
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
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

	//回転角度初期化
	m_isRotation = false;
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

	if (m_isRotation) {
		float targetAngle = XMConvertToRadians(90);
		float tolerance = XMConvertToRadians(5);
		float currentAngle = ((b2RevoluteJoint*)m_joint)->GetJointAngle();

		if (fabs(fabs(currentAngle) - targetAngle) <= tolerance) {
			b2Body* bodyA = m_joint->GetBodyA();
			b2Body* bodyB = m_joint->GetBodyB();

			Physics::GetWorld()->DestroyJoint(m_joint);

			b2WeldJointDef jointDef;
			jointDef.bodyA = bodyA;
			jointDef.bodyB = bodyB;
			jointDef.localAnchorA.Set(0.0f, 0.0f);
			jointDef.localAnchorB = bodyB->GetLocalPoint(bodyA->GetPosition());
			jointDef.referenceAngle = bodyB->GetAngle() - bodyA->GetAngle();

			m_joint = Physics::GetWorld()->CreateJoint(&jointDef);

			m_isRotation = false;
		}

	}
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
	b2RevoluteJointDef jointDef;
	jointDef.bodyA = playerBody;
	jointDef.bodyB = m_body;

	jointDef.localAnchorA.Set(0.0f, 0.0f);
	jointDef.localAnchorB = jointDef.bodyB->GetLocalPoint(jointDef.bodyA->GetPosition());
	jointDef.enableMotor = true;
	float angle = atan2f(jointDef.bodyB->GetPosition().y - jointDef.bodyA->GetPosition().y, jointDef.bodyB->GetPosition().x - jointDef.bodyA->GetPosition().x);
	angle = ((int)XMConvertToDegrees(angle) + 360) % 360;
	jointDef.motorSpeed = XMConvertToRadians(angle) - XMConvertToRadians(90);
	jointDef.maxMotorTorque = 100.0f;

	m_joint = Physics::GetWorld()->CreateJoint(&jointDef);

	m_body->SetType(b2_dynamicBody);

	m_isRotation = true;
}
