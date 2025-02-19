/******************************************************
* Shell.h	?L
* ?????F????
* ?쐬???F2024/12/06
* ?ŏI?X?V???F2024/12/06
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

class Shell : public ThrowObject {
public:
	Shell() = delete;
	Shell(float x, float y, float r);
	~Shell();

private:

};
