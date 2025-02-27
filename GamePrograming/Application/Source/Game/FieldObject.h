/******************************************************
* FieldObject.h		フィールドオブジェクト
* 制作者：ミヤタジョウジ
* 作成日：2024/11/14
* 最終更新日：2024/11/14
*******************************************************/
#pragma once

#include "Game/GameObject.h"

enum FIELD_DIRECTION {
	TOP = 0,
	LEFT,
	RIGHT,
	BOTTOM,
	LEFTCORNER,
	RIGHTCORNER,
};

/****************************************************
* フィールドオブジェクトクラス
*****************************************************/
class FieldObject : public GameObject {
public:
	FieldObject() = delete;
	FieldObject(const XMFLOAT2& pos, float rot, const XMFLOAT2& size, const XMFLOAT2& offset, int uvNum, const FIELD_DIRECTION fieldDirection);
	virtual ~FieldObject();

	virtual void Update() override;
	virtual void Draw() override;

	virtual void Attack(int attack);

	XMFLOAT2 GetPos() { return m_pos; }
	FIELD_DIRECTION GetDirection() { return m_fieldDirection; }
protected:
	b2Body* m_body = nullptr;
	XMFLOAT2 m_pos;
	float m_rot;
	XMFLOAT2 m_size;
	XMFLOAT2 m_texSize;

	XMFLOAT2 m_uvPos;
	XMFLOAT2 m_uvSize;

	std::vector<std::unique_ptr<Texture>> m_tex;

	int m_hp = 0;
	int m_maxHp = 0;

	int m_texNum = 0;

	FIELD_DIRECTION m_fieldDirection;

private:
};




