/******************************************************
* physics.h		物理演算
* 制作者：ミヤタジョウジ
* 作成日：2024/10/31
* 最終更新日：2024/10/31
*******************************************************/
#pragma once

#include <box2d/box2d.h>
#include "Game/GameObject.h"

constexpr float B2_TO_DX_RATE = 100.0f;

/****************************************************
* 衝突リスナー
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
* フィジックスクラス
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

