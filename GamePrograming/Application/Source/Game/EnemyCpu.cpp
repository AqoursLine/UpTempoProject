#include "framework.h"
#include "DirectX/DirectX.h"
#include "DirectX/Audio.h"
#include "Game/Physics.h"
#include "Game/Controller.h"
#include "Game/EnemyCpu.h"
#include "Game/HitStop.h"
#include "Game/FieldObject.h"
#include "Game/EffectManager.h"
#include "SaveData.h"
#include "Lamp.h"

#include "Game/ThrowObjectManager.h"

#include "Game/Esper.h"
#include "Game/Bancho.h"
#include "Game/Handsome.h"
#include "Game/Beautiful.h"
#include "Game/Ghost.h"
#include "Game/Rabbit.h"


// -900 900
//

EnemyCpu::EnemyCpu(XMFLOAT2 startpos, int pnum) : Player(startpos, pnum) {

	m_moveDir = 1;
	m_frameCnt = 0;

	//座標変換
	b2Vec2 pos = Physics::ConvertDXtoB2Float2(m_pos);
	//ボディ作成
	Physics::CreateBody(&m_senBody, pos.x, pos.y, m_rot, true, this);
	//座標変換
	b2Vec2 size = Physics::ConvertDXtoB2Float2(m_size);
	//当たり判定作成　カプセルじゃなくていい
	Physics::CreateFixture(&m_senBody, size.x * 0.5f * 10000.0f, size.y * 10000.0f, 1.0f, 0.3f, 0.0f, true);

	//回転無効
	m_senBody->SetFixedRotation(true);

	//ふぃるたー
	b2Fixture* fixture = m_senBody->GetFixtureList();
	while (fixture) {
		b2Filter filter = fixture->GetFilterData();
		filter.maskBits = 0x0000;
		fixture->SetFilterData(filter);
		fixture = fixture->GetNext();

	}
	m_charactorUvX = 4;

	m_playerNumObj.SetTexture(this, true, m_pNum);
}

EnemyCpu::~EnemyCpu()
{
	if(m_targetObject)
	m_targetObject->cpuTarget[m_pNum - 1] = nullptr;
}


/****************************************************
* プレイヤー更新
*****************************************************/
void EnemyCpu::Update() {
	m_playerNumObj.Update();

	if (m_respawnStandby)
	{
		if (m_respawnCnt > 180)
		{
			RespawnPlayer(XMFLOAT2(static_cast<float>(320 * m_pNum), static_cast<float>(SCREEN_HEIGHT / 2)));//プレイヤーの総人数から調整する場合は320を1920/(2+総プレイヤー数)
			m_respawnStandby = false;
			PhysicsUpdate();
		}
		else
		{
			m_respawnCnt++;
			return;
		}
	}


	//スティックの役割を持たせる変数
	b2Vec2 stickL{1.0f,1.0f};

	m_senBody->SetTransform(m_body->GetPosition(), m_body->GetAngle());

	

	if (m_targetObject)
	{
		if (m_targetObject->GetIsThrow() || m_targetObject->GetPlayerBool())
		{
			m_targetObject = nullptr;
		}
	}
	//if (!m_targetObject)//近くのオブジェクトを確認しセット
	CheckFixturesInSensor(m_senBody->GetFixtureList());

	//仮想Lスティック変数に値を入力
	if (m_holdObject)
	{
		
		if (m_targetP)
		{
			stickL.x = m_targetP->GetPos().x - m_pos.x;
			stickL.y = m_targetP->GetPos().y - m_pos.y;
		}
		else
		{//基本的には通らない//ターゲットPの切り替わりに何フレームか遅延があるためターゲットが死んだ直後だと通るかも
			stickL.x = m_moveDir;
			stickL.y = -0.5f;
		}
		if (stickL.x == 0 && stickL.y == 0) {//こんな状況はない
			stickL.x = m_moveDir;
		}
	
	}
	else if(m_targetObject)
	{//ターゲットになるオブジェクトが存在している場合
		float vec = m_targetObject->GetPos().x - m_pos.x;
		stickL.x = vec / abs(vec);
		if (CTRL.GetKeyboardPress(DIK_A))
		{
			int a = 0;
		}
		
	}
	else
	{//オブジェクトを持っていなく、ターゲットのオブジェクトもない場合
		if (abs(m_pos.x - SCREEN_WIDTH / 2.0f) > 300)//落下防止のため中央付近でうろうろするようにしたい
		{
			if (m_pos.x - SCREEN_WIDTH / 2.0f > 0)
			{
				if (m_frameCnt > 10)
				{
					m_moveDir = -1;
					m_frameCnt = 0;	
				}
			}
			else
			{
				if (m_frameCnt > 10)
				{
					m_moveDir = 1;
					m_frameCnt = 0;
				}
			}
		}

		stickL.x = m_moveDir;
	}

	stickL.Normalize();


	m_frameCnt++;
	m_targetRestCnt ++;


	if (m_Hitstop.IsHitStop(m_body))
	{
		// ヒットストップ状態のときはキャラクターのヒットストップアニメーションだけ処理する
		m_pCharacter->SetAnimState(HITSTOP);
		m_pCharacter->Update();
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
	int smashDir = IsBringDown();
	if (smashDir) {
		//所持しているモノを削除
		m_holdObject = nullptr;

		switch (smashDir)
		{//左　右　上　下
		case 1:
			// 撃墜エフェクトを呼ぶ
			EffectManager::CreateEffect(SmashEffect, XMFLOAT2(-10.0f + 450 * 0.5, m_pos.y), XMFLOAT2(450.0f, 450.0f), 3.14f * 0.5f, 60, m_playerColor);
			break;
		case 2:
			// 撃墜エフェクトを呼ぶ
			EffectManager::CreateEffect(SmashEffect, XMFLOAT2(SCREEN_WIDTH + 10.0f - 450 * 0.5, m_pos.y), XMFLOAT2(450.0f, 450.0f), 3.14f * -0.5f, 60, m_playerColor);
			break;
		case 3:
			// 撃墜エフェクトを呼ぶ
			EffectManager::CreateEffect(SmashEffect, XMFLOAT2(m_pos.x, -10.0f + 450 * 0.5), XMFLOAT2(450.0f, 450.0f), 3.14f, 60, m_playerColor);
			break;
		case 4:
			// 撃墜エフェクトを呼ぶ
			EffectManager::CreateEffect(SmashEffect, XMFLOAT2(m_pos.x, SCREEN_HEIGHT + 10.0f - 450 * 0.5), XMFLOAT2(450.0f, 450.0f), 0.0f, 60, m_playerColor);
			break;
		default:
			break;
		}


		//SE再生
		AUDIO.PlayAudio(soundNum5, 0);

		// 残機を減らす
		m_lives--;

		// 残機が0以下なら
		if (m_lives > 0) {

			// 復活処理
			m_respawnStandby = true;
			m_body->SetTransform(b2Vec2(4000.0f, 4000.0f), 0.0f);//衝突判定を起こさないようずらす

			//バフ関連リセット
			m_moveDown = false;
			m_atkBuff = false;
			m_defBuff = false;
			m_invert = false;
			m_isFloating = false;

			m_invertFrame = 0;
			m_downFrame = 0;

			m_damage = 0;

			m_buffEffectUse = &m_initEffectFlag;
			m_debuffEffectUse = &m_initEffectFlag;

			m_isBlowed = false;


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
		if (m_debuffEffectUse == &m_moveDown)
			m_debuffEffectUse = &m_initEffectFlag;

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
		if (m_debuffEffectUse == &m_invert)
			m_debuffEffectUse = &m_initEffectFlag;

		m_invertFrame = 0;
	}
	if (m_invert)
	{
		m_invertFrame++;
	}

	//バフデバフのエフェクト管理
	//厳密にやるならフラグが切り替わった瞬間にエフェクト作ったほうがいい
	//flag下げるとき関数化しとけば良かっためんどい

	if (m_atkBuff && !*m_buffEffectUse)
	{
		//最後の引数がパターンを切り替えるまでのフレーム数　２だとわかりやすいけど遅い
		//１だとバフのエフェクトが白いから動いてるとわかりずらい
		EffectManager::CreateMoveEffect(BuffEffect, &m_pos, XMFLOAT2(300.0f, 300.0f), &m_rot, 0, XMFLOAT4(1.0f,1.0f,1.0f,1.0f),&m_atkBuff, 2);
		m_buffEffectUse = &m_atkBuff;
	}
	if (m_defBuff && !*m_buffEffectUse)
	{
		EffectManager::CreateMoveEffect(BuffEffect, &m_pos, XMFLOAT2(300.0f, 300.0f), &m_rot, 0, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), &m_defBuff, 2);
		m_buffEffectUse = &m_defBuff;
	}
	if (m_moveDown && !*m_debuffEffectUse)
	{
		EffectManager::CreateMoveEffect(DebuffEffect, &m_pos, XMFLOAT2(300.0f, 300.0f), &m_rot, 0, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), &m_moveDown, 2);
		m_debuffEffectUse = &m_moveDown;
	}
	if (m_invert && !*m_debuffEffectUse)
	{
		EffectManager::CreateMoveEffect(DebuffEffect, &m_pos, XMFLOAT2(300.0f, 300.0f), &m_rot, 0, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), &m_invert, 2);
		m_debuffEffectUse = &m_invert;
	}



	if (m_isBlowed)//吹っ飛びeffectの移動処理
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


		m_eRot = ang + XMConvertToRadians(90.0f);

	}



	//左右移動
	//ゲームパッドが接続されているか
	//if (m_gamePadNum >= 0) {
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
		LONG hor = (stickL.x * 1000.0f) * (m_invert ? -1 : 1);
	
		//コントローラー補正値
		float controllerCorrection = 0.0f;
		if (vel.x * hor < 0) {
			controllerCorrection = 0.3f;
		}
		else {
			controllerCorrection = 0.05f;
		}

		b2Vec2 force;
		//移動デバフかかってる場合*0.5f
			force = b2Vec2(hor * controllerCorrection * (m_moveDown ? DOWN_MAGNIFICATION : 1), 0.0f);
		

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
		m_throwVector.x = stickL.x * (m_invert ? -1 : 1);
		m_throwVector.y = stickL.y * (m_invert ? -1 : 1);
		if (m_throwVector.x == 0 && m_throwVector.y == 0) {
			m_throwVector = oldVec;
		}
		m_throwVector.Normalize();

	//}
	//else {
		//if (CTRL.GetKeyboardPress(DIK_A)) {

		//	m_body->ApplyForceToCenter(b2Vec2(-50.0f * (m_moveDown ? DOWN_MAGNIFICATION : 1) *
		//		(m_invert ? -1 : 1), 0.0f), true);

		//	m_pCharacter->IsCharacterFacingLeft(false);

		//	if (m_isGround && !(m_pCharacter->GetInterruptFlag())) {
		//		m_pCharacter->SetAnimState(MOVE);
		//	}

		//}
		//else if (CTRL.GetKeyboardPress(DIK_D)) {
		//	//デバフの補正
		//	m_body->ApplyForceToCenter(b2Vec2(50.0f * (m_moveDown ? DOWN_MAGNIFICATION : 1) *
		//		(m_invert ? -1 : 1), 0.0f), true);

		//	m_pCharacter->IsCharacterFacingLeft(true);

		//	if (m_isGround && !(m_pCharacter->GetInterruptFlag())) {
		//		m_pCharacter->SetAnimState(MOVE);
		//	}

		//}
		//else if (m_isGround && !(m_pCharacter->GetInterruptFlag())) {
		//	m_pCharacter->SetAnimState(IDLE);
		//}

		////投げる角度
		//static float throwAngle = 0.0f;
		//if (CTRL.GetKeyboardPress(DIK_RIGHTARROW)) {
		//	//throwAngle += m_invert ? -5.0f : 5.0f;//反転だけど全キャラ共通のためうまく動作しない
		//	throwAngle += 5.0f;
		//}
		//if (CTRL.GetKeyboardPress(DIK_LEFTARROW)) {
		//	//throwAngle -= m_invert ? -5.0f : 5.0f;;
		//	throwAngle -= 5.0f;;
		//}


		//m_throwVector.x = cosf(XMConvertToRadians(throwAngle));
		//m_throwVector.y = sinf(XMConvertToRadians(throwAngle));

		//m_throwVector.Normalize();
	//}

	if (m_holdObject && m_holdObject->CompareType("Balloon"))//風船持ったら浮くよー
	{
		m_body->ApplyForce(b2Vec2(0, -9.8f * m_body->GetMass() * 1.5f), m_body->GetWorldCenter(), true);
		if (!m_isFloating)
		{
			m_remainingJumps--;
			m_isFloating = true;
		}
	}
	else if (m_isFloating)
	{
		m_isFloating = false;
	}

	//ジャンプ
	//スペースキーかパッドの×ボタンが押されたか、かつジャンプフラグが立っていたら

	if (m_pos.y > 850.0f)//落下防止ジャンプ
	{
		m_jumpFlag = true;
	}

	if (m_jumpFlag && m_jumpCoolCnt > 30 && m_remainingJumps > 0) {

		//SE再生
		AUDIO.PlayAudio(soundNum, 0);

		// SE 再生（1回目 or 2回目のジャンプ）
		if (m_isGround) {
			AUDIO.PlayAudio(soundNum, 0);  // 地上ジャンプの音
		}
		else {
			AUDIO.PlayAudio(soundNum3, 0); // 空中ジャンプの音
		}

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
		m_jumpCoolCnt = 0;
	}
	//ジャンプカウント管理
	m_jumpCoolCnt++;
	if (m_jumpCoolCnt > 10000)
		m_jumpCoolCnt = 100;

	const int holdCooltime = 30;//これ定数のやつに
	if (!m_holdObject)
	{
		m_enterCoolTime = holdCooltime;//つかむまで
	}

	if (m_holdObject)
	{
		m_enterCnt++;
	}
	else if (!m_collisionObjects.empty())
	{
		m_enterCnt++;
	}


	//オブジェクトホールド
	if (m_enterCnt > m_enterCoolTime) {
		if (!m_collisionObjects.empty() && !m_holdObject) {

			AUDIO.PlayAudio(soundNum2, 0);


			m_holdObject = (*m_collisionObjects.begin());
			if (m_holdObject->Hold(m_body, this)) {
				m_collisionObjects.erase(m_collisionObjects.begin());

				// 持ちモーションをセット
				m_pCharacter->SetAnimState(HAVETHINGS);
				m_pCharacter->SetInterruptFlag(true);

				m_enterCnt = holdCooltime;//つかむまで
				m_enterCoolTime = rand() % 60 + 20;//最大投げるまで1秒遅延
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
				if (m_buffEffectUse == &m_atkBuff)
					m_buffEffectUse = &m_initEffectFlag;
			}

			

			bool isThrow = m_holdObject->Throw(x, y);//投げる

			if (isThrow) {
				if (m_holdObject->CompareType("AtkBuff"))//投げるオブジェクトのタイプでバフを
				{
					m_atkBuff = true;
				}
				if (m_holdObject->CompareType("DefBuff"))//
				{
					m_defBuff = true;
				}
				m_holdObject = nullptr;

				// 投げるモーションをセット
				m_pCharacter->SetAnimState(THROW);
				m_pCharacter->SetStopAnim(false);

				m_enterCnt = 0;
				m_enterCoolTime = 5;
			}

		}
	}

	//ターゲットのオブジェクトに色をつける
	if ((!m_collisionObjects.empty() && !m_holdObject) && !(*m_collisionObjects.begin())->IsExistsPlayer()) {
		(*m_collisionObjects.begin())->SetPlayerColor(m_playerColor);
	}
	else if (m_holdObject) {
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
			if (!m_holdObject)
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


	m_jumpFlag = false;
}



void EnemyCpu::OnCollisionEnter(GameObject* collision)
{
	Player::OnCollisionEnter(collision);

	if (collision->CompareTag("Player"))
	{
		if (m_pNum > ((Player*)collision)->GetPlayerNum())
		{
			m_jumpFlag = true;
			ReversKey = 5;
		}
		else//これポジション比較して方向考えればいいかも
			m_moveDir = (m_pos.x <= ((Player*)collision)->GetPos().x) ? -1 : 1;
	}

}


void EnemyCpu::CheckFixturesInSensor(b2Fixture* sensor) {
	b2AABB aabb;
	sensor->GetShape()->ComputeAABB(&aabb, sensor->GetBody()->GetTransform(), 0);

	QueryCallback callback;
	b2World* world = Physics::GetWorld();
	world->QueryAABB(&callback, aabb);

	float Num = 3000;

	for (b2Fixture* fixture : callback.foundFixtures) {
		if (fixture != sensor && fixture->IsSensor() == false) {
			// 範囲内に存在するフィクスチャが取得できます
			GameObject* GameO = reinterpret_cast<GameObject*>(fixture->GetBody()->GetUserData().pointer);//これ無駄かも
			if (!m_targetObject)
			{
				if (GameO->CompareTag("ThrowObject"))
				{
					ThrowObject* Buff = reinterpret_cast<ThrowObject*>(GameO);
					//if (!Buff->GetIsThrow() && !Buff->GetPlayerBool() && !Buff->cpuTarget)
					if (!Buff->GetIsThrow() && !Buff->GetPlayerBool())
					{
						XMFLOAT2 a = Buff->GetPos();

						if (Num > abs(a.x - m_pos.x) + abs(a.y - m_pos.y))
						{//近いやつ？
							Num = abs(a.x - m_pos.x) + abs(a.y - m_pos.y);
							m_targetObject = Buff;

						}
					}
				}
			}
			else
			{
				m_targetObject->cpuTarget[m_pNum - 1] = nullptr;
				if (GameO->CompareTag("ThrowObject"))
				{
					ThrowObject* Buff = reinterpret_cast<ThrowObject*>(GameO);
					//if (!Buff->GetIsThrow() && !Buff->GetPlayerBool() && !Buff->cpuTarget)
					if (!Buff->GetIsThrow() && !Buff->GetPlayerBool())
					{
						XMFLOAT2 a = Buff->GetPos();

						if (Num > abs(a.x - m_pos.x) + abs(a.y - m_pos.y))
						{//近いやつ？
							Num = abs(a.x - m_pos.x) + abs(a.y - m_pos.y);
							m_targetObject = Buff;

						}
					}
				}
			}
			if (m_targetRestCnt > 180)
			{
				if (GameO->CompareTag("Player"))
				{

					Player* Buff = reinterpret_cast<Player*>(GameO);
					XMFLOAT2 a = Buff->GetPos();

					if (Num > abs(a.x - m_pos.x) + abs(a.y - m_pos.y))
					{//これ動作確認してないからミスってるかも
						if (Buff->GetPlayerNum() != m_pNum)
						{
							m_targetP = Buff;
						}
					}

				}
			}

		}
	}
	if (m_targetObject)
		m_targetObject->cpuTarget[m_pNum - 1] = &m_targetObject;


	if (m_targetRestCnt > 180)
		m_targetRestCnt = 0;
}
