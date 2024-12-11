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
#include "Game/FieldObject.h"

/****************************************************
* プレイヤー初期化
*****************************************************/
Player::Player(XMFLOAT2 startpos,int pnum) {
	//初期設定
	m_pos = startpos;//12/4
	m_rot = 0.0f;
	m_size = XMFLOAT2(120.0f, 120.0f);

	m_pNum = pnum;

	CreatePlayerBody();

	//テクスチャロード
	switch (m_pNum) {
		case 1:
			m_tex.Load("Data/Texture/fox.png");
			break;
		case 2:
			m_tex.Load("Data/Texture/ikemen.png");
			break;
		case 3:
			m_tex.Load("Data/Texture/Nekketsu.png");
			break;
		case 4:
			m_tex.Load("Data/Texture/bisyoujo.png");
			break;
		default:
			break;
	}

	m_throwArrowTex.Load("Data/Texture/throwArrow.png");

	m_gamePadNum = CTRL.GetGamepadHandle();

	m_isJump = false;

	m_throwVector.Set(5, -5);

	SetTag("Player");
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

	if (m_isBlow) {
		BlowAway();
		m_isBlow = false;
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
		vel.x = CTRL.GetLeftStickHorizontal(m_gamePadNum) * 0.005;
		//速度を変更
		if (m_isBlowed) {
			m_body->ApplyForceToCenter(vel, true);
		} else {
			m_body->SetLinearVelocity(vel);
		}
//		m_body->ApplyForceToCenter(vel, true);


		//投げる角度取得
		b2Vec2 oldVec = m_throwVector;
		m_throwVector.x = CTRL.GetLeftStickHorizontal(m_gamePadNum);
		m_throwVector.y = CTRL.GetLeftStickVertical(m_gamePadNum);
		if (m_throwVector.x == 0 && m_throwVector.y == 0) {
			m_throwVector = oldVec;
		}
		m_throwVector.Normalize();

	} else {
		if (CTRL.GetKeyboardPress(DIK_A)) {
			m_body->ApplyForceToCenter(b2Vec2(-10.0f, 0.0f), true);
		} else if (CTRL.GetKeyboardPress(DIK_D)) {
			m_body->ApplyForceToCenter(b2Vec2(10.0f, 0.0f), true);

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
	if (CTRL.GetKeyboardTrigger(DIK_RETURN) || CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_SQUARE, m_gamePadNum)) {
		if (!m_collisionObjects.empty() && !m_holdObject) {
			m_holdObject = (*m_collisionObjects.begin());
			if (m_holdObject->Hold(m_body, this)) {
				m_collisionObjects.erase(m_collisionObjects.begin());
			} else {
				m_holdObject = nullptr;
			}
		} else if (m_holdObject) {
			float x = m_throwVector.x * m_throwPower;
			float y = m_throwVector.y * m_throwPower;

			bool isThrow = m_holdObject->Throw(x, y);
			if (isThrow) m_holdObject = nullptr;
		}
	}
}

/****************************************************
* プレイヤー描画
*****************************************************/
void Player::Draw() {
	//dx座標で描画
	D3D.Draw2D(m_tex, m_pos.x, m_pos.y, m_size.x, m_size.y, m_rot, 0.0f, 0.0f, 1.0f, 1.0f);
	//オブジェクトを持っていたら
	if (m_holdObject) {
		//矢印描画
		float rot = atan2f(m_throwVector.y, m_throwVector.x);
		D3D.Draw2D(m_throwArrowTex, m_pos.x, m_pos.y - m_size.y, m_size.x * 0.5f, m_size.y * 0.5f, rot, 0.0f, 0.0f, 1.0f, 1.0f);
	}
}

/****************************************************
* プレイヤー当たり判定
*****************************************************/
void Player::OnCollisionEnter(GameObject* collision) {
	if (collision->CompareTag("Ground")) {
		m_isJump = true;
		m_isBlowed = false;
	}

	if (collision->CompareTag("Field") && m_isBlowed) {
		int damage = m_body->GetFixtureList()->GetDensity() * 5;
		((FieldObject*)collision)->Attack(damage);
		m_isBlowed = false;
	}

	if (collision->CompareTag("ThrowObject")) {
		m_collisionObjects.push_back((ThrowObject*)collision);

	}
}

/****************************************************
* プレイヤー当たり判定解除
*****************************************************/
void Player::OnCollisionExit(GameObject* collision) {
	if (collision->CompareTag("ThrowObject")) {
		for (auto itr = m_collisionObjects.begin(); itr != m_collisionObjects.end(); ) {
			if ((*itr) == collision) {
				itr = m_collisionObjects.erase(itr);
			} else {
				++itr;
			}
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
		m_isBlowed = true;
	}
}



/******************************************************
* 当たった関数	( OnCollisionとは違う関数　)
*******************************************************/
void Player::ApplyImpact(const b2Vec2& impactVector)
{
	//ヒットストップフラグ立てる
	m_Hitstop.SetIsHitStop(true, 10);

	//渡されたベクトルをメンバ変数に格納
	m_blowForce = impactVector;

	m_isBlow = true;
}

/*******************************************************
* プレイヤーボディ作成
********************************************************/
void Player::CreatePlayerBody() {
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
	m_filterName = "プレイヤー" + std::to_string(m_pNum);
	b2Filter filter = m_body->GetFixtureList()->GetFilterData();
	filter.categoryBits = std::hash<std::string>{} (m_filterName);
	m_body->GetFixtureList()->SetFilterData(filter);

	//保持しているものを破棄
	m_collisionObjects.clear();
	m_holdObject = nullptr;
}

