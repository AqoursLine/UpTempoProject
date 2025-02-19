#pragma once

#include "Game/Phase.h"

class PhaseAmusementPark :public Phase {
public:
	PhaseAmusementPark() = delete;
	PhaseAmusementPark(const int phaseNum); // コンストラクタでこのフェーズに登場するモノをThrowObjectManagerに登録

private:

};