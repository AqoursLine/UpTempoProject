/******************************************************
* Lamp.h	蛍光灯
* 制作者：murayama
* 作成日：2024/01/23
* 最終更新日：
*******************************************************/
#pragma once
#include "Game/StageObject.h"

class Lamp : public StageObject {
public:
	Lamp() = delete;
	Lamp(float x, float y, float r,bool left);
	~Lamp();

private:

};



