/******************************************************
* ThrowObject.h	スローオブジェクト
* 制作者：ミヤタジョウジ
* 作成日：2024/11/07
* 最終更新日：2024/11/07
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "DirectX/Audio.h"
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

	m_soundNum = AUDIO.LoadWaveFile("Data/Sound/SE/打撃6.wav");						
	m_breakObjectSound = AUDIO.LoadWaveFile("Data/Sound/SE/ショット7.wav");			
	m_throwSound = AUDIO.LoadWaveFile("Data/Sound/SE/剣の素振り3.wav");				
	m_throwSound2 = AUDIO.LoadWaveFile("Data/Sound/SE/剣の素振り2.wav");				
	m_throwSound3 = AUDIO.LoadWaveFile("Data/Sound/SE/剣の素振り1.wav");				
	m_collisionSound = AUDIO.LoadWaveFile("Data/Sound/SE/手足・殴る、蹴る05.wav");	
	m_collisionSound2 = AUDIO.LoadWaveFile("Data/Sound/SE/手足・殴る、蹴る09.wav");	
	m_collisionSound3 = AUDIO.LoadWaveFile("Data/Sound/SE/手足・殴る、蹴る07.wav");	

	AUDIO.SetVolume(m_soundNum, 1.0f);
	AUDIO.SetVolume(m_breakObjectSound, 1.0f);
	AUDIO.SetVolume(m_throwSound, 1.0f);
	AUDIO.SetVolume(m_throwSound2, 1.0f);
	AUDIO.SetVolume(m_throwSound3, 1.0f);
	AUDIO.SetVolume(m_collisionSound, 1.0f);
	AUDIO.SetVolume(m_collisionSound2, 1.0f);
	AUDIO.SetVolume(m_collisionSound3, 1.0f);

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

	AUDIO.PlayAudio(m_breakObjectSound, 0);

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
	m_isPlayerCollision = false;
	if(m_HitStop.IsHitStop(m_body)) { 
		return;
	}

	if (m_isDeleteStandBy) {
		SetIsDelete();
		return;
	}

	//画面外に行ったら
	if (m_pos.x <= (0.0f - m_size.x) || m_pos.x >= (SCREEN_WIDTH + m_size.x) || m_pos.y <= (0.0f - m_size.y) || m_pos.y >= (SCREEN_HEIGHT + m_size.y)) {
		SetIsDelete();
		return;
	}

	m_pos = Physics::ConvertB2toDXFloat2(m_body->GetPosition());
	m_rot = m_body->GetAngle();

	//回転していたら
	if (m_isRotation) {
		//現在の相対角度を取得
		float currentAngle = ((b2RevoluteJoint*)m_joint)->GetJointAngle();

		//フィルター更新
		b2Fixture* fixture = m_body->GetFixtureList();
		while (fixture) {
			b2Filter filter = fixture->GetFilterData();
			filter.maskBits &= ~(m_joint->GetBodyA()->GetFixtureList()->GetFilterData().categoryBits);
			fixture->SetFilterData(filter);
			fixture = fixture->GetNext();
		}


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
			b2Fixture* fixture = m_body->GetFixtureList();
			while (fixture) {
				b2Filter filter = fixture->GetFilterData();
				filter.maskBits = ~0;
				fixture->SetFilterData(filter);
				fixture = fixture->GetNext();
			}

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
	if (m_isPlayerCollision) {
		XMFLOAT2 size = XMFLOAT2(m_size.x * m_sizeAdjust, m_size.y * m_sizeAdjust);
		D3D.Draw2D(m_tex, m_pos, size, m_rot, m_uv, m_texSize, m_playerColor, PIXELMODE_SILHOUETTE);
	}

	D3D.Draw2D(m_tex, m_pos, m_size, m_rot, m_uv, m_texSize);

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
	m_throwPos = m_pos;

	switch (m_weight)
	{
	case WEIGHT_LIGHT:
		AUDIO.PlayAudio(m_throwSound, 0);
		break;
	case WEIGHT_NORMAL:
		AUDIO.PlayAudio(m_throwSound2, 0);
		break;
	case WEIGHT_HEAVY:
		AUDIO.PlayAudio(m_throwSound3, 0);
		break;
	}

	return true;
}

/****************************************************
* スローオブジェクト持つ
*****************************************************/
const bool ThrowObject::Hold(b2Body* playerBody, GameObject* player) {
	if (m_joint) {
		return false;
	}

	if (m_player) {
		return false;
	}

	HoldTiming();

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
	float rad = XMConvertToRadians((atan >= XMConvertToRadians(90.0f)) ? 270.0f : -90.0f);
	m_targetAngle = rad - atan;

	m_isRotation = true;
	m_isThrowed = false;

	return true;
}

/****************************************************
* スローオブジェクト投げられたモノに当たった
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
* スローオブジェクトメンバ変数PlayerColorセッター
*****************************************************/
void ThrowObject::SetPlayerColor(const XMFLOAT4& playerColor) {
	m_playerColor = playerColor;
	m_isPlayerCollision = true;
}


/****************************************************
* スローオブジェクト当たり判定
*****************************************************/
void ThrowObject::OnCollisionEnter(GameObject* collision) {

	if (m_isThrowed) {
		if ((collision->CompareTag("Field") || collision->CompareTag("Ground"))) {

			AUDIO.PlayAudio(m_soundNum, 0);
			int damage = static_cast<int>(m_body->GetFixtureList()->GetDensity() * 5);
			((FieldObject*)collision)->Attack(damage);

			m_isDeleteStandBy = true;
			m_isThrowed = false;
		}

		if ((collision->CompareTag("Player")) && collision != m_player) {

			HitPlayer((Player*)collision);//デバフとか
													
			b2Vec2 ToPlayerApplyImpact = b2Vec2(m_CollectionValue * m_weight,-m_CollectionValue * m_weight);

			// 右側から当たったらXベクトルにマイナスをかける
			if (m_pos.x > ((Player*)collision)->GetPos().x) {
				ToPlayerApplyImpact.x *= -1;
			}

			// 12/03追加(仙波）
			((Player*)collision)->ApplyImpact(ToPlayerApplyImpact, m_weight);


			//ヒットストップフラグを立てる
			m_HitStop.SetIsHitStop(true, m_weight * 5);


			//重さによる音
			switch (m_weight)
			{
			case WEIGHT_LIGHT:
				AUDIO.PlayAudio(m_collisionSound, 0);
				break;
			case WEIGHT_NORMAL:
				AUDIO.PlayAudio(m_collisionSound2, 0);
				break;
			case WEIGHT_HEAVY:
				AUDIO.PlayAudio(m_collisionSound3, 0);
				break;
			}

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


