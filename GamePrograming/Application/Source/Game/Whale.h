/******************************************************
* whale.h	??????
* ?????Fmurayama
* ?쐬???F2024/12/05
* ?ŏI?X?V???F2024/12/05
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
