/******************************************************
* Field.h	�t�B�[���h
* ����ҁF�~���^�W���E�W
* �쐬���F2024/11/05
* �ŏI�X�V���F2024/11/06
*******************************************************/
#pragma once
#include "Game/GameObject.h"

/****************************************************
* �t�B�[���h�N���X
*****************************************************/
class Field : public GameObject {
public:
	Field();
	~Field();

	void Update() override;
	void Draw() override;

private:
	b2Body* m_body = nullptr;
	XMFLOAT2 m_size;

	Texture tex;
};
