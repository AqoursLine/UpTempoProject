/******************************************************
* ThrowObject.h	スローオブジェクト
* 制作者：ミヤタジョウジ
* 作成日：2024/11/07
* 最終更新日：2024/11/07
*******************************************************/
#pragma once
#include "Game/GameObject.h"
#include "HitStop.h"
#include "Game/Player.h"


enum WEIGHT {
	WEIGHT_LIGHT = 1,
	WEIGHT_NORMAL,
	WEIGHT_HEAVY,
};

class Player;

class ThrowObject : public GameObject {
public:
	ThrowObject() = delete;
	ThrowObject(float x, float y, float r);
	virtual ~ThrowObject();

	virtual void Finalize();
	virtual void Update() override;
	virtual void Draw();

	virtual void OnCollisionEnter(GameObject* collision) override;

	virtual bool Throw(float vx, float vy);
	const bool Hold(b2Body* playerBody, GameObject* player);

	const WEIGHT& GetWeight() const { return m_weight; }

	// 追加日：12/27　担当：弓田
	const XMFLOAT2& GetPos() { return m_pos; }

	bool IsExistsPlayer() const { return m_player; }

	void Inpact(WEIGHT weighr);

	void SetPlayerColor(const XMFLOAT4&);

	bool GetIsThrow() const { return m_isThrowed; }//追加1/17

	virtual void HitPlayer(Player* p) {}

	virtual void HoldTiming() {};//1/31tuika

	// 02/18追加
	void SetIsThrow(bool isthrow) { m_isThrowed = isthrow; }
	void SetIsDeleteStandBy(bool isdelete) { m_isDeleteStandBy = isdelete; }


protected:
	Texture m_tex;

	XMFLOAT2 m_pos;
	float m_rot;
	XMFLOAT2 m_size;
	XMFLOAT2 m_uv;
	XMFLOAT2 m_texSize;

	b2Body* m_body;

	b2Vec2 m_ApplyImpact;

	HitStop m_HitStop;

	GameObject* m_player = nullptr;

	WEIGHT m_weight;

	int m_hitStopTotalFrame = 0;

	bool m_first = true;//追加01/17
	XMFLOAT2 m_throwPos;//

	float m_CollectionValue = 20.0f;//吹っ飛ばす時の補正値、オブジェクトごとに設定

	float m_sizeAdjust = 1.2f;//触れている場合の色に対するサイズの補正値

private:
	b2Joint* m_joint = nullptr;

	//レボリュートジョイントによる回転用
	b2Joint* m_revJoint = nullptr;
	b2Body* m_revBody = nullptr;
	float m_targetAngle;
	bool m_isRotation = false;

	//投げられたフラグ
	bool m_isThrowed = false;

	//消すフラグ
	bool m_isDeleteStandBy = false;

	//プレイヤーのターゲット
	bool m_isPlayerCollision = false;
	XMFLOAT4 m_playerColor;
};

