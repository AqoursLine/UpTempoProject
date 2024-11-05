/******************************************************
* physics.h		�������Z
* ����ҁF�~���^�W���E�W
* �쐬���F2024/10/31
* �ŏI�X�V���F2024/10/31
*******************************************************/
#pragma once

#include <box2d/box2d.h>

constexpr float B2_TO_DX_RATE = 100.0f;

/****************************************************
* �Փ˃��X�i�[
*****************************************************/
class MyContactListener : public b2ContactListener {
public:
	void BeginContact(b2Contact* contact) override {
		
	}

	void EndContact(b2Contact* contact) override {

	}
};

class Physics {
public:
	Physics() = delete;
	Physics(const float& gravityX, const float& gravityY);
	~Physics();

	void UpdatePhysics(const float& rate, const int32& vel, const int32& pos);

	static b2World* GetWorld() {return m_world;}

private:
	static b2World* m_world;
	MyContactListener m_mcl;

};

