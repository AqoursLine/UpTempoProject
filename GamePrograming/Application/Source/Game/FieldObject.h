/******************************************************
* FieldObject.h	�t�B�[���h�I�u�W�F�N�g
* ����ҁF�~���^�W���E�W
* �쐬���F2024/11/07
* �ŏI�X�V���F2024/11/07
*******************************************************/
#pragma once
#include "Game/GameObject.h"

enum FIELDOBJECTTYPE {
	FIELDOBJECTTYPE_NORMAL = 0,
	FIELDOBJECTTYPE_HEAVY,
	FIELDOBJECTTYPE_LIGHT,
};

class FieldObject : public GameObject {
public:
	FieldObject() = delete;
	FieldObject(FIELDOBJECTTYPE type, float x, float y, float w, float h, float r);
	~FieldObject();

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

	bool m_isRotation;

};
