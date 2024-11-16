#pragma once

#include "Game/GameObject.h"

class BackGround :public GameObject {
public:
	BackGround();
	~BackGround();

	void Update() override;
	void Draw() override;

private:
	XMFLOAT2 m_pos;
	XMFLOAT2 m_size;
	float m_rot;

	Texture m_tex;

	bool IsUse;
};