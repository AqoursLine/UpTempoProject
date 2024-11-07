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
	Physics::CreateFixture(&m_body, size.x, size.y);

	//回転無効
	m_body->SetFixedRotation(true);

	//テクスチャロード
	m_tex.Load("Data/Texture/player.png");

	m_gamePadNum = CTRL.GetGamepadHandle();

	m_isJump = false;
}

/****************************************************
* プレイヤー終了
*****************************************************/
Player::~Player() {

}

/****************************************************
* プレイヤー更新
*****************************************************/
void Player::Update() {
	//ボディの座標をDX座標に変換
	m_pos = Physics::ConvertB2toDXFloat2(m_body->GetPosition());
	m_rot = m_body->GetAngle();

	//左右移動
	//ゲームパッドが接続されているか
	if (m_gamePadNum >= 0) {
		//現在の速度を取得(ｙ方向の速度はそのまま使いたい為)
		b2Vec2 vel = m_body->GetLinearVelocity();
		//パッドの角度を補正して速度に代入
		vel.x = CTRL.GetLeftStickHorizontal(m_gamePadNum) * 0.01;
		//速度を変更
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
		} else {
			b2Vec2 vel = m_body->GetLinearVelocity();
			vel.x = 0;
			m_body->SetLinearVelocity(vel);
		}
	}

	//ジャンプ
	//スペースキーかパッドの×ボタンが押されたか、かつジャンプフラグが立っていたら
	if ((CTRL.GetKeyboardTrigger(DIK_SPACE) || CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CROSS, m_gamePadNum)) && m_isJump) {
		//上方向に力を加える
		m_body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -5.0f), true);
		m_isJump = false;
	}

	//オブジェクトホールド
	if (CTRL.GetKeyboardTrigger(DIK_RETURN)) {
		if (m_collisionObject && !m_holdObject) {
			m_holdObject = m_collisionObject;
			m_holdObject->Hold(m_body);
		} else if (m_holdObject) {
			m_holdObject->Throw(5, -5);
			m_holdObject = nullptr;
		}
	}
}

/****************************************************
* プレイヤー描画
*****************************************************/
void Player::Draw() {
	//dx座標で描画
	D3D.Draw2D(m_tex, m_pos.x, m_pos.y, m_size.x, m_size.y, m_rot, 0.1f, 0.1f, 0.8f, 0.8f);
}

/****************************************************
* プレイヤー当たり判定
*****************************************************/
void Player::OnCollisionEnter(GameObject* collision) {
	if (collision->CompareTag("Ground")) {
		m_isJump = true;
	}

	if (collision->CompareTag("FieldObject")) {
		if (!m_collisionObject) {
			m_collisionObject = (FieldObject*)collision;
		}
	}
}

/****************************************************
* プレイヤー当たり判定解除
*****************************************************/
void Player::OnCollisionExit(GameObject* collision) {
	if (collision->CompareTag("FieldObject")) {
		if (m_collisionObject) {
			m_collisionObject = nullptr;
		}
	}
}

