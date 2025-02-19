/******************************************************
* PC.h	PC
* ?????F????
* ?쐬???F2024/12/05
* ?ŏI?X?V???F2024/12/05
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

class PersonalComputer : public ThrowObject {
public:
	PersonalComputer() = delete;
	PersonalComputer(float x, float y, float r);
	~PersonalComputer();

private:

};
