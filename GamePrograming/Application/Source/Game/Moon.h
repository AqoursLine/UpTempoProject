/******************************************************
* Moon.h	月
* 制作者：センバソラ
* 作成日：2024/12/05
* 最終更新日：2025/2/13
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

class Moon : public ThrowObject {
public:
	Moon() = delete;
	Moon(float x, float y, float r);
	~Moon();

private:

};


