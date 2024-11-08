/******************************************************
* Fish.cpp		フィッシュ
* 制作者：センバソラ
* 作成日：2024/11/07
* 最終更新日：2024/11/07
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "Game/Controller.h"

#include "Game/Fish.h"

/****************************************************
* フィッシュ初期化
*****************************************************/

Fish::Fish()
{
	float x, y;
	x = rand() % SCREEN_WIDTH;
	y = rand() % SCREEN_HEIGHT;

	//初期設定
	m_pos = XMFLOAT2(x, y);
	m_rot = 0.0f;
	m_size = XMFLOAT2(100.0f, 100.0f);

	//座標変換
	b2Vec2 pos = Physics::ConvertDXtoB2Float2(m_pos);
	//ボディ作成
	Physics::CreateBody(&m_body, pos.x, pos.y, m_rot, true, this);

	//座標変換
	b2Vec2 size = Physics::ConvertDXtoB2Float2(m_size);

	//当たり判定作成
	Physics::CreateFixture(&m_body, size.x, size.y, true);

	//回転無効
	m_body->SetFixedRotation(true);

	//テクスチャロード
	m_tex.Load("Data/Texture/Fish.png");

	IsUse = true;
}

/****************************************************
* フィッシュ終了
*****************************************************/
Fish::~Fish()
{

}

/****************************************************
* フィッシュ更新
*****************************************************/
void Fish::Update()
{
	if (IsUse) {
		//座標更新
		m_pos = Physics::ConvertB2toDXFloat2(m_body->GetPosition());
		m_rot = m_body->GetAngle();
	}
	
}


/****************************************************
* フィッシュ描画
*****************************************************/
void Fish::Draw()
{
	if (IsUse) {
		//dx座標で描画
		D3D.Draw2D(m_tex, m_pos.x, m_pos.y, m_size.x, m_size.y, m_rot, 0.0f, 0.0f, 1.0f, 1.0f);
	}
	
}

void Fish::OnCollisionEnter(GameObject* collision)
{

	if (collision->CompareTag("Net")) {
		IsUse = false;
	}
}
