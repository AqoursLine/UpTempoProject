/******************************************************
* Player.h		プレイヤー
* 制作者：ミヤタジョウジ
* 作成日：2024/11/05
* 最終更新日：2024/11/05
*******************************************************/
#pragma once

#include "Game/GameObject.h"
#include "Game/ThrowObject.h"
#include "Game/Character.h"

class ThrowObject;
enum WEIGHT;

/****************************************************
* プレイヤークラス
*****************************************************/
class Player : public GameObject {
public:
	Player(XMFLOAT2 startpos, int pnum);//12/4
	virtual ~Player();

	void PhysicsUpdate();
	void Update() override;
	void Draw() override;

	void CharacterUpdate();

	void OnCollisionEnter(GameObject* collision) override;
	void OnCollisionExit(GameObject* collision) override;

	const XMFLOAT2& GetPos()const { return m_pos; }//12/03追加(仙波）
	void BlowAway();	//12/03追加(仙波）
	void ApplyImpact(const b2Vec2& impactVector, WEIGHT weight,int damage);//12/03追加(仙波）//02/21引数にint追加

	//プレイヤーのポジション取得  12/4
	XMFLOAT2 GetPos() { return m_pos; };

	// プレイヤー番号のゲッター　追加日：12/27（担当：弓田）
	int GetPlayerNum() { return m_pNum; }

	// 復活処理 追加日：12/27（担当：弓田）
	void RespawnPlayer(XMFLOAT2 RespawnPos);

	// 撃墜されたか　追加日：12/27（担当：弓田）
	bool IsBringDown();

	//プレイヤーボディ作成
	void CreatePlayerBody();

	void SetNullHoldObject(ThrowObject* pObj);

	//デバフ用ゲッター・セッター 02/04
	bool GetMoveDown()const { return m_moveDown; }
	bool GetInvert()const { return m_invert; }
	bool GetDefBuff()const { return m_defBuff; }
	bool GetAtkBuff()const { return m_atkBuff; }
	void SetMoveDown(bool down) { m_moveDown = down; }
	void SetInvert(bool invert) { m_invert = invert; }
	//フレームカウントのリセット
	void ResetDownFrame() { m_downFrame = 0; }
	void ResetInvertFrame() { m_invertFrame = 0; }

	Player* GetTarget(void) { return m_targetP; }
	void SetNullTarget() { m_targetP = nullptr; }


protected:
	HitStop m_Hitstop;

//private:
	//プレイヤーのボディ
	b2Body* m_body = nullptr;
	
	//トランスフォーム
	XMFLOAT2 m_pos;
	XMFLOAT2 m_size;
	float m_rot;
	bool m_isRight = true;

	Character* m_pCharacter;

	//ゲームパッド番号
	int m_gamePadNum;

	//あとどれだけジャンプできるか（変更日：12/27 担当：弓田 変更内容：boolからintに変更）
	int m_remainingJumps;

	// 地面についているか
	bool m_isGround;

	// 残機（追加日：12/27 担当：弓田）
	int m_lives;

	float m_hp;

	//ダメージ蓄積用	02/01追加　中川
	int m_damage = 0;
	Texture m_damageTex[11];
	void DrawDamageNumber(const XMFLOAT2& pos, int damage);
	void LoadDamageTextures();
	XMFLOAT4 m_damageColor;

	//触れているモノ
	std::list<ThrowObject*> m_collisionObjects;
	//所持しているモノ
	ThrowObject* m_holdObject = nullptr;	
	//投げるベクトル
	b2Vec2 m_throwVector;
	//投げる補正値
	float m_throwPower = 10.0f;
	//投げる矢印
	Texture m_throwArrowTex;

	//吹っ飛ばす力
	b2Vec2 m_blowForce = b2Vec2(0.0f, 0.0f);	//12/03追加(仙波）
	bool m_isBlow = false;
	bool m_isBlowed = false;

	//プレイヤー番号  12/4
	int m_pNum;
	//プレイヤーカラー
	XMFLOAT4 m_playerColor;

	

	// 吹っ飛びはじめてからの時間
	float m_blowedTime;

	//フィルターネーム
	std::string m_filterName;


	//バフデバフのフラグ関係
	bool m_moveDown;	//移動速度低下フラグ
	bool m_atkBuff;		//投げる力増加
	bool m_defBuff;		//吹っ飛ばされにくく
	bool m_invert;		//操作反転フラグ
	int m_invertFrame;
	int m_downFrame;

	bool m_isFloating;//浮てるか

	//吹っ飛びエフェクトのテスト
	XMFLOAT2 m_ePos;//基本一つのエフェクトに一つ。ポジションを共有してもよい場合は複数のcreateMoveEffectに使ってもいい
	float m_eRot;
	bool efUse;//使用中かチェック。isBlowedが複数回呼ばれるかどうかわからないため


	static Texture m_charactorIcon;

	int soundNum;
	int soundNum2;
	int soundNum3;
	int soundNum4;
	int soundNum5;

	
	bool* m_buffEffectUse = nullptr;//バフデバフ同時に描画しても問題なさそうだから二つ
	bool* m_debuffEffectUse = nullptr;

	bool m_initEffectFlag = false;//エフェクト管理ポインタを初期化するための変数　絶対変更しない メンバに置きたくない

	int m_charactorUvX;

	Player* m_targetP = nullptr;//CPU用
};

// デバフの呼び方
// 適応させたいオブジェクトの.hに
// void HitPlayer(Player* p)override;
// を追加しその関数内で下のコードを呼ぶ
// 
////これ移動デバフ
//if (!p->GetMoveDown())
//p->SetMoveDown(true);
//else
//p->ResetDownFrame();
//
////これ反転デバフ
//if (!p->GetInvert())
//p->SetInvert(true);
//else
//p->ResetInvertFrame();
//
// 攻撃バフ、防御バフ、風船はオブジェクトごとのコンストラクタで
// SetType("AtkBuff");
// SetType("DefBuff");
// SetType("Balloon");
// のどれかをセットする
//

