#pragma once

#include "DirectX/video_texture_AVI.h"

class ResultConfetti {
public:
	ResultConfetti();
	~ResultConfetti();

	void Update();
	void Draw();
private:
	VideoTextureAVI m_confettiVideo;
	XMFLOAT2 m_Pos;
};
