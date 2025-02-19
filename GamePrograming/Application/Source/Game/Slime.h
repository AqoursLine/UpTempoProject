/******************************************************
* Slime.h	?X???C??
* ?????F???~?^???I
* ?쐬???F2024/12/05
* ?ŏI?X?V???F2024/12/05
*******************************************************/
#pragma once

#include "Game/ThrowObject.h"

class Slime : public ThrowObject {
public:
	Slime() = delete;
	Slime(float x, float y, float r);
	~Slime();
	void HitPlayer(Player* p)override;

private:

};
