#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "Game/Corner.h"

Corner::Corner(const XMFLOAT2& pos, float rot, const XMFLOAT2& ceilingSize, const XMFLOAT2& wallSize, const std::wstring& fileName, const XMFLOAT2& texPos, const FIELD_DIRECTION fieldDirection) : FieldObject(pos, rot, wallSize, fileName, texPos,fieldDirection) {
	m_body->DestroyFixture(m_body->GetFixtureList());
	
	//?c?????蔻??
	XMFLOAT2 offset;
	offset.x = 0.0f;
	offset.y = (wallSize.y - ceilingSize.y) * 0.5f;
	b2Vec2 b2Offset = Physics::ConvertDXtoB2Float2(offset);
	//?`?쐬
	b2Vec2 b2Size = Physics::ConvertDXtoB2Float2(ceilingSize);
	b2PolygonShape box;
	box.SetAsBox(b2Size.x, b2Size.y, b2Offset, 0.0f);	
	//?t?B?N?X?`???쐬
	b2FixtureDef fixturedef;
	fixturedef.shape = &box;
	m_body->CreateFixture(&fixturedef);

	//???????蔻??
	offset.y = 0.0f;
	offset.x = (ceilingSize.x - wallSize.x) * 0.5f;
	offset.x *= (pos.x > SCREEN_WIDTH * 0.5f ? -1 : 1);
	b2Offset = Physics::ConvertDXtoB2Float2(offset);
	//?`?쐬
	b2Size = Physics::ConvertDXtoB2Float2(wallSize);
	box.SetAsBox(b2Size.x, b2Size.y, b2Offset, 0.0f);
	//?t?B?N?X?`???쐬
	fixturedef.shape = &box;
	m_body->CreateFixture(&fixturedef);

}

Corner::~Corner() {
}
