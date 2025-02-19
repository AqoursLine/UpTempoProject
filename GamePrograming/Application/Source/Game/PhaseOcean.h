#pragma once

#include "Game/Phase.h"

class PhaseOcean :public Phase {
public:
	PhaseOcean() = delete;
	PhaseOcean(const int phaseNum); // コンストラクタでこのフェーズに登場するモノをThrowObjectManagerに登録

private:

};
