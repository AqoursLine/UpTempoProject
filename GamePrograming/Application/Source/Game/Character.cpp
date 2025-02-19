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

	m_stopAnim = false;

	m_uvNum = 0;
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

	m_oldState = m_currentState;


	// アニメーションのループ処理
	if (m_uvNumMax <= m_uvNum) {

		if (m_currentState == HAVETHINGS) {
			m_stopAnim = true;
		}
		else {
			m_uvNum = 0;
		}
		

		// 着地モーションからIDLEモーションに移行する
		if (m_currentState == LANDING) {
			m_currentState = IDLE;

			// 描画する画像の差し替え
			m_currentTex = ReplaceTex();

			// 現在のステートのアニメーションに合わせてUVの枚数を変える
			ChangePetternUV(m_currentState);

			m_uvNum = 0;
		}

		// 投げるモーションを最後まで再生したら割り込みフラグを下げる
		if (m_currentState == THROW) {
			m_interruptFlag = false;
		}
	}

	m_texSize.x = 1.0f / m_uvNumX;
	m_texSize.y = 1.0f / m_uvNumY;

	m_uv.x = m_texSize.x * ((int)m_uvNum % m_uvNumX);
	m_uv.y = m_texSize.y * ((int)m_uvNum / m_uvNumX);


	if (!m_stopAnim) {
		m_uvNum += m_animSpeed;
	}
	



}

void Character::Draw(XMFLOAT2 Pos, XMFLOAT2 Size, float rotate)
{
	if(m_isLeft)
		D3D.Draw2D(m_currentTex, Pos, XMFLOAT2(Size.x * 1.5f, Size.y * 1.5f), rotate, m_uv, m_texSize);
	else
		D3D.Draw2D(m_currentTex, Pos, XMFLOAT2(-Size.x * 1.5f, Size.y * 1.5f), rotate, m_uv, m_texSize);
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

ANIM_STATE Character::GetAnimState()
{
	return m_currentState;
}

void Character::SetStopAnim(bool isStop)
{
	m_stopAnim = isStop;
}

void Character::IsCharacterFacingLeft(bool isLeft)
{
	m_isLeft = isLeft;
}


