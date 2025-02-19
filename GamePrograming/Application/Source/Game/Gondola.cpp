/******************************************************
* Gondola.cpp	観覧車
* 制作者：イササトル
* 作成日：2025/01/17
* 最終更新日：2025/01/17
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"
#include "Game/Gondola.h"

/****************************************************
* 観覧車初期化
*****************************************************/
Gondola::Gondola(float x, float y, float r) : ThrowObject(x, y, r) {
	//テクスチャ設定
	m_uv.x = 0.0f;
	m_uv.y = 0.0f;
	m_texSize.x = 1.0f;
	m_texSize.y = 1.0f;

	//サイズ
	float aspect = m_texSize.x / m_texSize.y;
	float height = 80.0f;
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
	m_tex.Load(L"Data/Texture/kanran1.png");

	// 与える力の設定
	m_ApplyImpact = { 2.0f,1.0f };
}

/****************************************************
* 観覧車終了
*****************************************************/
Gondola::~Gondola() {

}

