#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "Game/Controller.h"

#include "Game/Net.h"

Net::Net()
{
	//初期設定
	m_pos = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	m_rot = 0.0f;
	m_size = XMFLOAT2(125.0f * 2.0f, 100.0f * 2.0f);

	//座標変換
	b2Vec2 pos = Physics::ConvertDXtoB2Float2(m_pos);
	//ボディ作成
	Physics::CreateBody(&m_body, pos.x, pos.y, m_rot, true, this);

	//座標変換
	b2Vec2 size = Physics::ConvertDXtoB2Float2(m_size);

	//当たり判定作成
	Physics::CreateFixture(&m_body, size.x, size.y, true);

	//回転無効
	m_body->SetFixedRotation(true);

	//テクスチャロード
	m_tex.Load("Data/Texture/sight.png");

	SetTag("Sight");
}

Net::~Net()
{
}

void Net::Update()
{


	if (!m_IsThrow) {

		//座標更新
		m_pos = Physics::ConvertB2toDXFloat2(m_body->GetPosition());
		m_rot = m_body->GetAngle();

		//左右移動
		if (CTRL.GetKeyboardPress(DIK_LEFT)) {
			b2Vec2 vel = m_body->GetLinearVelocity();
			vel.x = -5;
			m_body->SetLinearVelocity(vel);
		}
		else if (CTRL.GetKeyboardPress(DIK_RIGHT)) {
			b2Vec2 vel = m_body->GetLinearVelocity();
			vel.x = 5;
			m_body->SetLinearVelocity(vel);
		}
		else if (CTRL.GetKeyboardPress(DIK_UP)) {
			b2Vec2 vel = m_body->GetLinearVelocity();
			vel.y = -5;
			m_body->SetLinearVelocity(vel);
		}
		else if (CTRL.GetKeyboardPress(DIK_DOWN)) {
			b2Vec2 vel = m_body->GetLinearVelocity();
			vel.y = 5;
			m_body->SetLinearVelocity(vel);
		}
		else {
			b2Vec2 vel = m_body->GetLinearVelocity();
			vel.x = 0;
			vel.y = 0;
			m_body->SetLinearVelocity(vel);
		}

		if (CTRL.GetKeyboardTrigger(DIK_J)) {
			ThrowNet();
		}
	}
}

void Net::Draw()
{

	//dx座標で描画
	D3D.Draw2D(m_tex, m_pos.x, m_pos.y, m_size.x, m_size.y, m_rot, 0.0f, 0.0f, 1.0f, 1.0f);

}

void Net::ThrowNet()
{
	if (!m_IsThrow) {


		m_IsThrow = true;
		SetTag("Net");


		m_tex.Load("Data/Texture/RedNet.png");
	}
	
}

void Net::SetNetPos(XMFLOAT2 Pos)
{
	//座標更新
	m_pos = Physics::ConvertB2toDXFloat2(m_body->GetPosition());
	m_rot = m_body->GetAngle();

	b2Vec2 vel = m_body->GetLinearVelocity();
	vel.x = Pos.x;
	vel.y = Pos.y;
	m_body->SetLinearVelocity(vel);

	//m_pos = Pos;
	//b2Vec2 BodyPos;
	//BodyPos = Physics::ConvertDXtoB2Float2(m_pos);
	//
	//m_body->SetTransform(BodyPos, m_rot);
}



void Net::VelReset()
{
	b2Vec2 vel = m_body->GetLinearVelocity();
	vel.x = 0;
	vel.y = 0;
	m_body->SetLinearVelocity(vel);
}

void Net::OnCollisionEnter(GameObject* collision)
{
	if (m_IsThrow) {

		if (collision->CompareTag("Player")) {
			m_IsThrow = false;

			SetTag("Sight");

			m_tex.Load("Data/Texture/sight.png");
		}
	}
	
}
