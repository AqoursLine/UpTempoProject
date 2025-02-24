#pragma once

#include "Game/Transition.h"

class ResultToTitleBar {
public:
	ResultToTitleBar();
	~ResultToTitleBar();

	void Update();
	void Draw();

private:
	Transition m_barAnim;
};
