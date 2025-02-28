/******************************************************
* Horse.cpp	うま
* 制作者：murayama
* 作成日：2024/01/23
* 最終更新日：
* ポールは描画しない。もともと背景にあるものを使いたい
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"
#include "Game/Horse.h"


/****************************************************
* 初期化
*****************************************************/
Horse::Horse(float x, float y, float r,bool front) : ThrowObject(x, y, r) {
	m_cnt = 0;
	m_front = front;

	if (m_front)
	{
		//テクスチャ設定
		m_uv.x = 0.0175f;
		m_uv.y = 0.42f;
		m_texSize.x = 0.098f;
		m_texSize.y = 0.255f;
	}
	else
	{//テクスチャ設定
		//m_uv.x = 0.24f;
		//m_uv.y = 0.553f;
		//m_texSize.x = 0.115f;
		//m_texSize.y = 0.31f;

		m_uv.x = 0.495f;
		m_uv.y = 0.435f;
		m_texSize.x = 0.095f;
		m_texSize.y = 0.255f;
	}

	//サイズ設定
	float aspect = m_texSize.x / m_texSize.y;
	float height = 298.0f;//test用で試した際のポールを考慮したちょうどいいサイズ
	//サイズの比率
	m_size = XMFLOAT2(height * aspect, height);

	//ポジション変換
	b2Vec2 b2pos = Physics::ConvertDXtoB2Float2(m_pos);
	//ボディ作成
	Physics::CreateBody(&m_body, b2pos.x, b2pos.y, r, true, this);

	//サイズ変換
	b2Vec2 b2size = Physics::ConvertDXtoB2Float2(m_size);
	//当たり判定作成
	Physics::CreateFixture(&m_body, b2size.x, b2size.y, 0.0f);

	//テクスチャ
	if(m_front)
	m_tex.Load(L"Data/Texture/HorseFront.png");
	else
	{//後ろ向き
		m_tex.Load(L"Data/Texture/HorseBack.png"); // 前面の馬だけポールがないのが気持ち悪いからポール無しに統一
		m_tex2.Load(L"Data/Texture/HorseBack.png");
	}

	//ダメージ値
	m_impactDamage = 8;

	//重量
	m_weight = WEIGHT_HEAVY;

}

/****************************************************
* しゅうりょう
*****************************************************/
Horse::~Horse() {

}

/****************************************************
* Update
*****************************************************/
void Horse::Update() {
	ThrowObject::Update();

	
	if (m_fixed)
	{
		if (m_cnt >= 120)
		{//最初に配置した位置が動きの最低の位置
			m_turn *= -1;
			m_cnt = 0;
		}
		m_body->SetLinearVelocity(b2Vec2(0.f, 0.5f * m_turn));//与えている数値や切り替えるフレームは適当

		m_cnt++;
	}

	
}

void Horse::HoldTiming()
{
	
		b2Fixture* fixture = (m_body)->GetFixtureList();
		fixture->SetDensity(1.0f);
		fixture->SetFriction(0.3f);
		fixture->SetRestitution(0.0f);
		m_body->ResetMassData();
		m_fixed = false;

		if (!m_front && first)
		{//背中側の場合ポールなしの画像に切り替え
			m_uv.x = 0.495f;
			m_uv.y = 0.435f;
			m_texSize.x = 0.095f;
			m_texSize.y = 0.255f;
			m_tex = m_tex2;
			first = false;
		}
	
}




