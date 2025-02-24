/******************************************************
* Ground.cpp	地面
* 制作者：ミヤタジョウジ
* 作成日：2024/11/07
* 最終更新日：2024/11/07
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"

#include "Game/Ground.h"

/****************************************************
* 地面初期化
*****************************************************/
Ground::Ground(const XMFLOAT2& pos, float rot, const XMFLOAT2& size, const XMFLOAT2& offset, int uvNum, const FIELD_DIRECTION fieldDirection) : FieldObject(pos, rot, size, offset, uvNum,fieldDirection) {

	SetTag("Ground");

	m_maxHp = m_hp = 15;
}



