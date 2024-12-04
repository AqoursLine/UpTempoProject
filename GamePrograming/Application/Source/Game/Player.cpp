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
#include "Game/HitStop.h"

/****************************************************
* プレイヤー初期化
*****************************************************/
Player::Player(XMFLOAT2 startpos,int pnum) {
	//初期設定
	m_pos = startpos;//12/4
	m_rot = 0.0f;
	m_size = XMFLOAT2(120.0f, 120.0f);

	m_pNum = pnum;
	
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

	//フィルター設定
	m_filterName = "プレイヤー" + std::to_string(pnum);
	b2Filter filter = m_body->GetFixtureList()->GetFilterData();
	filter.categoryBits = std::hash<std::string>{} (m_filterName);
	m_body->GetFixtureList()->SetFilterData(filter);

	//テクスチャロード
	m_tex.Load("Data/Texture/player.png");

	m_gamePadNum = CTRL.GetGamepadHandle();

	m_isJump = false;

	CompareTag("Player");
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

	if(m_Hitstop.IsHitStop(m_body))
	{
		return;
	}
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
			//b2Vec2 vel = m_body->GetLinearVelocity();
			//vel.x = -5;
			//m_body->SetLinearVelocity(vel);
			m_body->ApplyForceToCenter(b2Vec2(-10.0f, 0.0f), true);
		} else if (CTRL.GetKeyboardPress(DIK_D)) {
			//b2Vec2 vel = m_body->GetLinearVelocity();
			//vel.x = 5;
			//m_body->SetLinearVelocity(vel);

			m_body->ApplyForceToCenter(b2Vec2(10.0f, 0.0f), true);

		} else {
			//b2Vec2 vel = m_body->GetLinearVelocity();
			//vel.x = 0;
			//m_body->SetLinearVelocity(vel);
		}
	}

	//ジャンプ
	//スペースキーかパッドの×ボタンが押されたか、かつジャンプフラグが立っていたら
	if ((CTRL.GetKeyboardTrigger(DIK_SPACE) || CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CROSS, m_gamePadNum)) && m_isJump) {
		//上方向に力を加える
		m_body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -8.0f), true);
		m_isJump = false;
	}

	//オブジェクトホールド
	if (CTRL.GetKeyboardTrigger(DIK_RETURN) || CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_R2, m_gamePadNum)) {
		if (m_collisionObject && !m_holdObject) {
			m_holdObject = m_collisionObject;
			m_holdObject->Hold(m_body);
			m_collisionObject = nullptr;
		} else if (m_holdObject) {
			bool isThrow = m_holdObject->Throw(5, -5);
			if (isThrow) m_holdObject = nullptr;
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

	if (collision->CompareTag("ThrowObject")) {
		if (!m_collisionObject) {
			m_collisionObject = (ThrowObject*)collision;
		}
	}
}

/****************************************************
* プレイヤー当たり判定解除
*****************************************************/
void Player::OnCollisionExit(GameObject* collision) {
	if (collision->CompareTag("ThrowObject")) {
		if (m_collisionObject) {
			m_collisionObject = nullptr;
		}
	}
}


//12/03追加(仙波）

/*****************************************************
* 吹っ飛ばす処理
******************************************************/
void Player::BlowAway()
{
	if (m_body) {
		// メンバ変数の吹っ飛ぶ力をボディに加える
		m_body->ApplyLinearImpulseToCenter(m_blowForce, true);
	}
}



/******************************************************
* 当たった関数	( OnCollisionとは違う関数　)
*******************************************************/
void Player::ApplyImpact(const b2Vec2& impactVector)
{
	//ヒットストップフラグ立てる
	m_Hitstop.SetIsHitStop(true, 60);

	//渡されたベクトルをメンバ変数に格納
	m_blowForce = impactVector;
}

/*******************************************************
* 
********************************************************/
