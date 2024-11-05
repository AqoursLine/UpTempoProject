/******************************************************
* physics.h		物理演算
* 制作者：ミヤタジョウジ
* 作成日：2024/10/31
* 最終更新日：2024/10/31
*******************************************************/
#include "physics.h"

b2World* Physics::m_world = nullptr;


/****************************************************
* コンストラクタ
*****************************************************/
Physics::Physics(const float& gravityX, const float& gravityY) {
	//ワールド作成
	b2Vec2 gravity(gravityX, gravityY);
	m_world = new b2World(gravity);

	//当たり判定リスナー
	m_world->SetContactListener(&mcl);

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
