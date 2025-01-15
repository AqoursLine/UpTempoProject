/******************************************************
* Movie.h		動画クラス
* 制作者：ミヤタジョウジ
* 作成日：2024/1/15
* 最終更新日：2024/1/15
*******************************************************/
#pragma once

#include <mfapi.h>
#include <mfplay.h>
#include <mfreadwrite.h>
#include <mfobjects.h>

class Movie {
public:
	Movie() = default;
	~Movie();

	bool Load(const std::wstring& filePath);

	void Update();
	void Draw(const XMFLOAT2& pos, const XMFLOAT2& size);

	void SeekToStart();

private:
	IMFSourceReader* m_pReader = nullptr;
	XMFLOAT2 m_size;

	ID3D11Texture2D* m_pTexture = nullptr;

	ID3D11ShaderResourceView* m_pSrv = nullptr;

	float m_frameDuration;

	bool CreateTexture();
	void UpdateTextureWithFrameData(BYTE* pData, DWORD dataLength);
};

