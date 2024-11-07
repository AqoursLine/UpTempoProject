/******************************************************
* Player.h		プレイヤー
* 制作者：ミヤタジョウジ
* 作成日：2024/11/05
* 最終更新日：2024/11/05
*******************************************************/
#pragma once

#include "Game/GameObject.h"
#include "Game/FieldObject.h"

/****************************************************
* プレイヤークラス
*****************************************************/
class Player : public GameObject {
public:
	Player();
	~Player();

	void Update() override;
	void Draw() override;

	void OnCollisionEnter(GameObject* collision) override;
	void OnCollisionExit(GameObject* collision) override;
	
private:
	//プレイやーのボディ
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
	FieldObject* m_collisionObject = nullptr;
	//所持しているモノ
	FieldObject* m_holdObject = nullptr;

};
