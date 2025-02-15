#include "framework.h"
#include "Game/Transition.h"

Transition::Transition(Texture& tex, XMFLOAT2 pos, XMFLOAT2 size, float rot, int uvNumX, int uvNumY, int animFrameMax, float animSpeed)
	:m_tex(tex), m_pos(pos), m_size(size), m_rot(rot),
	m_uvNumX(uvNumX), m_uvNumY(uvNumY), m_animFrameMax(animFrameMax),
	m_animSpeed(animSpeed)
{
	m_animFinished = false;
	m_uvNum = 0.0f;
}

void Transition::Update()
{
	// アニメーションが最後まで再生されていたら何もしない
	if (m_animFinished) return;

	// テクスチャ一個分の長さ測るぜ
	m_texSize.x = 1.0f / m_uvNumX;
	m_texSize.y = 1.0f / m_uvNumY;

	// その長さ分、縦と横にずらすぜ。
	m_uv.x = m_texSize.x * (static_cast<int>(m_uvNum) % m_uvNumX);
	m_uv.y = m_texSize.y * (static_cast<int>(m_uvNum) / m_uvNumX);


	if (m_uvNum < m_animFrameMax) {

		m_uvNum += m_animSpeed;
	}
	else {
		m_animFinished = true;
	}
	
}

void Transition::Draw() const
{
	D3D.Draw2D(m_tex, m_pos, m_size, m_rot, m_uv, m_texSize);
}
