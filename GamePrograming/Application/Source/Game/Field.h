/******************************************************
* Field.h	フィールド
* 制作者：ミヤタジョウジ
* 作成日：2024/11/05
* 最終更新日：2024/11/06
*******************************************************/
#pragma once
#include "Game/GameObject.h"

/****************************************************
* フィールドクラス
*****************************************************/
class Field : public GameObject {
public:
	Field();
	~Field();

	void Update() override;
	void Draw() override;

private:
	b2Body* m_body = nullptr;
	XMFLOAT2 m_size;

	Texture tex;
};
