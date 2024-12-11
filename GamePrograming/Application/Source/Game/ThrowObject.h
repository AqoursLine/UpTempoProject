/******************************************************
* ThrowObject.h	スローオブジェクト
* 制作者：ミヤタジョウジ
* 作成日：2024/11/07
* 最終更新日：2024/11/07
*******************************************************/
#pragma once
#include "Game/GameObject.h"
#include "HitStop.h"

enum WEIGHT {
	WEIGHT_LIGHT = 1,
	WEIGHT_NORMAL,
	WEIGHT_HEAVY,
};

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

	const bool GetIsDelete() const { return m_isDelete; }

	const WEIGHT& GetWeight() const { return m_weight; }

	bool IsExistsPlayer() const { return m_player; }

	void Inpact(WEIGHT weighr);

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

private:
	b2Joint* m_joint = nullptr;

	//レボリュートジョイントによる回転用
	b2Joint* m_revJoint = nullptr;
	b2Body* m_revBody = nullptr;

	float m_targetAngle;
	bool m_isRotation = false;

	bool m_isThrowed = false;

	bool m_isDelete = false;
	bool m_isDeleteStandBy = false;
};
