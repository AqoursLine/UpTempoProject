/******************************************************
* Ghost.cpp		ゴーストcpp
* 制作者：イササトル
* 作成日：2025/2/15
* 最終更新日：2025/2/15
*******************************************************/

#include "framework.h"
#include "Ghost.h"
#include <mutex>

// static メンバ変数の初期化
//ANIM_TEX Esper::m_allTex = []() {
//	ANIM_TEX tex;
//	tex.idleTex.Load(L"Data/Texture/Motion/Esper/Idle_1.png");
//	tex.moveTex.Load(L"Data/Texture/Motion/Esper/Move_1.png");
//	tex.jumpTex.Load(L"Data/Texture/Motion/Esper/Jump_1.png");
//	tex.fallTex.Load(L"Data/Texture/Motion/Esper/Fall_1.png");
//	tex.landingTex.Load(L"Data/Texture/Motion/Esper/Landing.png");
//	tex.hitstopTex.Load(L"Data/Texture/Motion/Esper/Hitstop.png");
//	tex.blowTex.Load(L"Data/Texture/Motion/Esper/Blow.png");
//	tex.havethingsTex.Load(L"Data/Texture/Motion/Esper/HaveThings.png");
//	tex.throwTex.Load(L"Data/Texture/Motion/Esper/Throw.png");
//	return tex;
//	}();
ANIM_TEX Ghost::m_allTex;

Ghost::Ghost()
{

	//m_allTex.idleTex.Load(L"Data/Texture/Motion/Esper/Idle.png");
	//m_allTex.moveTex.Load(L"Data/Texture/Motion/Esper/Move.png");
	//m_allTex.jumpTex.Load(L"Data/Texture/Motion/Esper/Jump.png");
	//m_allTex.fallTex.Load(L"Data/Texture/Motion/Esper/Fall.png");
	//m_allTex.landingTex.Load(L"Data/Texture/Motion/Esper/Landing.png");

	//m_allTex.hitstopTex.Load(L"Data/Texture/Motion/Esper/Hitstop.png");
	//m_allTex.blowTex.Load(L"Data/Texture/Motion/Esper/Blow.png");
	//m_allTex.havethingsTex.Load(L"Data/Texture/Motion/Esper/HaveThings.png");
	//m_allTex.throwTex.Load(L"Data/Texture/Motion/Esper/Throw.png");

	static bool isInitialized = false;

	if (!isInitialized)
	{
		m_allTex.idleTex.Load(L"Data/Texture/Motion/Ghost/Idle.png");//待機
		m_allTex.moveTex.Load(L"Data/Texture/Motion/Ghost/Move.png");//歩き
		m_allTex.jumpTex.Load(L"Data/Texture/Motion/Ghost/Jump.png");//ジャンプ
		m_allTex.fallTex.Load(L"Data/Texture/Motion/Ghost/Fall.png");//落下
		m_allTex.landingTex.Load(L"Data/Texture/Motion/Ghost/Landing.png");//着地
		m_allTex.hitstopTex.Load(L"Data/Texture/Motion/Ghost/Hitstop.png");//ヒットストップ
		m_allTex.blowTex.Load(L"Data/Texture/Motion/Ghost/Blow.png");//ふっとび
		m_allTex.havethingsTex.Load(L"Data/Texture/Motion/Ghost/HaveThings.png");//拾う
		m_allTex.throwTex.Load(L"Data/Texture/Motion/Ghost/Throw.png");//投げる

		isInitialized = true;
	}

	m_animSpeed = 0.25f;
	m_currentTex = m_allTex.idleTex;

	ChangePetternUV(m_currentState);

}

void Ghost::Draw(XMFLOAT2 Pos, XMFLOAT2 Size, float rotate)
{
	if (m_isLeft){

		if (m_currentState == LANDING) {
			D3D.Draw2D(m_currentTex, XMFLOAT2(Pos.x, Pos.y + 50.0f), XMFLOAT2(Size.x, Size.y), rotate, m_uv, m_texSize);
		}
		else {
			D3D.Draw2D(m_currentTex, Pos, XMFLOAT2(Size.x, Size.y), rotate, m_uv, m_texSize);
		}
	}

	else {

		if (m_currentState == LANDING) {
			D3D.Draw2D(m_currentTex, XMFLOAT2(Pos.x, Pos.y + 50.0f), XMFLOAT2(-Size.x, Size.y), rotate, m_uv, m_texSize);
		}
		else {
			D3D.Draw2D(m_currentTex, Pos, XMFLOAT2(-Size.x, Size.y), rotate, m_uv, m_texSize);
		}
		
	}
}

// 後から枚数が変更されるかもしれないから一応Caseはまとめないでおく
void Ghost::ChangePetternUV(ANIM_STATE currentState)
{
	switch (currentState)
	{
	case IDLE:
		m_uvNumX = 4;
		m_uvNumY = 8;
		m_uvNumMax = 30;
		m_animSpeed = 0.25f;
		break;

	case MOVE:
		m_uvNumX = 4;
		m_uvNumY = 8;
		m_uvNumMax = 30;
		m_animSpeed = 0.25f;
		break;

	case JUMP:
		m_uvNumX = 4;
		m_uvNumY = 8;
		m_uvNumMax = 30;
		m_animSpeed = 0.25f;
		break;

	case FALL:
		m_uvNumX = 4;
		m_uvNumY = 8;
		m_uvNumMax = 29;
		m_animSpeed = 0.25f;
		break;

	case LANDING:
		m_uvNumX = 5;
		m_uvNumY = 6;
		m_uvNumMax = 30;
		m_animSpeed = 0.75f;
		break;

	case HITSTOP:
		m_uvNumX = 4;
		m_uvNumY = 8;
		m_uvNumMax = 30;
		m_animSpeed = 0.25f;
		break;

	case BLOW:
		m_uvNumX = 4;
		m_uvNumY = 8;
		m_uvNumMax = 30;
		m_animSpeed = 0.25f;
		break;

	case HAVETHINGS:
		m_uvNumX = 4;
		m_uvNumY = 8;
		m_uvNumMax = 29;
		m_animSpeed = 0.75f;
		break;

	case THROW:
		m_uvNumX = 4;
		m_uvNumY = 8;
		m_uvNumMax = 30;
		m_animSpeed = 0.75f;
		break;

	default:
		break;
	}
}

Texture Ghost::ReplaceTex()
{
	switch (m_currentState)
	{
	case IDLE:
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
	}

	return m_allTex.idleTex;
}

