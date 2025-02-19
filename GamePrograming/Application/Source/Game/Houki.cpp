/******************************************************
* Houki.cpp	箒
* 制作者：イササトル
* 作成日：2024/12/05
* 最終更新日：2024/12/05
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"
#include "Houki.h"

/****************************************************
* 箒初期化
*****************************************************/
Houki::Houki(float x, float y, float r) : ThrowObject(x, y, r) {
	//テクスチャ設定
	m_uv.x = 0.3f;
	m_uv.y = 0.05f;
	m_texSize.x = 0.5f;
	m_texSize.y = 0.9f;

	//サイズ
	float aspect = m_texSize.x / m_texSize.y;
	float height = 120.0f;
	m_size = XMFLOAT2(height * aspect, height);

	//ポジション変換
	b2Vec2 b2pos = Physics::ConvertDXtoB2Float2(m_pos);
	//ボディ作成
	Physics::CreateBody(&m_body, b2pos.x, b2pos.y, r, true, this);

	//サイズ変換
	b2Vec2 b2size = Physics::ConvertDXtoB2Float2(m_size);
	//当たり判定作成
	Physics::CreateFixture(&m_body, b2size.x, b2size.y, 1.0f);

	//テクスチャ
	m_tex.Load(L"Data/Texture/Houki.png");

	//重量
	m_weight = WEIGHT_NORMAL;


}

/****************************************************
* 箒終了
*****************************************************/
Houki::~Houki() {

}

/****************************************************
* Update
*****************************************************/
void Houki::Update(){
	ThrowObject::Update();

	
	if (GetIsThrow() && m_first)
	{
		Physics::SetDensity(&m_body, 0.0f);
		m_first = false;
	}
	if (GetIsThrow()&&downFirst)
	{
		int a = static_cast<int>(m_pos.x - m_throwPos.x);
		int b = static_cast<int>(m_pos.y - m_throwPos.y);
		int c = a * a + b * b;

		int downDistance = 700;
		if ( c >= downDistance* downDistance)
		{
			Physics::SetDensity(&m_body, 1.0f);
			m_body->SetLinearVelocity(b2Vec2(0,5));
			downFirst = false;
		}
		
	}

}


