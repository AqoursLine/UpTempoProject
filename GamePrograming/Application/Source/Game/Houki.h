#pragma once
/******************************************************
* Houki.h	?
* ?????F ?C?T?T?g??
* ?쐬???F2024/12/05
* ?ŏI?X?V???F2024/12/05
*******************************************************/
#pragma once
#include "Game/ThrowObject.h"

class Houki : public ThrowObject {
public:
	Houki() = delete;
	Houki(float x, float y, float r);
	~Houki();

	void Update() override;
	bool downFirst=true;
private:

};
