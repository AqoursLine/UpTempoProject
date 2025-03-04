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
#include "Game/ScaffoldBase.h"

ScaffoldBase::ScaffoldBase(float x, float y, float r,int pattern) : ThrowObject(x, y, r)
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
	

	//ポジション変換
	b2Vec2 b2pos = Physics::ConvertDXtoB2Float2(m_pos);
	//ボディ作成
	Physics::CreateBody(&m_body, b2pos.x, b2pos.y, r, false, this);

	//テクスチャ
	m_tex.Load(L"Data/Texture/Scaffold.png");

	//ダメージ値
	m_impactDamage = 8;

	//重量
	m_weight = WEIGHT_LIGHT;

	m_body->SetType(b2_kinematicBody);

	m_pattern = pattern;
}

ScaffoldBase::~ScaffoldBase()
{
}

void ScaffoldBase::Update()
{
	
	ScaffoldBase::PhysicsUpdate();
	if (m_pattern == 1)//これ本当ならコンストラクタで巡回するポジションもらったほうがいい
	{
		if (m_pos.x > SCREEN_WIDTH * 0.5f - 200)
			m_moveDir = -1.0f;
		else if (m_pos.x < 200)
			m_moveDir = 1.0f;
	}
	if (m_pattern == 2)//これ本当ならコンストラクタで巡回するポジションもらったほうがいい
	{
		if (m_pos.x > SCREEN_WIDTH - 400)
			m_moveDir = -1.0f;
		else if (m_pos.x < SCREEN_WIDTH * 0.6f)
			m_moveDir = 1.0f;
	}

		m_body->SetLinearVelocity(b2Vec2(m_moveDir, 0.0f));

}



