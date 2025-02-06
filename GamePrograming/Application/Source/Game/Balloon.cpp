/******************************************************
* Balloon.cpp	風船
* 制作者：カワマタトウ
* 作成日：2024/12/28
* 最終更新日：2024/12/28
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"
#include "Game/Balloon.h"
/****************************************************
* 風船初期化
*****************************************************/
Balloon::Balloon(float x, float y, float r) : ThrowObject(x, y, r) {
	//テクスチャ設定
	m_uv.x = 0.2f;
	m_uv.y = 0.04f;
	m_texSize.x = 0.65f;
	m_texSize.y = 0.95f;

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
	m_tex.Load(L"Data/Texture/balloon.png");

	//重量
	m_weight = WEIGHT_LIGHT;

	SetType("Balloon");
}

/****************************************************
* 風船終了
*****************************************************/
Balloon::~Balloon() {

}