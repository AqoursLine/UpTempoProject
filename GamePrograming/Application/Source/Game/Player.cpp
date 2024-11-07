/******************************************************
* Player.cpp		�v���C���[
* ����ҁF�~���^�W���E�W
* �쐬���F2024/11/05
* �ŏI�X�V���F2024/11/05
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "Game/Controller.h"

#include "Game/Player.h"

/****************************************************
* �v���C���[������
*****************************************************/
Player::Player() {
	//�����ݒ�
	m_pos = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	m_rot = 0.0f;
	m_size = XMFLOAT2(100.0f, 100.0f);

	//���W�ϊ�
	b2Vec2 pos = Physics::ConvertDXtoB2Float2(m_pos);
	//�{�f�B�쐬
	Physics::CreateBody(&m_body, pos.x, pos.y, m_rot, true, this);

	//���W�ϊ�
	b2Vec2 size = Physics::ConvertDXtoB2Float2(m_size);


	//�����蔻��쐬
	Physics::CreateFixture(&m_body, size.x, size.y);

	//��]����
	m_body->SetFixedRotation(true);

	//�e�N�X�`�����[�h
	m_tex.Load("Data/Texture/icon.png");

	m_gamePadNum = CTRL.GetGamepadHandle();

	m_isJump = false;
}

/****************************************************
* �v���C���[�I��
*****************************************************/
Player::~Player() {
	
}

/****************************************************
* �v���C���[�X�V
*****************************************************/
void Player::Update() {
	//���W�X�V
	m_pos = Physics::ConvertB2toDXFloat2(m_body->GetPosition());
	m_rot = m_body->GetAngle();

	//���E�ړ�
	//�Q�[���p�b�h���ڑ�����Ă��邩
	if (m_gamePadNum >= 0) {
		b2Vec2 vel = m_body->GetLinearVelocity();
		vel.x = CTRL.GetLeftStickHorizontal(m_gamePadNum) * 0.01;
		m_body->SetLinearVelocity(vel);
	} else {
		if (CTRL.GetKeyboardPress(DIK_A)) {
			b2Vec2 vel = m_body->GetLinearVelocity();
			vel.x = -5;
			m_body->SetLinearVelocity(vel);
		} else if (CTRL.GetKeyboardPress(DIK_D)) {
			b2Vec2 vel = m_body->GetLinearVelocity();
			vel.x = 5;
			m_body->SetLinearVelocity(vel);
		} else {
			b2Vec2 vel = m_body->GetLinearVelocity();
			vel.x = 0;
			m_body->SetLinearVelocity(vel);
		}
	}

	//�W�����v
	if ((CTRL.GetKeyboardTrigger(DIK_SPACE) || CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CROSS, m_gamePadNum)) && m_isJump) {
		m_body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -7.0f), true);
		m_isJump = false;
	}
}

/****************************************************
* �v���C���[�`��
*****************************************************/
void Player::Draw() {
	//dx���W�ŕ`��
	D3D.Draw2D(m_tex, m_pos.x, m_pos.y, m_size.x, m_size.y, m_rot, 0.0f, 0.0f, 1.0f, 1.0f);
}

/****************************************************
* �v���C���[�����蔻��
*****************************************************/
void Player::OnCollisionEnter(GameObject* collision) {
	if (collision->CompareTag("Ground")) {
		m_isJump = true;
	}
}
