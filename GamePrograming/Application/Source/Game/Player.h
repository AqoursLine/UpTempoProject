/******************************************************
* Player.h		�v���C���[
* ����ҁF�~���^�W���E�W
* �쐬���F2024/11/05
* �ŏI�X�V���F2024/11/05
*******************************************************/
#pragma once

#include "Game/GameObject.h"
#include "Game/FieldObject.h"

/****************************************************
* �v���C���[�N���X
*****************************************************/
class Player : public GameObject {
public:
	Player();
	~Player();

	void Update() override;
	void Draw() override;

	void OnCollisionEnter(GameObject* collision) override;
	void OnCollisionExit(GameObject* collision) override;
	
private:
	//�v���C��[�̃{�f�B
	b2Body* m_body = nullptr;
	
	//�g�����X�t�H�[��
	XMFLOAT2 m_pos;
	XMFLOAT2 m_size;
	float m_rot;

	//�e�N�X�`��
	Texture m_tex;

	//�Q�[���p�b�h�ԍ�
	int m_gamePadNum;

	//�W�����v�t���O
	bool m_isJump;

	//�G��Ă��郂�m
	FieldObject* m_collisionObject = nullptr;
	//�������Ă��郂�m
	FieldObject* m_holdObject = nullptr;

};
