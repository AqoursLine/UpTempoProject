/******************************************************
* Ground.cpp	?n??
* ?????F?~???^?W???E?W
* ?쐬???F2024/11/07
* ?ŏI?X?V???F2024/11/07
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"

#include "Game/Ground.h"

/****************************************************
* ?n?ʏ?????
*****************************************************/
Ground::Ground(const XMFLOAT2& pos, float rot, const XMFLOAT2& size, const std::wstring& fileName, const XMFLOAT2& texPos, const FIELD_DIRECTION fieldDirection) : FieldObject(pos, rot, size, fileName, texPos,fieldDirection) {

	SetTag("Ground");

	m_hp = 50;
}
