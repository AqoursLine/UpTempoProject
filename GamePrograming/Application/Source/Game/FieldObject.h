/******************************************************
* FieldObject.h		フィールドオブジェクト
* 制作者：ミヤタジョウジ
* 作成日：2024/11/14
* 最終更新日：2024/11/14
*******************************************************/
#pragma once

#include "Game/GameObject.h"

/****************************************************
* フィールドオブジェクトクラス
*****************************************************/
class FieldObject : public GameObject {
public:
	FieldObject() = delete;
	FieldObject(XMFLOAT2 pos, float rot, XMFLOAT2 size);
	~FieldObject();

	virtual void Update() override;
	virtual void Draw() override;

	virtual void Attack(int attack);

	bool GetIsDelete() const { return m_isDelete; }
protected:
	b2Body* m_body = nullptr;
	const XMFLOAT2 m_pos;
	const float m_rot;
	const XMFLOAT2 m_size;

	Texture m_tex;

private:
	int m_hp = 0;
	bool m_isDelete = false;
};


