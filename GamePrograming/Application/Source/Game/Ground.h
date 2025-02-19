/******************************************************
* Ground.h	?n??
* ?????F?~???^?W???E?W
* ?쐬???F2024/11/07
* ?ŏI?X?V???F2024/11/07
*******************************************************/
#pragma once

#include "Game/FieldObject.h"

/****************************************************
* ?n?ʃN???X
*****************************************************/
class Ground : public FieldObject {
public:
	Ground() = delete;
	Ground(const XMFLOAT2& pos, float rot, const XMFLOAT2& size, const std::wstring& fileName, const XMFLOAT2& texPos, const FIELD_DIRECTION fieldDirection);

private:
};
