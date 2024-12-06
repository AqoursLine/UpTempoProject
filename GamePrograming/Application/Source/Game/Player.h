/******************************************************
* Player.h		プレイヤー
* 制作者：ミヤタジョウジ
* 作成日：2024/11/05
* 最終更新日：2024/11/05
*******************************************************/
#pragma once

#include "Game/GameObject.h"
#include "Game/ThrowObject.h"


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

	//プレイヤーボディ作成
	void CreatePlayerBody();

protected:
	HitStop m_Hitstop;

private:
	

	//プレイヤーのボディ
	b2Body* m_body = nullptr;
	
	//トランスフォーム
	XMFLOAT2 m_pos;
	XMFLOAT2 m_size;
	float m_rot;

	//テクスチャ
	Texture m_tex;

	//ゲームパッド番号
	int m_gamePadNum;

	//ジャンプフラグ
	bool m_isJump;

	//触れているモノ
	ThrowObject* m_collisionObject = nullptr;
	//所持しているモノ
	ThrowObject* m_holdObject = nullptr;	

	//吹っ飛ばす力
	b2Vec2 m_blowForce = b2Vec2(0.0f, 0.0f);	//12/03追加(仙波）
	bool m_isBlow = false;
	bool m_isBlowed = false;

	//プレイヤー番号  12/4
	int m_pNum;

	//フィルターネーム
	std::string m_filterName;

};
