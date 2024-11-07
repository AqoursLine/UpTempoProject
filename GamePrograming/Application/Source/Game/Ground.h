/******************************************************
* Ground.h	地面
* 制作者：ミヤタジョウジ
* 作成日：2024/11/07
* 最終更新日：2024/11/07
*******************************************************/
#pragma once

#include "Game/GameObject.h"

/****************************************************
* 地面クラス
*****************************************************/
class Ground : GameObject {
public:
	Ground();
	~Ground();

	void Update() override;
	void Draw() override;

private:
	b2Body* m_body = nullptr;
	XMFLOAT2 m_size;

	Texture m_tex;
};
