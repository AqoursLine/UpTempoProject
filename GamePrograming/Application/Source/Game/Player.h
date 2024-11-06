/******************************************************
* Player.h		�v���C���[
* ����ҁF�~���^�W���E�W
* �쐬���F2024/11/05
* �ŏI�X�V���F2024/11/05
*******************************************************/
#pragma once

#include "Game/GameObject.h"

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

private:
	b2Body* m_body = nullptr;

	XMFLOAT2 m_pos;
	XMFLOAT2 m_size;
	float m_rot;

	Texture m_tex;

	int m_gamePadNum;

	bool m_isJump;
};
