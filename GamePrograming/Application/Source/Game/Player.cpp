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
	m_size = XMFLOAT2(140.0f * 1.4f, 140.0f * 1.4f); // もっと大きくする必要あり
	m_pNum = pnum;
	m_blowedTime = 0.0f;

	m_ePos = startpos;//12/4
	m_eRot = 0.0f;
	efUse = false;

	m_hp = 0.0f;
	/*******************************************
	 追加日：12/27　担当：弓田
	********************************************/
	// 残機の初期化
	m_lives = 2;

	// 残りジャンプ回数の初期化
	m_remainingJumps = 2; //　変更日：2024/12/28 担当：弓田

	/********************************************/

	//バフ関連初期化
	m_moveDown = false;
	m_atkBuff = false;
	m_defBuff = false;
	m_invert = false;
	m_isFloating = false;

	int m_invertFrame = 0;
	int m_downFrame = 0;

	CreatePlayerBody();
	LoadDamageTextures();

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

	LoadDamageTextures(); 
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

	//定数
	constexpr float DOWN_MAGNIFICATION = 0.5f;	//デバフ倍率　移動速度 * 倍率でデバフ
	constexpr float THROW_MAGNIFICATION = 2.5f;	//バフ倍率　投げる強さ * 倍率でバフ
	//デバフの時間管理
	if (m_downFrame > 60 * 5)//持続時間 60 * ??　移動デバフ
	{
		m_moveDown = false;
		m_downFrame = 0;
	}
	if (m_moveDown)
	{
		m_downFrame++;
	}
	//操作反転
	if (m_invertFrame > 60 * 5)//持続時間 60 * ??
	{
		m_invert = false;
		m_invertFrame = 0;
	}
	if (m_moveDown)
	{
		m_invertFrame++;
	}


	if (m_isBlowed)//effectの移動処理
	{
		b2Vec2 vel = m_body->GetLinearVelocity();
		float check = vel.Normalize();
		
		//位置
		m_ePos.x = m_pos.x + (-vel.x * 100);
		m_ePos.y = m_pos.y + (-vel.y * 100);

		//角度
		float ang = atan2(vel.x, -vel.y);
		if (ang < 0)
			ang += XM_PI * 2;

		m_eRot = ang ;

	}



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
		LONG hor = CTRL.GetLeftStickHorizontal(m_gamePadNum) * (m_invert ? -1 : 1);
		//コントローラー補正値
		float controllerCorrection = 0.0f;
		if (vel.x * hor < 0) {
			controllerCorrection = 0.3f;
		} else {
			controllerCorrection = 0.05f;
		}

		
		//移動デバフかかってる場合*0.5f
		b2Vec2 force = b2Vec2(hor * controllerCorrection * (m_moveDown ? DOWN_MAGNIFICATION : 1), 0.0f);

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
		// 着地モーションが再生されているときはIDLEモーションにしない。再生が終わったらcharacter.cppの方でIDLEモーションにする
		else if (m_isGround && !(m_pCharacter->GetInterruptFlag()) && m_pCharacter->GetAnimState() != LANDING) {
			m_pCharacter->SetAnimState(IDLE);
		}
		

		//投げる角度取得
		b2Vec2 oldVec = m_throwVector;
		m_throwVector.x = (float)CTRL.GetLeftStickHorizontal(m_gamePadNum) * (m_invert ? -1 : 1);
		m_throwVector.y = (float)CTRL.GetLeftStickVertical(m_gamePadNum) * (m_invert ? -1 : 1);
		if (m_throwVector.x == 0 && m_throwVector.y == 0) {
			m_throwVector = oldVec;
		}
		m_throwVector.Normalize();

	} else {
		if (CTRL.GetKeyboardPress(DIK_A)) {

			m_body->ApplyForceToCenter(b2Vec2(-50.0f * (m_moveDown? DOWN_MAGNIFICATION : 1) *
				(m_invert ? -1 : 1), 0.0f), true);
		
			m_pCharacter->IsCharacterFacingLeft(false);
			
			if (m_isGround && !(m_pCharacter->GetInterruptFlag())) {
				m_pCharacter->SetAnimState(MOVE);
			}

		} else if (CTRL.GetKeyboardPress(DIK_D)) {
			//デバフの補正
			m_body->ApplyForceToCenter(b2Vec2(50.0f * (m_moveDown ? DOWN_MAGNIFICATION : 1) *
				(m_invert ? -1 : 1), 0.0f), true);
			
			m_pCharacter->IsCharacterFacingLeft(true);

			if (m_isGround && !(m_pCharacter->GetInterruptFlag())) {
				m_pCharacter->SetAnimState(MOVE);
			}
		
		}
		else if (m_isGround && !(m_pCharacter->GetInterruptFlag())) {
			m_pCharacter->SetAnimState(IDLE);
		}

		//投げる角度
		static float throwAngle = 0.0f;
		if (CTRL.GetKeyboardPress(DIK_RIGHTARROW)) {
			//throwAngle += m_invert ? -5.0f : 5.0f;//反転だけど全キャラ共通のためうまく動作しない
			throwAngle += 5.0f;
		}
		if (CTRL.GetKeyboardPress(DIK_LEFTARROW)) {
			//throwAngle -= m_invert ? -5.0f : 5.0f;;
			throwAngle -= 5.0f;;
		}
		

		m_throwVector.x = cosf(XMConvertToRadians(throwAngle));
		m_throwVector.y = sinf(XMConvertToRadians(throwAngle));

		m_throwVector.Normalize();
	}

	if (m_holdObject && m_holdObject->CompareType("Balloon"))//風船持ったら浮くよー
	{
		m_body->ApplyForce(b2Vec2(0, -9.8f * m_body->GetMass() * 1.5f), m_body->GetWorldCenter(), true);
		if (!m_isFloating)
		{
			m_remainingJumps--;
			m_isFloating = true;
		}
	}
	else if(m_isFloating)
	{
		m_isFloating = false;
	}

	//ジャンプ
	//スペースキーかパッドの×ボタンが押されたか、かつジャンプフラグが立っていたら
	if ((CTRL.GetKeyboardTrigger(DIK_SPACE) || CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CROSS, m_gamePadNum)) && m_remainingJumps > 0) {
		//上方向に力を加える
		// 追記：一旦、かかっている力をリセットしてから力を加えた方がいいかも
		if (m_moveDown)//デバフ時
		{
			m_body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -27.5f * DOWN_MAGNIFICATION), true);
		}
		else//通常
		{
			m_body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -27.5f), true); // -20から-27.5に変更。担当：弓田 
		}
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

				// 持ちモーションをセット
				m_pCharacter->SetAnimState(HAVETHINGS);
				m_pCharacter->SetInterruptFlag(true);
			}
			else {
				m_holdObject = nullptr;
			}
		}
		else if (m_holdObject) {
			float x = m_throwVector.x * m_throwPower;
			float y = m_throwVector.y * m_throwPower;

			if (m_atkBuff)
			{
				x *= THROW_MAGNIFICATION;
				y *= THROW_MAGNIFICATION;
				m_atkBuff = false;
			}

			if (m_holdObject->CompareType("AtkBuff"))//投げるオブジェクトのタイプでバフを
			{
				m_atkBuff = true;
			}
			if (m_holdObject->CompareType("DefBuff"))//
			{
				m_defBuff = true;
			}

			bool isThrow = m_holdObject->Throw(x, y);//投げる

			if (isThrow) {
				m_holdObject = nullptr;

				// 投げるモーションをセット
				m_pCharacter->SetAnimState(THROW);
				m_pCharacter->SetStopAnim(false);
			}
			
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
		
			efUse = false;//エフェクト使用可能

			// モノを持っていなかったら割り込みフラグを下げる
			if(!m_holdObject)
			m_pCharacter->SetInterruptFlag(false);
		}
	}

	


	if (abs(m_body->GetLinearVelocity().y) <= 0.01f) {
		m_isGround = true;

		if (m_pCharacter->GetAnimState() == FALL) {
			// 着地モーション
			m_pCharacter->SetAnimState(LANDING);
		}
		
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

	
	m_pCharacter->Update();
	OutputDebugString((L"Current Damage: " + std::to_wstring(m_damage) + L"\n").c_str());
}




//ダメージテクスチャロード 02/01追加	中川
void Player::LoadDamageTextures()
{
	m_damageTex[0].Load(L"Data/Texture/Damage_0.png");
	m_damageTex[1].Load(L"Data/Texture/Damage_1.png");
	m_damageTex[2].Load(L"Data/Texture/Damage_2.png");
	m_damageTex[3].Load(L"Data/Texture/Damage_3.png");
	m_damageTex[4].Load(L"Data/Texture/Damage_4.png");
	m_damageTex[5].Load(L"Data/Texture/Damage_5.png");
	m_damageTex[6].Load(L"Data/Texture/Damage_6.png");
	m_damageTex[7].Load(L"Data/Texture/Damage_7.png");
	m_damageTex[8].Load(L"Data/Texture/Damage_8.png");
	m_damageTex[9].Load(L"Data/Texture/Damage_9.png");
	m_damageTex[10].Load(L"Data/Texture/Damage_Percent.png");
}

void Player::DrawDamageNumber(const XMFLOAT2& pos, int damage) {
	std::string damageText = std::to_string(damage) + "%";

	float digitSpacing = 40.0f;	//文字間のスペース
	XMFLOAT2 drawSize = XMFLOAT2(50, 80);	//画像のサイズ

	for (size_t i = 0; i < damageText.size(); i++)
	{
		int index = (damageText[i] == '%') ? 10 : (damageText[i] - '0');

		D3D.Draw2D(m_damageTex[index],
			XMFLOAT2(pos.x + i * digitSpacing, pos.y),
			drawSize);
	}
}








/****************************************************
* プレイヤー描画
*****************************************************/
void Player::Draw() {
	//dx座標で描画
	//D3D.Draw2D(m_tex, m_pos, m_size, m_rot);
	m_pCharacter->Draw(m_pos, m_size, m_rot);

	//ダメージ表示の位置を画面左上に固定
	XMFLOAT2 damagePos = XMFLOAT2(50 + (m_pNum -1) * 100, 30);

	DrawDamageNumber(damagePos, m_damage);

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
		if (m_holdObject && m_holdObject->CompareType("Balloon"))//空中でのジャンプ回数を1度にするため
		{
		}
		else
		{
			// ジャンプ回数をリセット
			m_remainingJumps = 2;
		}
		//m_isGround = true;
	}

	if (collision->CompareTag("Field") && m_isBlowed) {

		// エフェクト
		EffectManager::CreateEffect(PlayerHitWall, m_pos, XMFLOAT2(600.0f, 600.0f), 0.0f);

		int damage = 5;
		((FieldObject*)collision)->Attack(damage);

		m_isBlowed = false;

		//エフェクト再使用可能に
		efUse = false;

		if (!m_holdObject) {
			m_pCharacter->SetInterruptFlag(false);
		}
		
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

		if (!efUse)//吹っ飛びエフェクト生成テスト
		{
			m_ePos = m_pos;
			m_eRot = 0;
			efUse = true;
			EffectManager::CreateMoveEffect(Jump, &m_ePos, XMFLOAT2(300.0f, 300.0f), &m_eRot, 0, &m_isBlowed, 1);
		}
		


		// 吹っ飛びモーションをセット
		m_pCharacter->SetAnimState(BLOW);
	}
}



/******************************************************
* 当たった関数	( OnCollisionとは違う関数　)
*******************************************************/
void Player::ApplyImpact(const b2Vec2& impactVector, WEIGHT weight)
{
	int damageAmount = 0;

	//ダメージ量を重さで変える		02・01追加	中川
	switch (weight) {
	case WEIGHT_LIGHT:
		damageAmount = 5;
		break;
	case WEIGHT_NORMAL:
		damageAmount = 10;
		break;
	case WEIGHT_HEAVY:
		damageAmount = 20;
		break;
	}
	m_damage += damageAmount;

	//ダメージに応じて吹っ飛ぶ力を増加　最大三倍	中川
	float impactScale = 1.0f + (m_damage * 0.02f);
	impactScale = min(impactScale, 3.0f);	//	最大3倍

	b2Vec2 adjustedImpact = b2Vec2(impactVector.x * impactScale, impactVector.y * impactScale);

	//ヒットストップフラグ立てる
	m_Hitstop.SetIsHitStop(true, 10);

	//渡されたベクトルをメンバ変数に格納
	m_blowForce = adjustedImpact;



	m_isBlow = true;

	m_defBuff = false;

	// モーションの割り込みフラグを立てる
	m_pCharacter->SetInterruptFlag(true);
	
	// ヒットストップモーションをセット
	m_pCharacter->SetAnimState(HITSTOP);


}

/******************************************************
* 復活関数	（追加日：12/27　担当：弓田）
*******************************************************/
void Player::RespawnPlayer(XMFLOAT2 RespawnPos)
{
	m_body->SetTransform(Physics::ConvertDXtoB2Float2(RespawnPos), 0.0f);
	m_body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	m_body->SetAngularVelocity(0.0f);

  m_hp = 0.0f;
	//バフ関連リセット
	m_moveDown = false;
	m_atkBuff = false;
	m_defBuff = false;
	m_invert = false;
	m_isFloating = false;

	int m_invertFrame = 0;
	int m_downFrame = 0;

	m_damage = 0;

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

void Player::SetNullHoldObject()
{
	m_holdObject = nullptr;
	m_pCharacter->SetInterruptFlag(false);
	m_pCharacter->SetAnimState(IDLE);
}
