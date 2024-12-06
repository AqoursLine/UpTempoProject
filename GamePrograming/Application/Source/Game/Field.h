/******************************************************
* Field.h	フィールド
* 制作者：ミヤタジョウジ
* 作成日：2024/11/05
* 最終更新日：2024/11/06
*******************************************************/
#pragma once
#include "Game/GameObject.h"
#include "Game/ThrowObject.h"

/****************************************************
* フィールドクラス
*****************************************************/
class Field {
public:
	Field();
	~Field();

	void Update();
	void Draw();

	void Attack(int attack);

	void LoadBackGround(std::string fileName);

private:
	int m_HP;

	Texture m_bgTex;
	Texture m_hpTex;

};
