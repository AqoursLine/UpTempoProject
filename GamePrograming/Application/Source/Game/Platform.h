/******************************************************
* platform.h	???d
* ?????Fmurayama
* ?쐬???F2024/12/05
* ?ŏI?X?V???F2024/12/05
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
