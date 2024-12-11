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
#include "Game/Camera.h"

/****************************************************
* スローオブジェクト初期化
*****************************************************/
ThrowObject::ThrowObject(float x, float y, float r) : m_pos(XMFLOAT2(x, y)), m_rot(r) {
	m_ApplyImpact = {20.0f, -20.0f};

	SetTag("ThrowObject");
}

/****************************************************
* スローオブジェクトデストラクタ
*****************************************************/
ThrowObject::~ThrowObject() {
	b2World* world = Physics::GetWorld();
	b2JointEdge* jointEdge = m_body->GetJointList();
	while (jointEdge) {
		b2Joint* joint = jointEdge->joint;
		jointEdge = jointEdge->next;
		world->DestroyJoint(joint);
	}
	world->DestroyBody(m_body);
	if (m_isRotation) {
		jointEdge = m_revBody->GetJointList();
		while (jointEdge) {
			b2Joint* joint = jointEdge->joint;
			jointEdge = jointEdge->next;
			world->DestroyJoint(joint);
		}
		world->DestroyBody(m_revBody);
	}
}

/****************************************************
* スローオブジェクト終了
*****************************************************/
void ThrowObject::Finalize() {
}

/****************************************************
* スローオブジェクト更新
*****************************************************/
void ThrowObject::Update() {
	if(m_HitStop.IsHitStop(m_body))
	{ 
		return;
	}

	if (m_isDeleteStandBy) {
		m_isDelete = true;
		return;
	}

	m_pos = Physics::ConvertB2toDXFloat2(m_body->GetPosition());
	m_rot = m_body->GetAngle();

	//回転していたら
	if (m_isRotation) {
		//現在の相対角度を取得
		float currentAngle = ((b2RevoluteJoint*)m_joint)->GetJointAngle();

		//フィルター更新
		b2Filter filter = m_body->GetFixtureList()->GetFilterData();
		filter.maskBits &= ~(m_joint->GetBodyA()->GetFixtureList()->GetFilterData().categoryBits);
		m_body->GetFixtureList()->SetFilterData(filter);


		//ターゲット角度よりも回転角度が大きければ止める
		if (fabsf(currentAngle) >= fabsf(m_targetAngle)) {
			//保存してあるボディを取得
			b2Body* bodyA = m_joint->GetBodyA();
			b2Body* bodyB = m_revJoint->GetBodyB();

			//ジョイントを削除
			Physics::GetWorld()->DestroyJoint(m_joint);
			m_joint = nullptr;
			Physics::GetWorld()->DestroyJoint(m_revJoint);
			m_revJoint = nullptr;
			Physics::GetWorld()->DestroyBody(m_revBody);
			m_revBody = nullptr;

			//フィルター初期化
			b2Filter filter = m_body->GetFixtureList()->GetFilterData();
			filter.maskBits = ~0;
			m_body->GetFixtureList()->SetFilterData(filter);

			//ジョイントの情報初期化
			b2WeldJointDef jointDef;
			jointDef.bodyA = bodyA;
			jointDef.bodyB = bodyB;
			jointDef.localAnchorA.Set(0.0f, 0.0f);
			jointDef.localAnchorB = bodyB->GetLocalPoint(bodyA->GetPosition());
			jointDef.referenceAngle = bodyB->GetAngle() - bodyA->GetAngle();

			//ジョイント作成
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
	if (m_isRotation || m_isThrowed) {
		return false;
	}
	Physics::GetWorld()->DestroyJoint(m_joint);
	m_joint = nullptr;
	m_targetAngle = 0.0f;
	m_body->ApplyLinearImpulseToCenter(b2Vec2(vx, vy), true);
	m_isThrowed = true;

	return true;
}

/****************************************************
* スローオブジェクト持つ
*****************************************************/
const bool ThrowObject::Hold(b2Body* playerBody, GameObject* player) {
	if (m_joint) {
		return false;
	}

	m_player = player;

	b2RevoluteJointDef jointDef;

	//回転用ボディ作成
	b2Vec2 pos = m_body->GetPosition();
	Physics::CreateBody(&m_revBody, pos.x, pos.y, 0.0f, true, this);
	b2Vec2 size = Physics::ConvertDXtoB2Float2(m_size);
	Physics::CreateFixture(&m_revBody, size.x, size.y, 1.0f, 0.3f, 0.0f, true);

	//プレイヤーと回転用ボディをジョイント
	//ジョイントするボディを設定
	jointDef.bodyA = playerBody;
	jointDef.bodyB = m_revBody;

	//ローカルアンカー設定
	jointDef.localAnchorA.Set(0.0f, 0.0f);
	jointDef.localAnchorB = jointDef.bodyB->GetLocalPoint(jointDef.bodyA->GetPosition());
	//モーター有効化
	jointDef.enableMotor = true;
	jointDef.maxMotorTorque = 10000.0f;

	//ボディAからボディBへのベクトル
	b2Vec2 direction = m_body->GetPosition() - jointDef.bodyA->GetPosition();
	//正規化
	if (direction.Length() > 0.0f) {
		direction *= (1.0f / direction.Length());
	}
	//外積で回転方向を決定
	float cross = jointDef.bodyA->GetWorldVector(b2Vec2(0.0f, -1.0f)).x * direction.y - jointDef.bodyA->GetWorldVector(b2Vec2(0.0f, -1.0f)).y * direction.x;
	jointDef.motorSpeed = XMConvertToRadians(90) * (cross >= 0 ? -1.0f : 1.0f) * 2.0f;

	//ボディ同士の当たり判定を無効
	jointDef.collideConnected = false;

	//ジョイント作成
	m_joint = Physics::GetWorld()->CreateJoint(&jointDef);

	//回転用ボディと投げるモノをジョイント
	b2RevoluteJointDef revJointDef;
	//ジョイントするボディを設定
	revJointDef.bodyA = m_revBody;
	revJointDef.bodyB = m_body;
	//ローカルアンカー設定
	revJointDef.localAnchorA.Set(0.0f, 0.0f);
	revJointDef.localAnchorB.Set(0.0f, 0.0f);
	//ボディ同士の当たり判定を無効
	revJointDef.collideConnected = false;

	m_revJoint = Physics::GetWorld()->CreateJoint(&revJointDef);

	//ボディタイプを動的に設定
	m_body->SetType(b2_dynamicBody);

	//現在の角度から真上までの相対角度
	float atan = atan2f(direction.y, direction.x);
	float rad = XMConvertToRadians((atan >= XMConvertToRadians(90)) ? 270 : -90);
	m_targetAngle = rad - atan;

	m_isRotation = true;
	m_isThrowed = false;

	return true;
}

/****************************************************
* スローオブジェクトダメージ
*****************************************************/
void ThrowObject::Inpact(WEIGHT weight) {
	if (weight >= m_weight) {
		m_isDeleteStandBy = true;
		if (m_player) {
			((Player*)m_player)->SetNullHoldObject();
		}
	}
}

/****************************************************
* スローオブジェクト当たり判定
*****************************************************/
void ThrowObject::OnCollisionEnter(GameObject* collision) {

	if (m_isThrowed) {
		if ((collision->CompareTag("Field") || collision->CompareTag("Ground"))) {
			int damage = m_body->GetFixtureList()->GetDensity() * 5;
			((FieldObject*)collision)->Attack(damage);

			m_isDeleteStandBy = true;
			m_isThrowed = false;
		}

		if ((collision->CompareTag("Player")) && collision != m_player) {
			b2Vec2 ToPlayerApplyImpact;
			float CollectionValue = 20.0f;
			ToPlayerApplyImpact = b2Vec2(CollectionValue * m_weight, -CollectionValue * m_weight);

			// 右側から当たったらXベクトルにマイナスをかける
			if (m_pos.x > ((Player*)collision)->GetPos().x) {
				ToPlayerApplyImpact.x *= -1;
			}

			//12/03追加(仙波）
			((Player*)collision)->ApplyImpact(ToPlayerApplyImpact);


			//ヒットストップフラグを立てる
			m_HitStop.SetIsHitStop(true, m_weight * 5);

			m_isDeleteStandBy = true;
			m_isThrowed = false;
		}

		if (collision->CompareTag("ThrowObject")) {
			m_isDeleteStandBy = true;
			m_isThrowed = false;
			((ThrowObject*)collision)->Inpact(m_weight);
		}
	}
}
