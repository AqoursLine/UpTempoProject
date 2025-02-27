/******************************************************
* Player.cpp		プレイヤー
* 制作者：ミヤタジョウジ
* 作成日：2024/11/05
* 最終更新日：2024/11/05
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "DirectX/Audio.h"
#include "Game/Physics.h"
#include "Game/Controller.h"
#include "Game/Player.h"
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

Texture Player::m_charactorIcon;

/****************************************************
* プレイヤー初期化
*****************************************************/
Player::Player(XMFLOAT2 startpos,int pnum) {
	//初期設定
	m_pos = startpos;//12/4
	m_rot = 0.0f;
	m_size = XMFLOAT2(140.0f * 1.8f, 140.0f * 1.8f); // もっと大きくする必要あり
	m_pNum = pnum;
	m_blowedTime = 0.0f;

	////	プレイヤーごとのダメージ表示の色	02/21追加	中川
	//switch (m_pNum) {
	//case 1:
	//	m_damageColor = XMFLOAT4(1.0f, 0.3f, 0.2f, 1.0f);	//	赤
	//	break;
	//case 2:
	//	m_damageColor = XMFLOAT4(0.2f, 0.2f, 1.0f, 1.0f);	//	青
	//	break;
	//case 3:
	//	m_damageColor = XMFLOAT4(0.2f, 1.0f, 0.2f, 1.0f);	//	緑
	//	break;
	//case 4:
	//	m_damageColor = XMFLOAT4(1.0f, 1.0f, 0.2f, 1.0f);	//	黄
	//	break;

	//default:
	//	m_damageColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	//	白
	//}

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

	m_invertFrame = 0;
	m_downFrame = 0;

	//初期化はバフデバフのフラグなら何でもいい？
	m_buffEffectUse = &m_initEffectFlag;
	m_debuffEffectUse = &m_initEffectFlag;

	CreatePlayerBody();
	LoadDamageTextures();

	m_charactorUvX = m_pNum - 1;

	//プレイヤーデータ取得
	PlayerData pData = SaveData::GetPlayerData(m_pNum);

	//テクスチャロード
	switch (pData.PlayerNum) {
		case 1:
			m_playerColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		case 2:
			m_playerColor = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			break;
		case 3:
			m_playerColor = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			break;
		case 4:
			m_playerColor = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
			break;
		default:
			m_playerColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			break;
	}

	m_gamePadNum = pData.PadNum;
	

	// キャラクターをセット
	switch (pData.charactorNum) {
		case CHARACTOR_IKEMEN:
			m_pCharacter = new Handsome();
			break;
		case CHARACTOR_NEKKETSU:
			m_pCharacter = new Bancho();
			break;
		case CHARACTOR_FOX:
			m_pCharacter = new Ghost();
			break;
		case CHARACTOR_WOMAN:
			m_pCharacter = new Esper();
			break;
		case CHARACTOR_BISYOUZYO:
			m_pCharacter = new Beautiful();
			break;
		case CHARACTOR_RABBIT:
			m_pCharacter = new Rabbit();
			break;
		default:
			break;
	}

	m_throwArrowTex.Load(L"Data/Texture/throwArrow.png");

	if(m_pNum==1)//pNumが1の実体から生成されること前提になってる
	m_charactorIcon.Load(L"Data/Texture/Character_icon_UV.png");


	m_throwVector.Set(5, -5);

	SetTag("Player");

	LoadDamageTextures();

	//SE読み込み
	soundNum = AUDIO.LoadWaveFile("Data/Sound/SE/スイング05.wav");	//ジャンプ音
	soundNum2 = AUDIO.LoadWaveFile("Data/Sound/SE/ぶつかる02.wav");	//物をもつ音
	soundNum3 = AUDIO.LoadWaveFile("Data/Sound/SE/スイング07.wav");	//空中ジャンプ音
	soundNum4 = AUDIO.LoadWaveFile("Data/Sound/SE/打撃6.wav");	//外枠に当たる音
	soundNum5 = AUDIO.LoadWaveFile("Data/Sound/SE/K.O.wav");	//外枠に当たる音

	AUDIO.SetVolume(soundNum, 1.0f);
	AUDIO.SetVolume(soundNum2, 1.0f);
	AUDIO.SetVolume(soundNum3, 1.0f);
	AUDIO.SetVolume(soundNum4, 1.0f);
	AUDIO.SetVolume(soundNum5, 1.0f);

}

/****************************************************
* プレイヤー終了
*****************************************************/
Player::~Player() {
	if (m_holdObject) {
		m_holdObject->SetNullPlayer();
	}

	Physics::GetWorld()->DestroyBody(m_body);
	AUDIO.StopAudio(soundNum);
	AUDIO.StopAudio(soundNum2);
	AUDIO.StopAudio(soundNum3);
	AUDIO.StopAudio(soundNum4);
	AUDIO.StopAudio(soundNum5);
}

/****************************************************
* プレイヤー更新
*****************************************************/
void Player::Update() {

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
	if (IsBringDown()) {
		//所持しているモノを削除
		m_holdObject = nullptr;

		// 撃墜エフェクトを呼ぶ


		//SE再生
		AUDIO.PlayAudio(soundNum5, 0);

		// 残機を減らす
		m_lives--;

		// 残機が0以下なら
		if (m_lives>0) {

			// 復活処理
			RespawnPlayer(XMFLOAT2(static_cast<float>(320 * m_pNum), static_cast<float>(SCREEN_HEIGHT / 2)));//プレイヤーの総人数から調整する場合は320を1920/(2+総プレイヤー数)
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
		if(m_debuffEffectUse == &m_moveDown)
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

	if (m_atkBuff&&!*m_buffEffectUse)
	{
		//最後の引数がパターンを切り替えるまでのフレーム数　２だとわかりやすいけど遅い
		//１だとバフのエフェクトが白いから動いてるとわかりずらい
		EffectManager::CreateMoveEffect(BuffEffect, &m_pos, XMFLOAT2(300.0f, 300.0f), &m_rot, 0, &m_atkBuff, 2);
		m_buffEffectUse = &m_atkBuff;
	}
	if (m_defBuff && !*m_buffEffectUse)
	{
		EffectManager::CreateMoveEffect(BuffEffect, &m_pos, XMFLOAT2(300.0f, 300.0f), &m_rot, 0, &m_defBuff, 2);
		m_buffEffectUse = &m_defBuff;
	}
	if (m_moveDown && !*m_debuffEffectUse)
	{
		EffectManager::CreateMoveEffect(DebuffEffect, &m_pos, XMFLOAT2(300.0f, 300.0f), &m_rot, 0, &m_moveDown, 2);
		m_debuffEffectUse = &m_moveDown;
	}
	if (m_invert && !*m_debuffEffectUse)
	{
		EffectManager::CreateMoveEffect(DebuffEffect, &m_pos, XMFLOAT2(300.0f, 300.0f), &m_rot, 0, &m_invert, 2);
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
		}
		else {
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

	}
	else {
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
	}

	//オブジェクトホールド
	if (CTRL.GetKeyboardTrigger(DIK_RETURN) || CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_SQUARE, m_gamePadNum)) {
		if (!m_collisionObjects.empty() && !m_holdObject) {

			AUDIO.PlayAudio(soundNum2, 0);


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

void Player::DrawDamageNumber(const XMFLOAT2& pos, int damage)
{
	std::string damageText = std::to_string(damage) + "%";

	float digitSpacing = 35.0f;	//数字の間隔
	float percentSpacing = 80.0f;
	XMFLOAT2 digitSize = XMFLOAT2(45.0f, 45.0f);	//数字の大きさ	//ここいじった村山
	XMFLOAT2 percentSize = XMFLOAT2(45.0f, 45.0f);

	float currentX = pos.x;	//x座標の開始位置

	float redIntensity = std::min<float>(1.0f, damage / 100.0f);
	XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f - redIntensity, 1.0f - redIntensity, 1.0f);

	for (size_t i = 0; i < damageText.size(); i++)
	{
		int index = (damageText[i] == '%') ? 10 : (damageText[i] - '0');

		XMFLOAT2 drawSize = (damageText[i] == '%') ? percentSize : digitSize;
		float spacing = (damageText[i] == '%') ? percentSpacing : digitSpacing;

		D3D.Draw2D(m_damageTex[index], XMFLOAT2(currentX, pos.y), drawSize, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), color);

		currentX += spacing;
	}
}


/****************************************************
* プレイヤー描画
*****************************************************/
void Player::Draw() {
	

	//dx座標で描画
	//D3D.Draw2D(m_tex, m_pos, m_size, m_rot);
	m_pCharacter->Draw(m_pos, m_size, m_rot);

	int numDigits = static_cast<int>(std::to_string(m_damage).size());	//ダメージの桁数を取得
	float digitWidth = 35.0f;	//各数字の幅
	float percentWidth = 55.0f;	//%の幅
	float totalWidth = numDigits * digitWidth + percentWidth;	//数字＋%の合計値  

	//ダメージ表示の開始位置（プレイヤーごとに等間隔に並べる）
	//float baseX = static_cast<float>(200 + (m_pNum - 1) * 350);//ここもいじった村山
	float baseX = 300.0f;
	float maxSpacing = 300.0f;
	float minSpacing = 170.0f;
	//float adjustedX = baseX - totalWidth / 2;	//
	int totalPlayers = 4;


	float spacing = maxSpacing - (totalPlayers - 1) * 15.0f;
	spacing = std::max<float>(spacing, maxSpacing);

	float baseY = 80.0f;

	//XMFLOAT2 damagePos = XMFLOAT2(adjustedX, 100);
	XMFLOAT2 damagePos = XMFLOAT2(baseX + (m_pNum - 1) * spacing, baseY);//ここもいじった村山		02/21変更	中川
	

	//icon描画
	{//CPUの処理はCPUのclassで作る
		int charactorNum = SaveData::GetPlayerData(m_pNum).charactorNum;//セーブデータから直で取得

		XMFLOAT2 iconSize(1689.0f * 0.2f, 1069.0f * 0.2f);//これで良さげ
		
		XMFLOAT2 iconUvSize(1.0f / 5.0f, 1.0f / 6.0f);

		XMFLOAT2 iconUv;
		iconUv.x = iconUvSize.x * (m_charactorUvX);//CPUはここ固定で4
		iconUv.y = iconUvSize.y * (charactorNum - 1);
		XMFLOAT2 iconPos(damagePos.x - 30.0f, damagePos.y);
		D3D.Draw2D(m_charactorIcon, iconPos, iconSize, 0, iconUv, iconUvSize);
	}

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

		float erot = 0.0f;
		switch (((FieldObject*)collision)->GetDirection())
		{
		case LEFT:
			erot = -90.0f;
			break;
		case RIGHT:
			erot = 90.0f;
			break;
		case TOP:
			erot = 0.0f;
			break;
		case LEFTCORNER:
			erot = -45.0f;
			break;
		case RIGHTCORNER:
			erot = 45.0f;
			break;

		default:
			break;
		}
		erot = XMConvertToRadians(erot);
		// 壁損傷エフェクト
		EffectManager::CreateEffect(WallDesEffect, ((FieldObject*)collision)->GetPos(), XMFLOAT2(300.0f, 300.0f), erot,60.0f);

		int damage = 5;
		((FieldObject*)collision)->Attack(damage);

		m_isBlowed = false;

		//エフェクト再使用可能に
		efUse = false;

		if (!m_holdObject) {
			m_pCharacter->SetInterruptFlag(false);
		}
		else {
			m_pCharacter->SetAnimState(HAVETHINGS);
		}

		AUDIO.PlayAudio(soundNum4, 0);


	}

	if (collision->CompareTag("Lamp") && m_isBlowed) {//蛍光灯

		// エフェクト
		EffectManager::CreateEffect(PlayerHitWall, m_pos, XMFLOAT2(600.0f, 600.0f), 0.0f);

		
		((Lamp*)collision)->AddDamage();

		m_isBlowed = false;

		//エフェクト再使用可能に
		efUse = false;

		if (!m_holdObject) {//これよくわかってない
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
			}
			else {
				++itr;
			}
		}
	}

	if (collision->CompareTag("Ground") || collision->CompareTag("ThrowObject")) {
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


		AUDIO.PlayAudio(soundNum3, 0);

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
void Player::ApplyImpact(const b2Vec2& impactVector, WEIGHT weight,int damage)
{
	int damageAmount = damage;//スローオブジェクトの持つダメージをそのまま

	////ダメージ量を重さで変える		02・01追加	中川
	//switch (weight) {
	//case WEIGHT_LIGHT:
	//	damageAmount = 5;
	//	break;
	//case WEIGHT_NORMAL:
	//	damageAmount = 10;
	//	break;
	//case WEIGHT_HEAVY:
	//	damageAmount = 20;
	//	break;
	//}
	m_damage += static_cast<int>(damageAmount * (m_defBuff ? 0.5f : 1.0f));

	//ダメージに応じて吹っ飛ぶ力を増加　最大三倍	中川
	float impactScale = 1.0f + (m_damage * 0.02f);
	impactScale = min(impactScale, 3.0f) * (m_defBuff ? 0.5f : 1.0f);	//	最大3倍


	b2Vec2 adjustedImpact = b2Vec2(impactVector.x * impactScale, impactVector.y * impactScale);

	//ヒットストップフラグ立てる
	int hitStopFrame = 0;

	if (weight == WEIGHT_LIGHT) {
		hitStopFrame = 5;
	}
	else if (weight == WEIGHT_NORMAL) {
		hitStopFrame = 10;
	}
	else if (weight == WEIGHT_HEAVY) {
		hitStopFrame = 15;
	}
	m_Hitstop.SetIsHitStop(true, hitStopFrame);

	//渡されたベクトルをメンバ変数に格納
	m_blowForce = adjustedImpact;



	m_isBlow = true;

	m_defBuff = false;
	if(m_buffEffectUse == &m_defBuff)
	m_buffEffectUse = &m_initEffectFlag;
	
	m_pCharacter->SetInterruptFlag(true); // モーションの割り込みフラグを立てる
	// Updateの一番上に書いてある、
	// if (m_Hitstop.IsHitStop(m_body))で書いても良いんだけど,それだと毎回trueにして無駄だからここでやっちゃう。
	// 可読性はごめにょ。
}

/******************************************************
* 復活関数	（追加日：12/27　担当：弓田）
*******************************************************/
void Player::RespawnPlayer(XMFLOAT2 RespawnPos)
{
	//m_body->SetTransform(Physics::ConvertDXtoB2Float2(RespawnPos), 0.0f);

	ThrowObjectManager::PushRespawnScaffold(RespawnPos.x, RespawnPos.y, m_pNum);
	m_body->SetTransform(Physics::ConvertDXtoB2Float2(XMFLOAT2(RespawnPos.x, RespawnPos.y - 100)), 0.0f);
	m_body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	m_body->SetAngularVelocity(0.0f);

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
		filter.categoryBits = static_cast<uint16>(std::hash<std::string>{} (m_filterName));
		fixture->SetFilterData(filter);
		fixture = fixture->GetNext();
	}

	//保持しているものを破棄
	m_collisionObjects.clear();
	m_holdObject = nullptr;
}

void Player::SetNullHoldObject(ThrowObject* pObj)
{
	if (m_holdObject != pObj) {
		return;
	}
	m_holdObject = nullptr;
	m_pCharacter->SetInterruptFlag(false);
	m_pCharacter->SetAnimState(IDLE);
}


