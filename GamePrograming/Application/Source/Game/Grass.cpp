/******************************************************
* Grass.cpp	草
* 制作者：センバソラ
* 作成日：2024/12/05
* 最終更新日：2025/2/13
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"
#include "Grass.h"

Grass::Grass(float x, float y, float r) : StageObject(x, y, r)
{
	//テクスチャ設定
	m_uv.x = 0.2f;
	m_uv.y = 0.34f;
	m_texSize.x = 0.7f;
	m_texSize.y = 0.37f;

	//サイズ設定
	float aspect = m_texSize.x / m_texSize.y;
	float height = 150.0f;
	m_size = XMFLOAT2(height * aspect, height);

	//ポジション変換
	b2Vec2 b2pos = Physics::ConvertDXtoB2Float2(m_pos);
	//ボディ作成
	Physics::CreateBody(&m_body, b2pos.x, b2pos.y, r, false, this);

	//サイズ変換
	b2Vec2 b2size = Physics::ConvertDXtoB2Float2(m_size);
	//当たり判定作成
	Physics::CreateFixture(&m_body, b2size.x, b2size.y, 1.0f);

	//テクスチャ
	m_tex.Load(L"Data/Texture/Grass.png");

	//ダメージ値
	m_impactDamage = 10;

	//重量
	m_weight = WEIGHT_LIGHT;

	m_hp = 3;
	SetTag("Grass");
}

Grass::~Grass()
{
}


