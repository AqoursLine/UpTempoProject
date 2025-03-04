/******************************************************
* .cpp 看板
* 制作者：センバソラ
* 作成日：2024/12/05
* 最終更新日：2025/2/13
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"
#include "Game/Scaffold.h"
#include "Game/ScaffoldBase.h"

Scaffold::Scaffold(float x, float y, float r, int no, ThrowObject* base) : ThrowObject(x, y, r)
{
	//テクスチャ設定
	m_uv.x = 0.2f;
	m_uv.y = 0.27f;
	m_texSize.x = 0.53f;
	m_texSize.y = 0.55f;

	//サイズ設定
	float aspect = m_texSize.x / m_texSize.y;
	float height = 70.0f;
	m_size = XMFLOAT2(height * aspect, height);

	m_pos = base->GetPos();
	m_pos.x += no * m_size.x;

	//ポジション変換
	b2Vec2 b2pos = Physics::ConvertDXtoB2Float2(m_pos);
	//ボディ作成
	Physics::CreateBody(&m_body, b2pos.x, b2pos.y, r, true, this);

	//サイズ変換
	b2Vec2 b2size = Physics::ConvertDXtoB2Float2(m_size);
	//当たり判定作成
	Physics::CreateFixture(&m_body, b2size.x, b2size.y, 1.0f);

	ScaffoldBase* pBase = static_cast<ScaffoldBase*>(base);

	//テクスチャ
	m_tex.Load(L"Data/Texture/Scaffold.png");

	//ダメージ値
	m_impactDamage = 8;

	//重量
	m_weight = WEIGHT_LIGHT;

	b2WeldJointDef weldDef;
	weldDef.Initialize(m_body,pBase->GetBody() , pBase->GetBody()->GetWorldCenter());
	weldJoint = Physics::GetWorld()->CreateJoint(&weldDef);

}

Scaffold::~Scaffold()
{
}

void Scaffold::HoldTiming()
{
	Physics::GetWorld()->DestroyJoint(weldJoint);
	weldJoint = nullptr;

	b2Fixture* fixture = (m_body)->GetFixtureList();
	fixture->SetDensity(1.0f);
	fixture->SetFriction(0.3f);
	fixture->SetRestitution(0.0f);
	m_body->ResetMassData();

}



