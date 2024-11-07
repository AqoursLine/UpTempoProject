/******************************************************
* Ground.h	�n��
* ����ҁF�~���^�W���E�W
* �쐬���F2024/11/07
* �ŏI�X�V���F2024/11/07
*******************************************************/
#pragma once

#include "Game/GameObject.h"

/****************************************************
* �n�ʃN���X
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
