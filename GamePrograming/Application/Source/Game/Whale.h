/******************************************************
* whale.h	くじら
* 制作者：murayama
* 作成日：2024/12/05
* 最終更新日：2024/12/05
*******************************************************/
#pragma once
#include "Game/ThrowObject.h"

class Whale : public ThrowObject {
public:
	Whale() = delete;
	Whale(float x, float y, float r);
	~Whale();

private:

};



