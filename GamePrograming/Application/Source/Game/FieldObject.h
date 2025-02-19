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
	CORNER,
};

/****************************************************
* フィールドオブジェクトクラス
*****************************************************/
class FieldObject : public GameObject {
public:
	FieldObject() = delete;
	FieldObject(const XMFLOAT2& pos, float rot, const XMFLOAT2& size, const std::wstring& fileName, const XMFLOAT2& texPos,const FIELD_DIRECTION fieldDirection);
	virtual ~FieldObject();

	virtual void Update() override;
	virtual void Draw() override;

	virtual void Attack(int attack);

protected:
	b2Body* m_body = nullptr;
	const XMFLOAT2 m_pos;
	const float m_rot;
	const XMFLOAT2 m_size;
	XMFLOAT2 m_texPos;
	XMFLOAT2 m_texSize;

	Texture m_tex;

	int m_hp = 0;

	FIELD_DIRECTION m_fieldDirection;

private:
};




