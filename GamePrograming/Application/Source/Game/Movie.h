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
#include <mfidl.h>
#include <mftransform.h>
#include <wmcodecdsp.h>

class Movie {
public:
	Movie() = default;
	~Movie() = default;

	bool Load(const std::wstring& filePath);

//	void Update();
	void Draw(const XMFLOAT2& pos, const XMFLOAT2& size);

private:
	ComPtr<IMFSourceReader> m_pReader = nullptr;
	XMFLOAT2 m_size;

	ComPtr<IMFSample> m_pSample = nullptr;
	ComPtr<IMFMediaBuffer> m_pMediaBuffer = nullptr;

	ComPtr<ID3D11Texture2D> m_pTexture = nullptr;
	ComPtr<ID3D11ShaderResourceView> m_pSrv = nullptr;

	UINT32 m_frameWidth = 0;
	UINT32 m_frameHeight = 0;

	bool CreateVideoTexture();
	bool CreateShaderResource();
	bool GetVideoFrameDimensions();

	bool GetVideoFrame();
	bool UpdateVideoTexture();

	bool DecodeH264Frame();
	bool DecodeMJPGFrame();
	bool HandleUnSupportedFormat();

};

