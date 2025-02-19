#include "Effect.h"

Effect::Effect(Texture& tex, XMFLOAT2 pos, XMFLOAT2 size, float rot, float time, int uvx, int uvy, int pattern):m_tex(tex)
{
	//?f?[?^?̊i?[
	
	m_posBuff = pos;
	m_rotBuff = rot;
	m_pos = &m_posBuff;//?v????????????
	m_rot = &m_rotBuff;
	m_size = size;
	m_uvX = uvx;
	m_uvY = uvy;

	m_frameCount = 0;


	if (pattern != 0)
		m_imagePattern = pattern;
	else//?p?^?[???????͂??Ȃ??ꍇ?????ŉ摜?̍ő啪??????
		m_imagePattern = m_uvX * m_uvY;

	if (time == 0)
		m_drawTime = m_imagePattern;
	else
		m_drawTime = time;


	isUse = true;

}

Effect::Effect(Texture& tex, XMFLOAT2* pos, XMFLOAT2 size, float* rot, float time, int uvx, int uvy, bool* loopflag
	,int switchframe, int pattern) :m_tex(tex)
{
	//?f?[?^?̊i?[

	m_pos = pos;
	m_rot = rot;
	m_size = size;
	m_uvX = uvx;
	m_uvY = uvy;

	m_frameCount = 0;

	m_loopFlag = loopflag;
	m_switchFrame = switchframe;

	if (pattern != 0)
		m_imagePattern = pattern;
	else//?p?^?[???????͂??Ȃ??ꍇ?????ŉ摜?̍ő啪??????
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
	if (!m_loopFlag)
	{
		if (m_frameCount >= m_drawTime)//?t???O?ς?????
			isUse = false;
	}
	else//???????͐????????ꏊ?Ō??肷??
	{
		if (!*m_loopFlag)
		{
			isUse = false;
		}
	}

}

void Effect::Draw(void)
{

	int uvNum = 0;//?摜?̉??ڂ??????邩
	XMFLOAT2 uv;
	uv.x = 0.0f;
	uv.y = 0.0f;

	XMFLOAT2 texSize;
	texSize.x = 1.0f / m_uvX;		//??
	texSize.y = 1.0f / m_uvY;		//????

	if (!m_loopFlag)
	{
		uvNum = m_frameCount / (m_drawTime / m_imagePattern);
	}
	else
	{
		uvNum = m_frameCount / m_switchFrame % m_imagePattern;
	}


	uv.x = texSize.x * (uvNum % m_uvX);
	uv.y = texSize.y * (uvNum / m_uvX);

	if (isUse)
		D3D.Draw2D(m_tex, *m_pos, m_size, *m_rot, uv, texSize);

}
