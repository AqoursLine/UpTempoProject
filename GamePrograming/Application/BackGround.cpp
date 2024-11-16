#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "Game/Controller.h"

#include "Game/BackGround.h"

BackGround::BackGround()
{
	//初期設定
	m_pos = XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	m_rot = 0.0f;
	m_size = XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT);

	//座標変換
	b2Vec2 pos = Physics::ConvertDXtoB2Float2(m_pos);

	//座標変換
	b2Vec2 size = Physics::ConvertDXtoB2Float2(m_size);

	//テクスチャロード
	m_tex.Load("Data/Texture/Fish_BackGround.png");
}


BackGround::~BackGround()
{

}


void BackGround::Update()
{

}


void BackGround::Draw()
{
	//dx座標で描画
	D3D.Draw2D(m_tex, m_pos.x, m_pos.y, m_size.x, m_size.y, m_rot, 0.0f, 0.0f, 1.0f, 1.0f);
}
