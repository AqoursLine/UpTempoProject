/******************************************************
* Lamp.cpp	蛍光灯
* 制作者：murayama
* 作成日：2024/01/23
* 最終更新日：
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"
#include "Game/Lamp.h"


/****************************************************
* 初期化
*****************************************************/
Lamp::Lamp(float x, float y, float r,bool left) : ThrowObject(x, y, r) {

	m_hp = 3;//何回衝突したら落下するか
	m_sizeAdjust = 1.05f;

	//テクスチャ設定//同じにしてるからif文無駄だけど調整するかも
	if (left)
	{//左側のテクスチャ
		m_uv.x = 0.02f;
		m_uv.y = 0.3f;
		m_texSize.x = 0.96f;
		m_texSize.y = 0.24f;
	}
	else
	{
		m_uv.x = 0.02f;
		m_uv.y = 0.3f;
		m_texSize.x = 0.96f;
		m_texSize.y = 0.24f;
	}

	//サイズ設定	ポジション変換と当たり判定作成をコメントアウトしてるコードにすると上の線の部分を切った判定
	float aspect = m_texSize.x / m_texSize.y;
	float height = 120.0f;
	m_size = XMFLOAT2(height * aspect, height);

	//ポジション変換
	//b2Vec2 b2pos = Physics::ConvertDXtoB2Float2(XMFLOAT2( m_pos.x,m_pos.y + 25));//25は120の場合
	b2Vec2 b2pos = Physics::ConvertDXtoB2Float2(XMFLOAT2( m_pos.x,m_pos.y));
	//ボディ作成
	Physics::CreateBody(&m_body, b2pos.x, b2pos.y, r, false, this);

	//サイズ変換
	b2Vec2 b2size = Physics::ConvertDXtoB2Float2(m_size);
	//当たり判定作成
	Physics::CreateFixture(&m_body, b2size.x, b2size.y, 1.0f);//*0.5で線の部分の判定を消したい
	//Physics::CreateFixture(&m_body, b2size.x, b2size.y * 0.45, 1.0f);//*0.5で線の部分の判定を消したい


	
	//テクスチャ
	if(left)
		m_tex.Load(L"Data/Texture/LampLeft.png");
	else
		m_tex.Load(L"Data/Texture/LampRight.png");

	//重量
	m_weight = WEIGHT_HEAVY;

	SetTag("Lamp");

}

/****************************************************
* しゅうりょう
*****************************************************/
Lamp::~Lamp() {

}

/****************************************************
* Update
*****************************************************/
void Lamp::Update() {

	
	
	if (m_hp <= 0 && first)
	{
		m_fixed = false;
		m_body->SetType(b2_dynamicBody);
		b2Fixture* fixture = (m_body)->GetFixtureList();
		fixture->SetDensity(0.3f);//重すぎて飛ばないから0.
		fixture->SetFriction(0.3f);
		fixture->SetRestitution(0.0f);
		m_body->ResetMassData();
		SetTag("ThrowObject");
		first = false;
	
	}
	if (m_fixed)
	{
	}
	else
	{
		ThrowObject::Update();
	}

}

void Lamp::OnCollisionEnter(GameObject* collision)
{

	if (m_fixed)
	{
		if (collision->CompareTag("ThrowObject") && ((ThrowObject*)collision)->GetIsThrow()) 
		{
			m_hp -= 1;
			((ThrowObject*)collision)->SetIsThrow(false);
			((ThrowObject*)collision)->SetIsDeleteStandBy(true);
		}
	}
	else
	{
		ThrowObject::OnCollisionEnter(collision);
	}
	
}



