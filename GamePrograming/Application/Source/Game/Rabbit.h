/******************************************************
* Rabbit.h		ウサギh
* 制作者：イササトル
* 作成日：2025/2/14
* 最終更新日：2025/2/14
*******************************************************/

#pragma once

#include "Game/Character.h"

class Rabbit :public Character {
public:
	Rabbit();
	~Rabbit() = default;
	void Draw(XMFLOAT2 Pos, XMFLOAT2 Size, float rotate) override;
	void ChangePetternUV(ANIM_STATE currentState) override;
	Texture ReplaceTex() override;

private:
	static ANIM_TEX	m_allTex; // このキャラの全てのテクスチャを保存している構造体
};
