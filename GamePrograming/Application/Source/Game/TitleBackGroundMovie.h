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
	// ?ォ?炩?Ԃ????[?o???C?֘A?̐ݒ?
	Texture m_UI_backGroundOverlay;
	XMFLOAT2 m_overLayPos;
	XMFLOAT2 m_overLaySize;
	float	 m_overLayRot;
	XMFLOAT2 m_overLayUV;
	XMFLOAT2 m_overLayTexSize;
	XMFLOAT4 m_overLayColor;
	//========================================
	
	//========================================
	// ?????A
	VideoTexture m_backMovie;		//?@?w?i????
	XMFLOAT2 m_moviePos;
	XMFLOAT2 m_movieSize;
	// =======================================
};
