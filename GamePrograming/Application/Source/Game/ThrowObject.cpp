/******************************************************
* ThrowObject.h	フィールドオブジェクト
* 制作者：ミヤタジョウジ
* 作成日：2024/11/07
* 最終更新日：2024/11/07
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"
#include "Game/ThrowObject.h"

/****************************************************
* フィールドオブジェクト初期化
*****************************************************/
ThrowObject::ThrowObject(FIELDOBJECTTYPE type, float x, float y, float w, float h, float r) {
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
	SetTag("ThrowObject");

	//テクスチャ
	m_tex.Load("Data/Texture/wooden_box.png");

	//回転角度初期化
	m_isRotation = false;
}

/****************************************************
* フィールドオブジェクト終了
*****************************************************/
ThrowObject::~ThrowObject() {
}

/****************************************************
* フィールドオブジェクト更新
*****************************************************/
void ThrowObject::Update() {
	m_pos = Physics::ConvertB2toDXFloat2(m_body->GetPosition());
	m_rot = m_body->GetAngle();

	//回転していたら
	if (m_isRotation) {
		//現在の相対角度を取得
		float currentAngle = ((b2RevoluteJoint*)m_joint)->GetJointAngle();

		//ターゲット角度よりも回転角度が大きければ止める
		if (fabsf(currentAngle) >= fabsf(m_targetAngle)) {
			//保存してあるボディを取得
			b2Body* bodyA = m_joint->GetBodyA();
			b2Body* bodyB = m_joint->GetBodyB();

			//ジョイントを削除
			Physics::GetWorld()->DestroyJoint(m_joint);

			//ジョイントの情報初期化
			b2WeldJointDef jointDef;
			jointDef.bodyA = bodyA;
			jointDef.bodyB = bodyB;
			jointDef.localAnchorA.Set(0.0f, 0.0f);
			jointDef.localAnchorB = bodyB->GetLocalPoint(bodyA->GetPosition());
			jointDef.referenceAngle = bodyB->GetAngle() - bodyA->GetAngle();

			//ジョイント削除
			m_joint = Physics::GetWorld()->CreateJoint(&jointDef);

			//回転フラグを削除
			m_isRotation = false;
		}

	}
}

/****************************************************
* フィールドオブジェクト描画
*****************************************************/
void ThrowObject::Draw() {
	D3D.Draw2D(m_tex, m_pos.x, m_pos.y, m_size.x, m_size.y, m_rot, 0.2f, 0.3f, 0.6f, 0.6f);

}

/****************************************************
* フィールドオブジェクト投げる
*****************************************************/
void ThrowObject::Throw(float vx, float vy) {
	Physics::GetWorld()->DestroyJoint(m_joint);
	m_joint = nullptr;
	m_body->ApplyLinearImpulseToCenter(b2Vec2(vx, vy), true);
}

/****************************************************
* フィールドオブジェクト持つ
*****************************************************/
void ThrowObject::Hold(b2Body* playerBody) {
	b2RevoluteJointDef jointDef;
	//ジョイントするボディを設定
	jointDef.bodyA = playerBody;
	jointDef.bodyB = m_body;

	//ローカルアンカー設定
	jointDef.localAnchorA.Set(0.0f, 0.0f);
	jointDef.localAnchorB = jointDef.bodyB->GetLocalPoint(jointDef.bodyA->GetPosition());
	//モーター有効化
	jointDef.enableMotor = true;
	jointDef.maxMotorTorque = 100.0f;

	//ボディAからボディBへのベクトル
	b2Vec2 direction = jointDef.bodyB->GetPosition() - jointDef.bodyA->GetPosition();
	//正規化
	if (direction.Length() > 0.0f) {
		direction *= (1.0f / direction.Length());
	}

	//角度による速度調整
	float speedScale = 1.0f + direction.y;

	//外積で回転方向を決定
	float cross = jointDef.bodyA->GetWorldVector(b2Vec2(0.0f, 1.0f)).x * direction.y - jointDef.bodyA->GetWorldVector(b2Vec2(0.0f, 1.0f)).y * direction.x;
	jointDef.motorSpeed = XMConvertToRadians(90) * speedScale * (cross >= 0 ? 1.0f : -1.0f);

	//ジョイント作成
	m_joint = Physics::GetWorld()->CreateJoint(&jointDef);

	//ボディタイプを動的に設定
	m_body->SetType(b2_dynamicBody);

	//現在の角度から真上までの相対角度
	m_targetAngle = XMConvertToRadians(-90) - atan2f(direction.y, direction.x);

	float debugAngle = ((b2RevoluteJoint*)m_joint)->GetJointAngle();

	m_isRotation = true;
}
