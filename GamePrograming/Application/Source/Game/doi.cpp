#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "DirectX/Audio.h"

#include "Game/doi.h"



MrDoi::MrDoi(float x, float y, float r) : ThrowObject(x, y, r)
{
	//テクスチャ設定
	m_uv.x = 0.3f;
	m_uv.y = 0.06f;
	m_texSize.x = 0.35f;
	m_texSize.y = 0.89f;

	//サイズ設定
	float aspect = m_texSize.x / m_texSize.y;
	float height = 200;
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
	m_tex.Load(L"Data/Texture/Doi.png");

	//ダメージ値
	m_impactDamage = 8;

	//重量
	m_weight = WEIGHT_NORMAL;

//	m_soundNum = AUDIO.LoadWaveFile("Data/Sound/SE/ThingsToWall.wav");
	m_breakObjectSound = AUDIO.LoadWaveFile("Data/Sound/SE/土居_土居ヂュゥウゥ.wav");
	m_throwSound2 = AUDIO.LoadWaveFile("Data/Sound/SE/土居_やめろヨォ.wav");
//	m_collisionSound2 = AUDIO.LoadWaveFile("Data/Sound/SE/手足・殴る、蹴る09.wav");

	AUDIO.SetVolume(m_throwSound2, 3.0f);
	AUDIO.SetVolume(m_breakObjectSound, 10.0f);

}

MrDoi::~MrDoi()
{
}
