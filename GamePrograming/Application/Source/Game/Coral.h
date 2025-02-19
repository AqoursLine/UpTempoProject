/******************************************************
* Coral.h	サンゴ
* 制作者：カワマタトウ
* 作成日：2024/12/06
* 最終更新日：2024/12/06
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

class Coral : public ThrowObject {
public:
	Coral() = delete;
	Coral(float x, float y, float r);
	~Coral();

private:

};