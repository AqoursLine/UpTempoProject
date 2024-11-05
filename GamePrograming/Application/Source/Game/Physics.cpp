/******************************************************
* physics.h		�������Z
* ����ҁF�~���^�W���E�W
* �쐬���F2024/10/31
* �ŏI�X�V���F2024/10/31
*******************************************************/
#include "physics.h"

b2World* Physics::m_world = nullptr;


/****************************************************
* �R���X�g���N�^
*****************************************************/
Physics::Physics(const float& gravityX, const float& gravityY) {
	//���[���h�쐬
	b2Vec2 gravity(gravityX, gravityY);
	m_world = new b2World(gravity);

	//�����蔻�胊�X�i�[
	m_world->SetContactListener(&mcl);

}

/****************************************************
* �f�X�g���N�^
*****************************************************/
Physics::~Physics() {
	delete m_world;
}

/****************************************************
* ���[���h�X�V
*****************************************************/
void Physics::UpdatePhysics(const float& rate, const int32& vel, const int32& pos) {
	m_world->Step(rate, vel, pos);
}
