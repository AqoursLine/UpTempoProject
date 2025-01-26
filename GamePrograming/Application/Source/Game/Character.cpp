/******************************************************
* Character.cpp	キャラクター
* 制作者：ユミタリオ
* 作成日：	2025/01/21
* 最終更新日：2025/01/23
*******************************************************/
#include "framework.h"
#include "Game/Character.h"
#include "Game/SaveData.h"

Character::Character()
{
	// 各キャラごとに全てのモーションの画像を格納
	//m_allTex.idleTex.Load(L"Data/Texture/SampleIdle.png");

	// これはCharacterの方でやっとく
	m_currentState = m_oldState = IDLE;
	m_interruptFlag = false;

	// 待機状態のアニメーションの数をセット（各キャラのCPPでやる）
	//m_uvNumX = 5;
	//m_uvNumY = 6;
	//m_uvNumMax = 27;


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

		// 現在のステートのアニメーションに合わせてUVの枚数を変える
		ChangePetternUV(m_currentState);

		m_uvNum = 0; // リセット
	}

	// アニメーションのループ処理
	if (m_uvNumMax <= m_uvNum) {
		m_uvNum = 0;
	}

	m_texSize.x = 1.0f / m_uvNumX;
	m_texSize.y = 1.0f / m_uvNumY;

	m_uv.x = m_texSize.x * ((int)m_uvNum % m_uvNumX);
	m_uv.y = m_texSize.y * ((int)m_uvNum / m_uvNumX);

	m_uvNum += m_animSpeed;

	m_oldState = m_currentState;

}

void Character::Draw(XMFLOAT2 Pos, XMFLOAT2 Size, float rotate)
{
	if(m_isLeft)
		D3D.Draw2D(m_currentTex, Pos, XMFLOAT2(Size.x * 1.5, Size.y * 1.5), rotate, m_uv, m_texSize);
	else
		D3D.Draw2D(m_currentTex, Pos, XMFLOAT2(-Size.x * 1.5, Size.y * 1.5), rotate, m_uv, m_texSize);
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

void Character::IsCharacterFacingLeft(bool isLeft)
{
	m_isLeft = isLeft;
}
