/******************************************************
* physics.h		???????Z
* ?????F?~???^?W???E?W
* ?쐬???F2024/10/31
* ?ŏI?X?V???F2024/10/31
*******************************************************/
#include "framework.h"
#include "physics.h"

b2World* Physics::m_world = nullptr;

/****************************************************
* ?R???^?N?g???X?i?[??????????
*****************************************************/
void MyContactListener::BeginContact(b2Contact* contact) {
	//?Փ˂???2?̃I?u?W?F?N?g??擾
	uintptr_t dataA = contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	uintptr_t dataB = contact->GetFixtureB()->GetBody()->GetUserData().pointer;

	//?y?A??\?[?g
	if (dataA > dataB) std::swap(dataA, dataB);

	//?X?e?b?v???ɂ܂????????????Ȃ??ꍇ?̂ݏ???
	auto objPair = std::make_pair(dataA, dataB);
	if (processedBeginContacts.find(objPair) == processedBeginContacts.end()) {
		processedBeginContacts.insert(objPair);

		//?|?C???^????
	}

	GameObject* collisionA = reinterpret_cast<GameObject*>(dataA);
	GameObject* collisionB = reinterpret_cast<GameObject*>(dataB);

	collisionA->OnCollisionEnter(collisionB);
	collisionB->OnCollisionEnter(collisionA);
}

/****************************************************
* ?R???^?N?g???X?i?[?O?ꂽ?Ƃ?
*****************************************************/
void MyContactListener::EndContact(b2Contact* contact) {
	//?Փ˂???2?̃I?u?W?F?N?g??擾
	uintptr_t dataA = contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	uintptr_t dataB = contact->GetFixtureB()->GetBody()->GetUserData().pointer;

	//?y?A??\?[?g
	if (dataA > dataB) std::swap(dataA, dataB);

	//?X?e?b?v???ɂ܂????????????Ȃ??ꍇ?̂ݏ???
	auto objPair = std::make_pair(dataA, dataB);
	if (processedEndContacts.find(objPair) == processedEndContacts.end()) {
		processedEndContacts.insert(objPair);

	}
	//?|?C???^????
	GameObject* collisionA = reinterpret_cast<GameObject*>(dataA);
	GameObject* collisionB = reinterpret_cast<GameObject*>(dataB);

	collisionA->OnCollisionExit(collisionB);
	collisionB->OnCollisionExit(collisionA);
}

/****************************************************
* ?R???X?g???N?^
*****************************************************/
Physics::Physics(const float& gravityX, const float& gravityY) {
	//???[???h?쐬
	b2Vec2 gravity(gravityX, gravityY);
	m_world = new b2World(gravity);

	//?????蔻?胊?X?i?[
	m_world->SetContactListener(&m_mcl);

}

/****************************************************
* ?f?X?g???N?^
*****************************************************/
Physics::~Physics() {
	delete m_world;
}

/****************************************************
* ???[???h?X?V
*****************************************************/
void Physics::UpdatePhysics(const float& rate, const int32& vel, const int32& pos) {
	m_world->Step(rate, vel, pos);

	m_mcl.ClearProcessedContacts();
}

/****************************************************
* ?{?f?B?쐬
* ????
*	b2Body**	body		?i?[?p?{?f?B?|?C???^
*	float		x			???WX(box2d???W)
*	float		y			???WY(box2D???W)
*	float		r			???p(???W?A??)
*	bool		isDynamic	??????
*	GameObject*	obj			?{?f?B??????Ă????u?W?F?N?g?|?C???^
*****************************************************/
void Physics::CreateBody(b2Body** body, float x, float y, float r, bool isDynamic, GameObject* obj) {
	b2BodyDef bodydef;
	//?{?f?B?^?C?v?ݒ?(?????̂?)
	bodydef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
	bodydef.position.Set(x, y);
	bodydef.angle = r;
	//???[?U?[?f?[?^?ݒ?
	bodydef.userData.pointer = reinterpret_cast<uintptr_t>(obj);

	(*body) = m_world->CreateBody(&bodydef);
}

/****************************************************
* ?t?B?N?X?`???쐬
* ????
*	b2Body**	body		?i?[?p?{?f?B?|?C???^
*	float		w			??(box2d???W)
*	float		h			????(box2d???W)
*	float		density		???x
*	float		friction	???C
*	float		restitution	???˕Ԃ?
*****************************************************/
void Physics::CreateFixture(b2Body** body, float w, float h, float density, float friction, float restitution, bool isSensor) {
	b2PolygonShape box;
	box.SetAsBox(w * 0.5f, h * 0.5f);

	b2FixtureDef fixturedef;
	fixturedef.shape = &box;
	fixturedef.isSensor = isSensor;

	if ((*body)->GetType() == b2_dynamicBody) {
		fixturedef.density = density;
		fixturedef.friction = friction;
		fixturedef.restitution = restitution;
	}

	(*body)->CreateFixture(&fixturedef);
}

/****************************************************
* ?J?v?Z???t?B?N?X?`???쐬
* ????
*	b2Body**	body		?i?[?p?{?f?B?|?C???^
*	float		w			??(box2d???W)
*	float		h			????(box2d???W)
*	float		density		???x
*	float		friction	???C
*	float		restitution	???˕Ԃ?
*****************************************************/
void Physics::CreateCapsule(b2Body** body, float w, float h, float density, float friction, float restitution, bool isSensor) {
	//????????????Ɖ~?????ɕ???
	//???????̍???
	float boxHeight = h - w;
	//???a
	float radius = w * 0.5f;

	//???????̌`??
	b2PolygonShape box;
	box.SetAsBox(radius, boxHeight * 0.5f);
	b2FixtureDef fixturedef;
	fixturedef.shape = &box;
	fixturedef.isSensor = isSensor;
	if ((*body)->GetType() == b2_dynamicBody) {
		fixturedef.density = density;
		fixturedef.friction = friction;
		fixturedef.restitution = restitution;
	}
	(*body)->CreateFixture(&fixturedef);

	//?㕔?̉~?`?????̌`??
	b2CircleShape circle;
	//???S?̃I?t?Z?b?g
	circle.m_p.Set(0, boxHeight * 0.5f);
	circle.m_radius = radius;
	fixturedef.shape = &circle;
	(*body)->CreateFixture(&fixturedef);

	//?????̉~?`?????̌`??
	circle.m_p.Set(0, -boxHeight * 0.5f);
	fixturedef.shape = &circle;
	(*body)->CreateFixture(&fixturedef);
}

/****************************************************
* ???W?ϊ?dx??b2
*****************************************************/
b2Vec2 Physics::ConvertDXtoB2Float2(XMFLOAT2 dxf2) {
	b2Vec2 vec(dxf2.x / B2_TO_DX_RATE, dxf2.y / B2_TO_DX_RATE);
	return vec;
}

/****************************************************
* ???W?ϊ?b2??dx
*****************************************************/
XMFLOAT2 Physics::ConvertB2toDXFloat2(b2Vec2 b2v2) {
	XMFLOAT2 float2(b2v2.x * B2_TO_DX_RATE, b2v2.y * B2_TO_DX_RATE);
	return float2;
}

/****************************************************
* ?t?B?N?X?`???X?V
* ????
* b2Body**	body		?i?[?p?{?f?B?|?C???^
* float density ???x
*****************************************************/
void Physics::SetDensity(b2Body** body, float density) {

	b2Fixture* fixture = (*body)->GetFixtureList();
	fixture->SetDensity(density);

	// ???ʃf?[?^??X?V
	(*body)->ResetMassData();
} // 追加01 / 17
