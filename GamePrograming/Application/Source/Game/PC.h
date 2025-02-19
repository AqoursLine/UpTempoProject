/******************************************************
* PC.h	PC
* 制作者：仙波空
* 作成日：2024/12/05
* 最終更新日：2024/12/05
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


