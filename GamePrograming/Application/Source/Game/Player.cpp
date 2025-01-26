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
#include "Game/EffectManager.h"

#include "Game/Esper.h"

/****************************************************
* プレイヤー初期化
*****************************************************/
Player::Player(XMFLOAT2 startpos,int pnum) {
	//初期設定
	m_pos = startpos;//12/4
	m_rot = 0.0f;
	m_size = XMFLOAT2(140.0f * 1.5f, 140.0f * 1.5f);

	m_pNum = pnum;

	m_blowedTime = 0.0f;

	/*******************************************
	 追加日：12/27　担当：弓田
	********************************************/
	// 残機の初期化
	m_lives = 2;

	// 残りジャンプ回数の初期化
	m_remainingJumps = 2; //　変更日：2024/12/28 担当：弓田

	/********************************************/

	CreatePlayerBody();

	//テクスチャロード
	switch (m_pNum) {
		case 1:
			m_playerColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			m_tex.Load(L"Data/Texture/fox.png");
			break;
		case 2:
			m_playerColor = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			m_tex.Load(L"Data/Texture/ikemen.png");
			break;
		case 3:
			m_playerColor = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			m_tex.Load(L"Data/Texture/Nekketsu.png");
			break;
		case 4:
			m_playerColor = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
			m_tex.Load(L"Data/Texture/bisyoujo.png");
			break;
		default:
			m_playerColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			break;
	}

	// 仮にキャラクターをセット
	m_pCharacter = new Esper();

	m_throwArrowTex.Load(L"Data/Texture/throwArrow.png");

	m_gamePadNum = CTRL.GetGamepadHandle();

	m_throwVector.Set(5, -5);

	SetTag("Player");
}

/****************************************************
* プレイヤー終了
*****************************************************/
Player::~Player() {
	Physics::GetWorld()->DestroyBody(m_body);
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

	/*******************************************
	 追加日：12/27　担当：弓田
	********************************************/
	// 画面外にいるか判定
	if (IsBringDown()) {

		// 撃墜エフェクトを呼ぶ


		// 残機を減らす
		m_lives--;

		// 残機が0以下なら
		if (m_lives>0) {
			// 復活処理
			RespawnPlayer(XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
		}
		else {
			SetIsDelete();
		}
	}

	/********************************************/

	//左右移動
	//ゲームパッドが接続されているか
	if (m_gamePadNum >= 0) {
		////現在の速度を取得(ｙ方向の速度はそのまま使いたい為)
		//b2Vec2 vel = b2Vec2_zero; //m_body->GetLinearVelocity();
		////パッドの角度を補正して速度に代入
		//vel.x = CTRL.GetLeftStickHorizontal(m_gamePadNum) * 0.01f;
		////速度を変更
		////if (m_isBlowed) {
		////	m_body->ApplyForceToCenter(vel, true);
		////} else {
		////	m_body->SetLinearVelocity(vel);
		////}
		//m_body->ApplyForceToCenter(vel, true);

		//現在の速度を取得
		b2Vec2 vel = m_body->GetLinearVelocity();
		//コントローラーの左右を取得
		LONG hor = CTRL.GetLeftStickHorizontal(m_gamePadNum);
		//コントローラー補正値
		float controllerCorrection = 0.0f;
		if (vel.x * hor < 0) {
			controllerCorrection = 0.3f;
		} else {
			controllerCorrection = 0.05f;
		}
		b2Vec2 force = b2Vec2(hor * controllerCorrection, 0.0f);
		m_body->ApplyForceToCenter(force, true);

		
		// スティックが起因となるモーションの管理
		if (hor != 0) {

			// スティックの方向によってキャラを反転
			if (hor > 0) {
				m_pCharacter->IsCharacterFacingLeft(false);
			}
			else {
				m_pCharacter->IsCharacterFacingLeft(true);
			}

			if (m_isGround && !(m_pCharacter->GetInterruptFlag())) {
				m_pCharacter->SetAnimState(MOVE);
			}
		}
		else if (m_isGround && !(m_pCharacter->GetInterruptFlag())) {
			m_pCharacter->SetAnimState(IDLE);
		}
		
		


		//投げる角度取得
		b2Vec2 oldVec = m_throwVector;
		m_throwVector.x = (float)CTRL.GetLeftStickHorizontal(m_gamePadNum);
		m_throwVector.y = (float)CTRL.GetLeftStickVertical(m_gamePadNum);
		if (m_throwVector.x == 0 && m_throwVector.y == 0) {
			m_throwVector = oldVec;
		}
		m_throwVector.Normalize();

	} else {
		if (CTRL.GetKeyboardPress(DIK_A)) {
			m_body->ApplyForceToCenter(b2Vec2(-50.0f, 0.0f), true);

			m_pCharacter->IsCharacterFacingLeft(false);
			m_pCharacter->SetAnimState(MOVE);

		} else if (CTRL.GetKeyboardPress(DIK_D)) {

			m_body->ApplyForceToCenter(b2Vec2(50.0f, 0.0f), true);

			m_pCharacter->IsCharacterFacingLeft(true);
			m_pCharacter->SetAnimState(MOVE);
		}
		else if (m_isGround && !(m_pCharacter->GetInterruptFlag())) {
			m_pCharacter->SetAnimState(IDLE);
		}

		//投げる角度
		static float throwAngle = 0.0f;
		if (CTRL.GetKeyboardPress(DIK_RIGHTARROW)) {
			throwAngle += 5.0f;
		}
		if (CTRL.GetKeyboardPress(DIK_LEFTARROW)) {
			throwAngle -= 5.0f;
		}

		m_throwVector.x = cosf(XMConvertToRadians(throwAngle));
		m_throwVector.y = sinf(XMConvertToRadians(throwAngle));

		m_throwVector.Normalize();
	}


	//ジャンプ
	//スペースキーかパッドの×ボタンが押されたか、かつジャンプフラグが立っていたら
	if ((CTRL.GetKeyboardTrigger(DIK_SPACE) || CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CROSS, m_gamePadNum)) && m_remainingJumps > 0) {
		//上方向に力を加える
		m_body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -27.5f), true); // -20から-27.5に変更。担当：弓田
		
		m_remainingJumps--;

		m_isGround ? EffectManager::CreateEffect(Jump, XMFLOAT2(m_pos.x, m_pos.y + 10.0f), XMFLOAT2(300.0f, 300.0f), 0.0f) :
			EffectManager::CreateEffect(AirJump, XMFLOAT2(m_pos.x, m_pos.y + 10.0f), XMFLOAT2(300.0f, 300.0f), 0.0f);

		m_isGround = false;
	}

	//オブジェクトホールド
	if (CTRL.GetKeyboardTrigger(DIK_RETURN) || CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_SQUARE, m_gamePadNum)) {
		if (!m_collisionObjects.empty() && !m_holdObject) {
			m_holdObject = (*m_collisionObjects.begin());
			if (m_holdObject->Hold(m_body, this)) {
				m_collisionObjects.erase(m_collisionObjects.begin());
			}
			else {
				m_holdObject = nullptr;
			}
		}
		else if (m_holdObject) {
			float x = m_throwVector.x * m_throwPower;
			float y = m_throwVector.y * m_throwPower;

			bool isThrow = m_holdObject->Throw(x, y);
			if (isThrow) m_holdObject = nullptr;
		}
	}

	//ターゲットのオブジェクトに色をつける
	if ((!m_collisionObjects.empty() && !m_holdObject) && !(*m_collisionObjects.begin())->IsExistsPlayer()) {
		(*m_collisionObjects.begin())->SetPlayerColor(m_playerColor);
	} else if (m_holdObject) {
		m_holdObject->SetPlayerColor(m_playerColor);
	}


	// 01/17 弓田追加
	if (m_isBlowed) {
		m_blowedTime++;

		if (m_blowedTime >= 60.0f * 1.5f) {
			m_blowedTime = 0.0f;
			m_isBlowed = false;
		}
	}

	m_pCharacter->Update();


	if (abs(m_body->GetLinearVelocity().y) <= 0.01f) {
		m_isGround = true;
	}


	// 空中モーション制御
	if (!m_isGround && !(m_pCharacter->GetInterruptFlag())) {

		if (m_body->GetLinearVelocity().y < 0) {
			m_pCharacter->SetAnimState(JUMP);
		}
		else {
			m_pCharacter->SetAnimState(FALL);
		}
	}
}

/****************************************************
* プレイヤー描画
*****************************************************/
void Player::Draw() {
	//dx座標で描画
	//D3D.Draw2D(m_tex, m_pos, m_size, m_rot);
	m_pCharacter->Draw(m_pos, m_size, m_rot);

	//オブジェクトを持っていたら
	if (m_holdObject) {
		//矢印描画
		float rot = atan2f(m_throwVector.y, m_throwVector.x);
		XMFLOAT2 pos = XMFLOAT2(m_pos.x, m_pos.y - m_size.y);
		XMFLOAT2 size = XMFLOAT2(m_size.x * 0.5f, m_size.y * 0.5f);
		D3D.Draw2D(m_throwArrowTex, pos, size, rot, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), m_playerColor);
	}
}

/****************************************************
* プレイヤー当たり判定
*****************************************************/
void Player::OnCollisionEnter(GameObject* collision) {
	if (collision->CompareTag("Ground")) {
		// ジャンプ回数をリセット
		m_remainingJumps = 2;

		//m_isGround = true;
	}

	if (collision->CompareTag("Field") && m_isBlowed) {

		// エフェクト
		EffectManager::CreateEffect(PlayerHitWall, m_pos, XMFLOAT2(600.0f, 600.0f), 0.0f);

		int damage = 5;
		((FieldObject*)collision)->Attack(damage);

		m_isBlowed = false;
	}

	if (collision->CompareTag("ThrowObject")) {
		m_collisionObjects.push_back((ThrowObject*)collision);

		// モノの上に立っている場合、ジャンプ回数をリセット（追加日：12/27 担当：弓田）
		if (m_pos.y <= ((ThrowObject*)collision)->GetPos().y) {
			// ジャンプ回数をリセット
			m_remainingJumps = 2;

			//m_isGround = true;
		}
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
				break;
			} else {
				++itr;
			}
		}
	}

	if (collision->CompareTag("Ground")|| collision->CompareTag("ThrowObject")) {
		//m_isGround = false;
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

/******************************************************
* 復活関数	（追加日：12/27　担当：弓田）
*******************************************************/
void Player::RespawnPlayer(XMFLOAT2 RespawnPos)
{
	m_body->SetTransform(Physics::ConvertDXtoB2Float2(RespawnPos), 0.0f);
	m_body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	m_body->SetAngularVelocity(0.0f);
}

/******************************************************
* 撃墜されたか確認（追加日：12/27　担当：弓田）
*******************************************************/
bool Player::IsBringDown()
{
	return m_pos.x <= 0.0f || m_pos.x >= SCREEN_WIDTH + 10.0f || m_pos.y <= -10.0f || m_pos.y >= SCREEN_HEIGHT + 10.0f;
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
	Physics::CreateCapsule(&m_body, size.x * 0.5f, size.y);

	//回転無効
	m_body->SetFixedRotation(true);

	//フィルター設定
	m_filterName = "プレイヤー" + std::to_string(m_pNum);
	b2Fixture* fixture = m_body->GetFixtureList();
	while (fixture) {
		b2Filter filter = fixture->GetFilterData();
		filter.categoryBits = std::hash<std::string>{} (m_filterName);
		fixture->SetFilterData(filter);
		fixture = fixture->GetNext();
	}

	//保持しているものを破棄
	m_collisionObjects.clear();
	m_holdObject = nullptr;
}

