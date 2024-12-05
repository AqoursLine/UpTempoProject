/******************************************************
* Shield.cpp	シールド
* 制作者：ユミタリオ
* 作成日：2024/12/05
* 最終更新日：2024/12/05
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"

#include "Shield.h"

Shield::Shield(float x, float y, float r) :ThrowObject(x, y, r)
{
	//テクスチャ設定
	m_uv.x = 0.19f;
	m_uv.y = 0.21f;
	m_texSize.x = 0.5f;
	m_texSize.y = 0.57f;

	//サイズ設定
	float aspect = m_texSize.x / m_texSize.y;
	float height = 100.0f;
	m_size = XMFLOAT2(height * aspect, height);

	//ポジション変換
	b2Vec2 b2pos = Physics::ConvertDXtoB2Float2(m_pos);
	//ボディ作成
	Physics::CreateBody(&m_body, b2pos.x, b2pos.y, r, true, this);

	//サイズ変換
	b2Vec2 b2size = Physics::ConvertDXtoB2Float2(m_size);
	//当たり判定作成
	Physics::CreateFixture(&m_body, b2size.x, b2size.y, 1.0f);

	//タグセット
	SetTag("ThrowObject");

	//テクスチャ
	m_tex.Load("Data/Texture/shield.png");
}

Shield::~Shield()
{
}
