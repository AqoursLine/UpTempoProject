/******************************************************
* Fish.cpp		�t�B�b�V��
* ����ҁF�Z���o�\��
* �쐬���F2024/11/07
* �ŏI�X�V���F2024/11/07
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "Game/Controller.h"

#include "Game/Fish.h"

/****************************************************
* �t�B�b�V��������
*****************************************************/

Fish::Fish()
{
	float x, y;
	x = rand() % SCREEN_WIDTH;
	y = rand() % SCREEN_HEIGHT;

	//�����ݒ�
	m_pos = XMFLOAT2(x, y);
	m_rot = 0.0f;
	m_size = XMFLOAT2(100.0f, 100.0f);

	//���W�ϊ�
	b2Vec2 pos = Physics::ConvertDXtoB2Float2(m_pos);
	//�{�f�B�쐬
	Physics::CreateBody(&m_body, pos.x, pos.y, m_rot, true, this);

	//���W�ϊ�
	b2Vec2 size = Physics::ConvertDXtoB2Float2(m_size);

	//�����蔻��쐬
	Physics::CreateFixture(&m_body, size.x, size.y, true);

	//��]����
	m_body->SetFixedRotation(true);

	//�e�N�X�`�����[�h
	m_tex.Load("Data/Texture/Fish.png");

	IsUse = true;
}

/****************************************************
* �t�B�b�V���I��
*****************************************************/
Fish::~Fish()
{

}

/****************************************************
* �t�B�b�V���X�V
*****************************************************/
void Fish::Update()
{
	if (IsUse) {
		//���W�X�V
		m_pos = Physics::ConvertB2toDXFloat2(m_body->GetPosition());
		m_rot = m_body->GetAngle();
	}
	
}


/****************************************************
* �t�B�b�V���`��
*****************************************************/
void Fish::Draw()
{
	if (IsUse) {
		//dx���W�ŕ`��
		D3D.Draw2D(m_tex, m_pos.x, m_pos.y, m_size.x, m_size.y, m_rot, 0.0f, 0.0f, 1.0f, 1.0f);
	}
	
}

void Fish::OnCollisionEnter(GameObject* collision)
{

	if (collision->CompareTag("Net")) {
		IsUse = false;
	}
}
