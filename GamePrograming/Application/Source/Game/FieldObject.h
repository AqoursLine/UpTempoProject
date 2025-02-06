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
	FieldObject(XMFLOAT2 pos, float rot, XMFLOAT2 size,int uvNum);
	~FieldObject();

	virtual void Update() override;
	virtual void Draw() override;

	virtual void Attack(int attack);

protected:
	b2Body* m_body = nullptr;
	const XMFLOAT2 m_pos;
	const float m_rot;
	const XMFLOAT2 m_size;

	Texture m_tex;
	
	XMFLOAT2 m_uv;
	int m_uvNum;
	XMFLOAT2	m_texSize;	// 一区画の画像の幅を保存する

private:
	int m_hp = 0;
};


