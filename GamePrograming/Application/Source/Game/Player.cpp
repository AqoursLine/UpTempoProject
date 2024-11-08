/******************************************************
* Player.cpp		プレイヤー
* 制作者：ミヤタジョウジ
* 作成日：2024/11/05
* 最終更新日：2024/11/05
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "Game/Controller.h"

#include "Game/Player.h"

/****************************************************
* プレイヤー初期化
*****************************************************/
Player::Player() {
	//初期設定
	m_pos = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	m_rot = 0.0f;
	m_size = XMFLOAT2(100.0f, 100.0f);

	//座標変換
	b2Vec2 pos = Physics::ConvertDXtoB2Float2(m_pos);
	//ボディ作成
	Physics::CreateBody(&m_body, pos.x, pos.y, m_rot, true, this);

	//座標変換
	b2Vec2 size = Physics::ConvertDXtoB2Float2(m_size);
	//当たり判定作成
	Physics::CreateFixture(&m_body, size.x, size.y, false);

	//回転無効
	m_body->SetFixedRotation(true);

	//テクスチャロード
	m_tex.Load("Data/Texture/icon.png");

	m_gamePadNum = CTRL.GetGamepadHandle();

	m_isJump = false;

	m_net = new Net;

	SetTag("Player");
}

/****************************************************
* プレイヤー終了
*****************************************************/
Player::~Player() {
	delete m_net;
}

/****************************************************
* プレイヤー更新
*****************************************************/
void Player::Update() {
	//座標更新
	m_pos = Physics::ConvertB2toDXFloat2(m_body->GetPosition());
	m_rot = m_body->GetAngle();

	//左右移動
	//ゲームパッドが接続されているか
	if (m_gamePadNum >= 0) {
		b2Vec2 vel = m_body->GetLinearVelocity();
		vel.x = CTRL.GetLeftStickHorizontal(m_gamePadNum) * 0.01;
		m_body->SetLinearVelocity(vel);
	} else {
		if (CTRL.GetKeyboardPress(DIK_A)) {
			b2Vec2 vel = m_body->GetLinearVelocity();
			vel.x = -5;
			m_body->SetLinearVelocity(vel);

		} else if (CTRL.GetKeyboardPress(DIK_D)) {
			b2Vec2 vel = m_body->GetLinearVelocity();
			vel.x = 5;
			m_body->SetLinearVelocity(vel);
		}
		else if (CTRL.GetKeyboardPress(DIK_W)) {
			b2Vec2 vel = m_body->GetLinearVelocity();
			vel.y = -5;
			m_body->SetLinearVelocity(vel);
		}
		else if (CTRL.GetKeyboardPress(DIK_S)) {
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
	}

	m_net->Update();

	// 網が投げられていたら
	if (m_net->IsThrow()) {

		if (CTRL.GetKeyboardPress(DIK_K)) {
			PullNet();
		}

	}
}

/****************************************************
* プレイヤー描画
*****************************************************/
void Player::Draw() {
	//dx座標で描画
	D3D.Draw2D(m_tex, m_pos.x, m_pos.y, m_size.x, m_size.y, m_rot, 0.0f, 0.0f, 1.0f, 1.0f);
	m_net->Draw();
}


void Player::PullNet()
{
	// ネットからプレイヤーに向けてのベクトルを計算
	XMFLOAT2 Vec;
	Vec.x = m_pos.x - m_net->GetNetPos().x;
	Vec.y = m_pos.y - m_net->GetNetPos().y;

	// Float2をVECTOR型に変換し、正規化。その後、Float2に戻す。
	XMVECTOR vec = XMLoadFloat2(&Vec);
	vec = XMVector2Normalize(vec);
	XMStoreFloat2(&Vec, vec);
	
	// 正規化したベクトルにスカラをかける
	Vec.x *= 5.0f;
	Vec.y *= 5.0f;

	m_net->SetNetPos(Vec);
}

/****************************************************
* プレイヤー当たり判定
*****************************************************/
void Player::OnCollisionEnter(GameObject* collision) {
	if (collision->CompareTag("Net")) {
		
	}
}
