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

	//SetType("Balloon");これヤバイ　バグ多すぎ
	//問題点羅列
	// 上の壁に穴が開いた場合持ち続けると浮き続け死ぬしかも持ったまま死ぬためエラー吐く。
	// 上の壁に衝突したまま投げると上の壁に当たりまともに投げれない。
	// 持つときに上昇するためか持つタイミングで吹っ飛ばされ上の壁に当たると風船が上の壁にスタックし
	// プレイヤーもスタックする（プレイヤーは投げれば抜けれる）
	//------------------------
	// 
	// 上２つはプレイヤーが風船を持っている時に下降できるようになればそんな問題なさそう
	// 下の問題は持ち上げるときの回転動作が関係してそう
	// 
	//

}

/****************************************************
* 風船終了
*****************************************************/
Balloon::~Balloon() {

}

