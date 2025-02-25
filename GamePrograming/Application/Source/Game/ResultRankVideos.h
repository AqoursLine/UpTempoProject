#pragma once

#include "Game/Transition.h"

class ResultRankAnims {
public:
	ResultRankAnims();
	~ResultRankAnims();

	void Update();
	void Draw();

private:
	// 順位のアニメーション。トランジションクラスなのは許して。
	Transition m_1stAnim;
	Transition m_2ndAnim;
	Transition m_3rdAnim;
	Transition m_4thAnim;

	XMFLOAT2 m_1stPos;
	XMFLOAT2 m_2ndPos;
	XMFLOAT2 m_3rdPos;
	XMFLOAT2 m_4thPos;

	int m_totalPlayer;
};
