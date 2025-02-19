#pragma once

#include "DirectX/video_texture.h"
#include "DirectX/Texture.h"

class BackGroundMovie {
public:
	BackGroundMovie();
	~BackGroundMovie();
	void Update();
	void Draw();

private:
	//========================================
	// 上からかぶせるオーバレイ関連の設定
	Texture m_UI_backGroundOverlay;
	XMFLOAT2 m_overLayPos;
	XMFLOAT2 m_overLaySize;
	float	 m_overLayRot;
	XMFLOAT2 m_overLayUV;
	XMFLOAT2 m_overLayTexSize;
	XMFLOAT4 m_overLayColor;
	//========================================
	
	//========================================
	// 動画関連
	VideoTexture m_backMovie;		//　背景動画
	XMFLOAT2 m_moviePos;
	XMFLOAT2 m_movieSize;
	// =======================================
};
