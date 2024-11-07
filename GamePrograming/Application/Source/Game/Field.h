/******************************************************
* Field.h	フィールド
* 制作者：ミヤタジョウジ
* 作成日：2024/11/05
* 最終更新日：2024/11/06
*******************************************************/
#pragma once
#include "Game/GameObject.h"
#include "Game/Ground.h"
#include "Game/FieldObject.h"

/****************************************************
* フィールドクラス
*****************************************************/
class Field {
public:
	Field();
	~Field();

	void Update();
	void Draw();

private:
	Ground* m_ground;
	FieldObject* m_fieldObject;

};
