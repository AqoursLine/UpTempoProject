/******************************************************
* ThrowObject.h	スローオブジェクト
* 制作者：ミヤタジョウジ
* 作成日：2024/11/07
* 最終更新日：2024/11/07
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"
#include "Game/ThrowObject.h"
#include "Game/FieldObject.h"
#include "Game/Player.h"

/****************************************************
* スローオブジェクト初期化
*****************************************************/

/****************************************************
* スローオブジェクト終了
*****************************************************/
ThrowObject::~ThrowObject() {
}

/****************************************************
* スローオブジェクト更新
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
* スローオブジェクト描画
*****************************************************/
void ThrowObject::Draw() {
	D3D.Draw2D(m_tex, m_pos.x, m_pos.y, m_size.x, m_size.y, m_rot, m_uv.x, m_uv.y, m_texSize.x, m_texSize.y);

}

/****************************************************
* スローオブジェクト投げる
*****************************************************/
bool ThrowObject::Throw(float vx, float vy) {
	if (m_isRotation) {
		return false;
	}
	Physics::GetWorld()->DestroyJoint(m_joint);
	m_joint = nullptr;
	m_body->ApplyLinearImpulseToCenter(b2Vec2(vx, vy), true);
	m_isThrowed = true;

	return true;
}

/****************************************************
* スローオブジェクト持つ
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
	float cross = jointDef.bodyA->GetWorldVector(b2Vec2(0.0f, -1.0f)).x * direction.y - jointDef.bodyA->GetWorldVector(b2Vec2(0.0f, -1.0f)).y * direction.x;
	jointDef.motorSpeed = XMConvertToRadians(90) * speedScale * (cross >= 0 ? -1.0f : 1.0f) * 5.0f;

	//ジョイント作成
	m_joint = Physics::GetWorld()->CreateJoint(&jointDef);

	//ボディタイプを動的に設定
	m_body->SetType(b2_dynamicBody);

	//現在の角度から真上までの相対角度
	float atan = atan2f(direction.y, direction.x);
	float rad = XMConvertToRadians((atan >= XMConvertToRadians(90)) ? 270 : -90);
	m_targetAngle = rad - atan;

	m_isRotation = true;
}

/****************************************************
* スローオブジェクト当たり判定
*****************************************************/
void ThrowObject::OnCollisionEnter(GameObject* collision) {
	if (m_isThrowed && (collision->CompareTag("Field") || collision->CompareTag("Ground"))) {
		int damage = m_body->GetFixtureList()->GetDensity() * 5;
		((FieldObject*)collision)->Attack(damage);

		

		m_isThrowed = false;
	}

	if (m_isThrowed && (collision->CompareTag("Player")))
	{
		b2Vec2 ToPlayerApplyImpact = m_ApplyImpact;

		// 右側から当たったらXベクトルにマイナスをかける
		if (m_pos.x > ((Player*)collision)->GetPos().x) {
			ToPlayerApplyImpact.x *= -1;
		}

			//12/03追加(仙波）
		((Player*)collision)->ApplyImpact(ToPlayerApplyImpact);
		
		
		//ヒットストップフラグを立てる
	}

}
