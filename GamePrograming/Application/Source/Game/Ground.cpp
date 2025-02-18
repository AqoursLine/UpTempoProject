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
Ground::Ground(const XMFLOAT2& pos, float rot, const XMFLOAT2& size, const std::wstring& fileName, const XMFLOAT2& texPos, const FIELD_DIRECTION fieldDirection) : FieldObject(pos, rot, size, fileName, texPos,fieldDirection) {

	SetTag("Ground");

	m_hp = 50;
}

