/******************************************************
* Field.cpp	�t�B�[���h
* ����ҁF�~���^�W���E�W
* �쐬���F2024/11/05
* �ŏI�X�V���F2024/11/06
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"

#include "Game/Field.h"

/****************************************************
* �t�B�[���h������
*****************************************************/
Field::Field() {
	m_ground = new Ground();
	m_fieldObject = new FieldObject(FIELDOBJECTTYPE_NORMAL, SCREEN_WIDTH * 0.5f - 300.0f, SCREEN_HEIGHT * 0.5f + 200.0f, 100.0f, 100.0f, 0.0f);
}

/****************************************************
* �t�B�[���h�I��
*****************************************************/
Field::~Field() {
	if (m_ground) delete m_ground;
	if (m_fieldObject) delete m_fieldObject;
}

/****************************************************
* �t�B�[���h�X�V
*****************************************************/
void Field::Update() {
	m_ground->Update();
	m_fieldObject->Update();
}

/****************************************************
* �t�B�[���h�`��
*****************************************************/
void Field::Draw() {
	m_ground->Draw();
	m_fieldObject->Draw();
}
