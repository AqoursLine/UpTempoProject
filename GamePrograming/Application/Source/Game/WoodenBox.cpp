/******************************************************
* WoodenBox.cpp		木箱
* 制作者：ミヤタジョウジ
* 作成日：2024/11/12
* 最終更新日：2024/11/12
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/Physics.h"
#include "WoodenBox.h"


/****************************************************
* 木箱初期化
*****************************************************/
WoodenBox::WoodenBox(float x, float y, float r) : ThrowObject(x, y, r) {
	//初期設定
	m_size = XMFLOAT2(150.0f, 150.0f);

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
	m_tex.Load("Data/Texture/wooden_box.png");

}

/****************************************************
* 木箱初終了
*****************************************************/
WoodenBox::~WoodenBox() {

}

/****************************************************
* 木箱初描画
*****************************************************/
void WoodenBox::Draw() {
	D3D.Draw2D(m_tex, m_pos.x, m_pos.y, m_size.x, m_size.y, m_rot, 0.2f, 0.3f, 0.6f, 0.6f);
}
