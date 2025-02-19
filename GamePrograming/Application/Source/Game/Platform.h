/******************************************************
* platform.h	教壇
* 制作者：murayama
* 作成日：2024/12/05
* 最終更新日：2024/12/05
*******************************************************/
#pragma once
#include "Game/ThrowObject.h"

class Platform : public ThrowObject {
public:
	Platform() = delete;
	Platform(float x, float y, float r);
	~Platform();

private:

};


