/******************************************************
* ThrowObject.h	フィールドオブジェクト
* 制作者：ミヤタジョウジ
* 作成日：2024/11/07
* 最終更新日：2024/11/07
*******************************************************/
#pragma once
#include "Game/GameObject.h"

enum FIELDOBJECTTYPE {
	FIELDOBJECTTYPE_NORMAL = 0,
	FIELDOBJECTTYPE_HEAVY,
	FIELDOBJECTTYPE_LIGHT,
};

class ThrowObject : public GameObject {
public:
	ThrowObject() = delete;
	ThrowObject(float x, float y, float r) : m_pos(XMFLOAT2(x, y)), m_rot(r) {}
	virtual ~ThrowObject();

	virtual void Update() override;
	virtual void Draw() = 0;

	virtual void Throw(float vx, float vy);
	void Hold(b2Body* playerBody);
 
protected:
	Texture m_tex;

	XMFLOAT2 m_pos;
	float m_rot;
	XMFLOAT2 m_size;

	b2Body* m_body;
private:
	b2Joint* m_joint = nullptr;

	float m_targetAngle;
	bool m_isRotation = false;



};
