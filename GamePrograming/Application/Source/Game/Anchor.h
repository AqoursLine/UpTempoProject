/******************************************************
* Anchor.h	アンカー
* 制作者：ユミタリオ
* 作成日：2024/12/06
* 最終更新日：2024/12/06
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

class Anchor : public ThrowObject {
public:
	Anchor() = delete;
	Anchor(float x, float y, float r);
	~Anchor();

private:

};
