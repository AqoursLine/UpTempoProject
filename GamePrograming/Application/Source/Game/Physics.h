/******************************************************
* physics.h		�������Z
* ����ҁF�~���^�W���E�W
* �쐬���F2024/10/31
* �ŏI�X�V���F2024/10/31
*******************************************************/
#pragma once

#include <box2d/box2d.h>
#include "Game/GameObject.h"

constexpr float B2_TO_DX_RATE = 100.0f;

/****************************************************
* �Փ˃��X�i�[
*****************************************************/
class MyContactListener : public b2ContactListener {
public:
	void BeginContact(b2Contact* contact) override {
		GameObject* collisionA = reinterpret_cast<GameObject*>(contact->GetFixtureA()->GetUserData().pointer);
		GameObject* collisionB = reinterpret_cast<GameObject*>(contact->GetFixtureB()->GetUserData().pointer);

		collisionA->OnCollisionEnter(collisionB);
		collisionB->OnCollisionEnter(collisionA);
	}

	void EndContact(b2Contact* contact) override {
		GameObject* collisionA = reinterpret_cast<GameObject*>(contact->GetFixtureA()->GetUserData().pointer);
		GameObject* collisionB = reinterpret_cast<GameObject*>(contact->GetFixtureB()->GetUserData().pointer);

		collisionA->OnCollisionExit(collisionB);
		collisionB->OnCollisionExit(collisionA);
	}
};

/****************************************************
* �t�B�W�b�N�X�N���X
*****************************************************/
class Physics {
public:
	Physics() = delete;
	Physics(const float& gravityX, const float& gravityY);
	~Physics();

	void UpdatePhysics(const float& rate, const int32& vel, const int32& pos);

	static void CreateBody(b2Body** body, float x, float y, float r, bool isDynamic, GameObject* obj);
	static void CreateFixture(b2Body** body, float w, float h, bool IsTrigger, float density = 1.0f, float friction = 0.3f, float restitution = 0.0f);

	static b2Vec2 ConvertDXtoB2Float2(XMFLOAT2 dxf2);
	static XMFLOAT2 ConvertB2toDXFloat2(b2Vec2 b2v2);

	static b2World* GetWorld() {return m_world;}

private:
	static b2World* m_world;
	MyContactListener m_mcl;

};

