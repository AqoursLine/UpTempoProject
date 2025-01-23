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

/****************************************************
* プレイヤークラス
*****************************************************/
class Player : public GameObject {
public:
	Player(XMFLOAT2 startpos, int pnum);//12/4
	~Player();

	
	void Update() override;
	void Draw() override;

	void OnCollisionEnter(GameObject* collision) override;
	void OnCollisionExit(GameObject* collision) override;

	const XMFLOAT2& GetPos()const { return m_pos; }//12/03追加(仙波）
	void BlowAway();	//12/03追加(仙波）
	void ApplyImpact(const b2Vec2& impactVector);//12/03追加(仙波）

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

	void SetNullHoldObject() { m_holdObject = nullptr; }

protected:
	HitStop m_Hitstop;

private:
	//プレイヤーのボディ
	b2Body* m_body = nullptr;
	
	//トランスフォーム
	XMFLOAT2 m_pos;
	XMFLOAT2 m_size;
	float m_rot;
	bool m_isRight = true;

	//テクスチャ
	Texture m_tex;

	Character* m_pCharacter;

	//ゲームパッド番号
	int m_gamePadNum;

	//あとどれだけジャンプできるか（変更日：12/27 担当：弓田 変更内容：boolからintに変更）
	int m_remainingJumps;

	// 地面についているか
	bool m_isGround;

	// 残機（追加日：12/27 担当：弓田）
	int m_lives;

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

};
