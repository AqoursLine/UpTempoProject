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

	if (time == 0)
		m_drawTime = m_imagePattern;
	else
		m_drawTime = time;


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
	XMFLOAT2 uv;
	uv.x = 0.0f;
	uv.y = 0.0f;

	XMFLOAT2 texSize;
	texSize.x = 1.0f / m_uvX;		//幅
	texSize.y = 1.0f / m_uvY;		//高さ

	uvNum = m_frameCount / (m_drawTime / m_imagePattern);


	uv.x = texSize.x * (uvNum % m_uvX);
	uv.y = texSize.y * (uvNum / m_uvX);

	if (isUse)
		D3D.Draw2D(m_tex, m_pos, m_size, m_rot, uv, texSize);

}
