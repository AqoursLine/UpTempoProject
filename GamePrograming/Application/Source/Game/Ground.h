/******************************************************
* Ground.h	地面
* 制作者：ミヤタジョウジ
* 作成日：2024/11/07
* 最終更新日：2024/11/07
*******************************************************/
#pragma once

#include "Game/FieldObject.h"

/****************************************************
* 地面クラス
*****************************************************/
class Ground : public FieldObject {
public:
	Ground() = delete;
	Ground(const XMFLOAT2& pos, float rot, const XMFLOAT2& size, const std::wstring& fileName, const XMFLOAT2& texPos);

private:
};
