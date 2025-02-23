#pragma once

#include "DirectX/video_texture_AVI.h"

class ResultRankVideos {
public:
	ResultRankVideos();
	~ResultRankVideos();

	void Update();
	void Draw();

private:
	// 順位のビデオ
	VideoTextureAVI m_1stVideo;
	VideoTextureAVI m_2ndVideo;
	VideoTextureAVI m_3rdVideo;
	VideoTextureAVI m_4thVideo;

	XMFLOAT2 m_1stPos;
	XMFLOAT2 m_2ndPos;
	XMFLOAT2 m_3rdPos;
	XMFLOAT2 m_4thPos;
};
