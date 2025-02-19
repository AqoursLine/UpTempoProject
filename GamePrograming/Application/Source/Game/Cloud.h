/******************************************************
* Cloud.h	?_
* ?????F?Z???o?\??
* ?쐬???F2024/12/05
* ?ŏI?X?V???F2025/2/13
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

class Cloud : public ThrowObject {
public:
	Cloud() = delete;
	Cloud(float x, float y, float r);
	~Cloud();

private:

};
