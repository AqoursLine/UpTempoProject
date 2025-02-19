/******************************************************
* Balloon.h	???D
* ?????F?J???}?^?g?E
* ?쐬???F2024/12/28
* ?ŏI?X?V???F2024/12/28
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
