/******************************************************
* Balloon.h	風船
* 制作者：カワマタトウ
* 作成日：2024/12/28
* 最終更新日：2024/12/28
*******************************************************/
#pragma once
#include "Game/ThrowObject.h"

class Balloon : public ThrowObject {
public:
	Balloon() = delete;
	Balloon(float x, float y, float r);
	~Balloon();

private:

};