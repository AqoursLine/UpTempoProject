#pragma once

#include "Game/Character.h"

class Esper :public Character {
public:
	Esper();
	~Esper() = default;
	void Draw(XMFLOAT2 Pos, XMFLOAT2 Size, float rotate) override;
	void ChangePetternUV(ANIM_STATE currentState) override;
	Texture ReplaceTex() override;

private:
	static ANIM_TEX	m_allTex; // このキャラの全てのテクスチャを保存している構造体
};

