/******************************************************
* ThrowObject.h	スローオブジェクト
* 制作者：ミヤタジョウジ
* 作成日：2024/11/07
* 最終更新日：2024/11/07
*******************************************************/
#pragma once
#include "Game/GameObject.h"
#include "HitStop.h"

enum FIELDOBJECTTYPE {
	FIELDOBJECTTYPE_NORMAL = 0,
	FIELDOBJECTTYPE_HEAVY,
	FIELDOBJECTTYPE_LIGHT,
};

class ThrowObject : public GameObject {
public:
	ThrowObject() = delete;
	ThrowObject(float x, float y, float r) : m_pos(XMFLOAT2(x, y)), m_rot(r) {m_ApplyImpact = {20.0f, -20.0f};}
	virtual ~ThrowObject();

	virtual void Update() override;
	virtual void Draw();

	void OnCollisionEnter(GameObject* collision) override;

	virtual bool Throw(float vx, float vy);
	bool Hold(b2Body* playerBody, GameObject* player);
 
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

private:
	b2Joint* m_joint = nullptr;

	//レボリュートジョイントによる回転用
	b2Joint* m_revJoint = nullptr;
	b2Body* m_revBody = nullptr;

	float m_targetAngle;
	bool m_isRotation = false;

	bool m_isThrowed = false;

};
