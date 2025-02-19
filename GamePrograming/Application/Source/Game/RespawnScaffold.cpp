/******************************************************
* R_Scaffold.cpp	うま
* 制作者：murayama
* 作成日：2024/01/23
* 最終更新日：
* ポールは描画しない。もともと背景にあるものを使いたい
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"
#include "Game/RespawnScaffold.h"


/****************************************************
* 初期化
*****************************************************/
R_Scaffold::R_Scaffold(float x, float y, float r,int pnum) : ThrowObject(x, y, r) {
	
	m_resPNum = pnum;

	m_isCollision = false;
	m_cnt = 0;
	//テクスチャ設定//テスト用設定
	m_uv.x = 0.0;
	m_uv.y = 0.4f;
	m_texSize.x = 1.0f;
	m_texSize.y = 0.4f;
	
	//サイズ設定
	float aspect = m_texSize.x / m_texSize.y;
	float height = 100.0f;
	//サイズの比率
	m_size = XMFLOAT2(height * aspect, height);

	//ポジション変換
	b2Vec2 b2pos = Physics::ConvertDXtoB2Float2(m_pos);
	//ボディ作成
	Physics::CreateBody(&m_body, b2pos.x, b2pos.y, r, false, this);

	//サイズ変換
	b2Vec2 b2size = Physics::ConvertDXtoB2Float2(m_size);
	//当たり判定作成
	Physics::CreateFixture(&m_body, b2size.x, b2size.y, 0.0f);

	//テクスチャ
	m_tex.Load(L"Data/Texture/asiba.png");//テスト用テクスチャ

	//重量
	m_weight = WEIGHT_HEAVY;

}

/****************************************************
* しゅうりょう
*****************************************************/
R_Scaffold::~R_Scaffold() {

}

void R_Scaffold::Update()
{
	ThrowObject::Update();
	if (m_fixed)
	{
		if (!m_isCollision)
			m_cnt++;

		if (m_cnt > 5)//５は適当
		{
			b2Fixture* fixture = (m_body)->GetFixtureList();
			fixture->SetDensity(1.0f);
			fixture->SetFriction(0.3f);
			fixture->SetRestitution(0.0f);
			m_body->ResetMassData();
			m_fixed = false;
		}
	}
}



void R_Scaffold::HoldTiming()
{

	b2Fixture* fixture = (m_body)->GetFixtureList();
	fixture->SetDensity(1.0f);
	fixture->SetFriction(0.3f);
	fixture->SetRestitution(0.0f);
	m_body->ResetMassData();
	m_fixed = false;
}


void R_Scaffold::OnCollisionEnter(GameObject* collision)
{
	if (!GetIsThrow())
	{
		if (collision->CompareTag("Player")&& ((Player*)collision)->GetPlayerNum() == m_resPNum)
		{
			m_isCollision = true;
			
		}
	}
	ThrowObject::OnCollisionEnter(collision);
	
}

void R_Scaffold::OnCollisionExit(GameObject* collision)
{
	
	if (collision->CompareTag("Player") && ((Player*)collision)->GetPlayerNum() == m_resPNum)
	{
		m_isCollision = false;
		m_cnt = 0;
	}
	
}


