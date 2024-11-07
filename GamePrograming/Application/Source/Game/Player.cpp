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
	m_tex.Load("Data/Texture/player.png");

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
	//�{�f�B�̍��W��DX���W�ɕϊ�
	m_pos = Physics::ConvertB2toDXFloat2(m_body->GetPosition());
	m_rot = m_body->GetAngle();

	//���E�ړ�
	//�Q�[���p�b�h���ڑ�����Ă��邩
	if (m_gamePadNum >= 0) {
		//���݂̑��x���擾(�������̑��x�͂��̂܂܎g��������)
		b2Vec2 vel = m_body->GetLinearVelocity();
		//�p�b�h�̊p�x��␳���đ��x�ɑ��
		vel.x = CTRL.GetLeftStickHorizontal(m_gamePadNum) * 0.01;
		//���x��ύX
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
	//�X�y�[�X�L�[���p�b�h�́~�{�^���������ꂽ���A���W�����v�t���O�������Ă�����
	if ((CTRL.GetKeyboardTrigger(DIK_SPACE) || CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CROSS, m_gamePadNum)) && m_isJump) {
		//������ɗ͂�������
		m_body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -5.0f), true);
		m_isJump = false;
	}

	//�I�u�W�F�N�g�z�[���h
	if (CTRL.GetKeyboardTrigger(DIK_RETURN)) {
		if (m_collisionObject && !m_holdObject) {
			m_holdObject = m_collisionObject;
			m_holdObject->Hold(m_body);
		} else if (m_holdObject) {
			m_holdObject->Throw(5, -5);
			m_holdObject = nullptr;
		}
	}
}

/****************************************************
* �v���C���[�`��
*****************************************************/
void Player::Draw() {
	//dx���W�ŕ`��
	D3D.Draw2D(m_tex, m_pos.x, m_pos.y, m_size.x, m_size.y, m_rot, 0.1f, 0.1f, 0.8f, 0.8f);
}

/****************************************************
* �v���C���[�����蔻��
*****************************************************/
void Player::OnCollisionEnter(GameObject* collision) {
	if (collision->CompareTag("Ground")) {
		m_isJump = true;
	}

	if (collision->CompareTag("FieldObject")) {
		if (!m_collisionObject) {
			m_collisionObject = (FieldObject*)collision;
		}
	}
}

/****************************************************
* �v���C���[�����蔻�����
*****************************************************/
void Player::OnCollisionExit(GameObject* collision) {
	if (collision->CompareTag("FieldObject")) {
		if (m_collisionObject) {
			m_collisionObject = nullptr;
		}
	}
}

