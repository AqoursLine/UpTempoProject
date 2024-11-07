/******************************************************
* physics.h		物理演算
* 制作者：ミヤタジョウジ
* 作成日：2024/10/31
* 最終更新日：2024/10/31
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/physics.h"

b2World* Physics::m_world = nullptr;


/****************************************************
* コンストラクタ
*****************************************************/
Physics::Physics(const float& gravityX, const float& gravityY) {
	//ワールド作成
	b2Vec2 gravity(gravityX, gravityY);
	m_world = new b2World(gravity);

	//当たり判定リスナー
	m_world->SetContactListener(&m_mcl);

}

/****************************************************
* デストラクタ
*****************************************************/
Physics::~Physics() {
	delete m_world;
}

/****************************************************
* ワールド更新
*****************************************************/
void Physics::UpdatePhysics(const float& rate, const int32& vel, const int32& pos) {
	m_world->Step(rate, vel, pos);
}

/****************************************************
* ボディ作成
* 引数
*	b2Body**	body		格納用ボディポインタ
*	float		x			座標X(box2d座標)
*	float		y			座標Y(box2D座標)
*	float		r			回転角(ラジアン)
*	bool		isDynamic	動くか
*	GameObject*	obj			ボディを持っているオブジェクトポインタ
*****************************************************/
void Physics::CreateBody(b2Body** body, float x, float y, float r, bool isDynamic, GameObject* obj) {
	b2BodyDef bodydef;
	//ボディタイプ設定(動くのか)
	bodydef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
	bodydef.position.Set(x, y);
	bodydef.angle = r;
	//ユーザーデータ設定
	bodydef.userData.pointer = reinterpret_cast<uintptr_t>(obj);

	(*body) = m_world->CreateBody(&bodydef);
}

/****************************************************
* フィクスチャ作成
* 引数
*	b2Body**	body		格納用ボディポインタ
*	float		w			幅(box2d座標)
*	float		h			高さ(box2d座標)
*	float		density		密度
*	float		friction	摩擦
*	float		restitution	跳ね返り
*****************************************************/
void Physics::CreateFixture(b2Body** body, float w, float h, bool IsTrigger, float density, float friction, float restitution) {
	b2PolygonShape box;
	box.SetAsBox(w * 0.5f, h * 0.5f);

	b2FixtureDef fixturedef;
	fixturedef.shape = &box;
	fixturedef.userData.pointer = (*body)->GetUserData().pointer;
	fixturedef.isSensor = IsTrigger;

	if ((*body)->GetType() == b2_dynamicBody) {
		fixturedef.density = density;
		fixturedef.friction = friction;
		fixturedef.restitution = restitution;
	}

	(*body)->CreateFixture(&fixturedef);
}

/****************************************************
* 座標変換dx→b2
*****************************************************/
b2Vec2 Physics::ConvertDXtoB2Float2(XMFLOAT2 dxf2) {
	b2Vec2 vec(dxf2.x / B2_TO_DX_RATE, dxf2.y / B2_TO_DX_RATE);
	return vec;
}

/****************************************************
* 座標変換b2→dx
*****************************************************/
XMFLOAT2 Physics::ConvertB2toDXFloat2(b2Vec2 b2v2) {
	XMFLOAT2 float2(b2v2.x * B2_TO_DX_RATE, b2v2.y * B2_TO_DX_RATE);
	return float2;
}
