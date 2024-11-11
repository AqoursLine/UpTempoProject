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
	ThrowObject(FIELDOBJECTTYPE type, float x, float y, float w, float h, float r);
	~ThrowObject();

	void Update() override;
	void Draw() override;

	void Throw(float vx, float vy);
	void Hold(b2Body* playerBody);
 
private:
	FIELDOBJECTTYPE m_type;

	XMFLOAT2 m_pos;
	float m_rot;
	XMFLOAT2 m_size;

	b2Body* m_body;

	Texture m_tex;

	b2Joint* m_joint = nullptr;

	float m_targetAngle;
	bool m_isRotation;

};
