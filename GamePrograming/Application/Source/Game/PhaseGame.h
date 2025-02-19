#pragma once

#include "Game/Phase.h"

class PhaseGame :public Phase {
public:
	PhaseGame() = delete;
	PhaseGame(const int phaseNum); // コンストラクタでこのフェーズに登場するモノをThrowObjectManagerに登録
	
private:

};

