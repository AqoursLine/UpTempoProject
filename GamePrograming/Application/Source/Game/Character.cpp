/******************************************************
* Character.cpp	キャラクター
* 制作者：ユミタリオ
* 作成日：	2025/01/21
* 最終更新日：2025/01/21
*******************************************************/
#include "framework.h"
#include "Game/Character.h"
#include "Game/SaveData.h"

Character::Character()
{
	// どのキャラクターを使っているかを確認し、そのキャラの全てのテクスチャをメンバに保存
	m_allTex.idleTex.Load(L"Data/Texture/SampleIdle.png");

	m_currentState = m_oldState = IDLE;
	m_interruptFlag = false;

	// 待機状態のアニメーションの数をセット
	m_uvNumX = 5;
	m_uvNumY = 6;
	m_uvNumMax = 26;

	m_currentTex = m_allTex.idleTex;
}

Character::~Character()
{
	
}

void Character::Update()
{
	// 前回と今回のアニメーションステートが違ったら
	if (m_currentState != m_oldState) {

		// 描画する画像の差し替え
		m_currentTex = ReplaceTex();

		m_uvNum = 0; // リセット
	}

	// アニメーションのループ処理
	if (m_uvNumMax < m_uvNum) {
		m_uvNum = 0;
	}

	m_texSize.x = 1.0f / m_uvNumX;
	m_texSize.y = 1.0f / m_uvNumY;

	m_uv.x = m_texSize.x * (m_uvNum % m_uvNumX);
	m_uv.y = m_texSize.y * (m_uvNum / m_uvNumX);

	m_uvNum++;

}

void Character::Draw(XMFLOAT2 Pos, XMFLOAT2 Size, float rotate)
{
	D3D.Draw2D(m_currentTex, Pos, XMFLOAT2(Size.x * 1.5, Size.y * 1.5), rotate, m_uv, m_texSize);
}

void Character::SetInterruptFlag(bool flag)
{
	m_interruptFlag = flag;
}

bool Character::GetInterruptFlag()
{
	return m_interruptFlag;
}

void Character::SetAnimState(ANIM_STATE animState)
{
	m_currentState = animState;
}

Texture Character::ReplaceTex()
{
	switch (m_currentState)
	{
	case IDLE:
		m_uvNumX = 5;
		m_uvNumY = 6;
		m_uvNumMax = 27;
		return m_allTex.idleTex;

	case MOVE:

		return m_allTex.moveTex;

	case JUMP:

		return m_allTex.jumpTex;

	case FALL:

		return m_allTex.fallTex;

	case LANDING:

		return m_allTex.landingTex;

	case HITSTOP:

		return m_allTex.hitstopTex;

	case BLOW:

		return m_allTex.blowTex;

	case HAVETHINGS:

		return m_allTex.havethingsTex;

	case THROW:

		return m_allTex.throwTex;

	default:
		break;
	}
}
