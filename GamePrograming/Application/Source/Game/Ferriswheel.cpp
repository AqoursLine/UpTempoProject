#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "Game/FerrisWheel.h"

FerrisWheel::FerrisWheel(float x, float y, float r) : ThrowObject(x, y, r) {
	//テクスチャ設定
	m_uv.x = 0.0f;
	m_uv.y = 0.0f;
	m_texSize.x = 1.0f;
	m_texSize.y = 1.0f;

	//サイズ
	float aspect = m_texSize.x / m_texSize.y;
	float height = 500.0f;
	m_size = XMFLOAT2(height * aspect, height);

	//ポジション変換
	b2Vec2 b2pos = Physics::ConvertDXtoB2Float2(m_pos);
	//ボディ作成
	Physics::CreateBody(&m_body, b2pos.x, b2pos.y, r, false, this);

	//テクスチャ
	m_tex.Load(L"Data/Texture/Wheel.png");

}

FerrisWheel::~FerrisWheel() {

}

