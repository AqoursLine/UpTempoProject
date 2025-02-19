/******************************************************
* Sword.h	ソード
* 制作者：ユミタリオ
* 作成日：2024/12/05
* 最終更新日：2024/12/05
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

class Sword : public ThrowObject {
public:
	Sword() = delete;
	Sword(float x, float y, float r);
	~Sword();

private:

};
