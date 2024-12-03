#include "Effect.h"

Effect::Effect(Texture& tex, XMFLOAT2 pos, XMFLOAT2 size, float rot, float time, int uvx, int uvy, int pattern):m_tex(tex)
{
	//データの格納
	
	m_pos = pos;
	m_rot = rot;
	m_size = size;
	m_uvX = uvx;
	m_uvY = uvy;

	m_frameCount = 0;


	if (pattern != 0)
		m_imagePattern = pattern;
	else//パターン数を入力しない場合自動で画像の最大分割数に
		m_imagePattern = m_uvX * m_uvY;

	if(time==0)
		m_drawTime = m_imagePattern;


	isUse = true;

}

Effect::~Effect()
{
}

void Effect::Update(void)
{
	m_frameCount++;
	if (m_frameCount >= m_drawTime)//フラグ変えるよん
		isUse = false;
}

void Effect::Draw(void)
{

	int uvNum = 0;//画像の何個目を表示するか
	float uvLeft = 0.0f;
	float uvTop = 0.0f;

	float uvW = 1.0f / m_uvX;		//幅
	float uvH = 1.0f / m_uvY;		//高さ

	uvNum = m_frameCount / (m_drawTime / m_imagePattern);


	uvLeft = uvW * (uvNum % m_uvX);
	uvTop = uvH * (uvNum / m_uvX);


	if (isUse)
		D3D.Draw2D(m_tex, m_pos.x, m_pos.y, m_size.x, m_size.y, m_rot, uvLeft, uvTop, uvW, uvH);

}
