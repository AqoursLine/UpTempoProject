/******************************************************
* physics.h		???????Z
* ?????F?~???^?W???E?W
* ?쐬???F2024/10/31
* ?ŏI?X?V???F2024/10/31
*******************************************************/
#pragma once

#include <box2d/box2d.h>
#include <set>
#include "DirectX/DirectX.h"
#include "Game/GameObject.h"

constexpr float B2_TO_DX_RATE = 100.0f;

/****************************************************
* ?Փ˃??X?i?[
*****************************************************/
class MyContactListener : public b2ContactListener {
public:
	void BeginContact(b2Contact* contact) override;

	void EndContact(b2Contact* contact) override;

	void ClearProcessedContacts() { processedBeginContacts.clear(); processedEndContacts.clear(); }
private:
	std::set<std::pair<uintptr_t, uintptr_t>> processedBeginContacts;
	std::set<std::pair<uintptr_t, uintptr_t>> processedEndContacts;
};

/****************************************************
* ?t?B?W?b?N?X?N???X
*****************************************************/
class Physics {
public:
	Physics() = delete;
	Physics(const float& gravityX, const float& gravityY);
	~Physics();

	void UpdatePhysics(const float& rate, const int32& vel, const int32& pos);

	static void CreateBody(b2Body** body, float x, float y, float r, bool isDynamic, GameObject* obj);
	static void CreateFixture(b2Body** body, float w, float h, float density = 1.0f, float friction = 0.3f, float restitution = 0.0f, bool isSensor = false);
	static void CreateCapsule(b2Body** body, float w, float h, float density = 1.0f, float friction = 0.3f, float restitution = 0.0f, bool isSensor = false);

	static b2Vec2 ConvertDXtoB2Float2(XMFLOAT2 dxf2);
	static XMFLOAT2 ConvertB2toDXFloat2(b2Vec2 b2v2);

	static b2World* GetWorld() {return m_world;}

	static void SetDensity(b2Body** body, float density);//?ǉ?01/17

private:
	static b2World* m_world;
	MyContactListener m_mcl;

};
