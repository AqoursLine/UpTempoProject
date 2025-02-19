/******************************************************
* Coin.h	?R?C??
* ?????F???~?^???I
* ?쐬???F2024/12/05
* ?ŏI?X?V???F2024/12/05
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

class Coin : public ThrowObject {
public:
	Coin() = delete;
	Coin(float x, float y, float r);
	~Coin();

private:

};
