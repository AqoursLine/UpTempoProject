/******************************************************
* Fish.h		フィッシュ
* 制作者：センバソラ
* 作成日：2024/11/07
* 最終更新日：2024/11/07
*******************************************************/
#pragma once

#include "Game/GameObject.h"

#define FISH_MAX (300)

/****************************************************
* フィッシュクラス
*****************************************************/
class Fish: public GameObject 
{
public:
	Fish();
	~Fish();

	void Update() override;
	void Draw() override;

	void OnCollisionEnter(GameObject* collision) override;

private:
	b2Body* m_body = nullptr;

	XMFLOAT2 m_pos;
	XMFLOAT2 m_size;
	float m_rot;

	Texture m_tex;

	bool IsUse;
};