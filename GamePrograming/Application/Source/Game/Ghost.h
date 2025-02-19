/******************************************************
* Ghost.h		ゴーストh
* 制作者：イササトル
* 作成日：2025/2/15
* 最終更新日：2025/2/15
*******************************************************/

#pragma once

#include "Game/Character.h"

class Ghost :public Character {
public:
	Ghost();
	~Ghost() = default;
	void Draw(XMFLOAT2 Pos, XMFLOAT2 Size, float rotate) override;
	void ChangePetternUV(ANIM_STATE currentState) override;
	Texture ReplaceTex() override;

private:
	static ANIM_TEX	m_allTex; // このキャラの全てのテクスチャを保存している構造体
};
