/******************************************************
* Beautiful.h		美少女h
* 制作者：イササトル
* 作成日：2025/2/14
* 最終更新日：2025/2/14
*******************************************************/

#pragma once

#include "Game/Character.h"

class Beautiful :public Character {
public:
	Beautiful();
	~Beautiful() = default;
	void Draw(XMFLOAT2 Pos, XMFLOAT2 Size, float rotate) override;
	void ChangePetternUV(ANIM_STATE currentState) override;
	Texture ReplaceTex() override;

private:
	static ANIM_TEX	m_allTex; // このキャラの全てのテクスチャを保存している構造体
};
