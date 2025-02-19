#pragma once

#include "Game/Phase.h"

class PhaseClassRoom :public Phase {
public:
	PhaseClassRoom() = delete;
	PhaseClassRoom(const int phseNum); // コンストラクタでこのフェーズに登場するモノをThrowObjectManagerに登録
private:

};
