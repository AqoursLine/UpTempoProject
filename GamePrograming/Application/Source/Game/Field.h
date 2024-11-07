/******************************************************
* Field.h	�t�B�[���h
* ����ҁF�~���^�W���E�W
* �쐬���F2024/11/05
* �ŏI�X�V���F2024/11/06
*******************************************************/
#pragma once
#include "Game/GameObject.h"
#include "Game/Ground.h"
#include "Game/FieldObject.h"

/****************************************************
* �t�B�[���h�N���X
*****************************************************/
class Field {
public:
	Field();
	~Field();

	void Update();
	void Draw();

private:
	Ground* m_ground;
	FieldObject* m_fieldObject;

};
