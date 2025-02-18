#include "framework.h"
#include "DirectX/DirectX.h"

#include "Game/GameSystem.h"

// ----------------------------------
// MF API usage copied from mediafoundationsamples\MFMP4ToYUVWithoutMFT
#include <mfapi.h>
#include <mfplay.h>
#include <mfreadwrite.h>
#include <mferror.h>
#include <wmcodecdsp.h>
#include <PropIdl.h>

#include "DirectX/video_texture_AVI.h"


VideoTextureAVI::VideoTextureAVI(const std::wstring& fileName) {
	HRESULT hr = S_OK;

	//ファイルの読み込み
	hr = MFCreateSourceReaderFromURL(fileName.c_str(), nullptr, m_reader.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(nullptr, L"AVIファイルの読み込みに失敗しました", L"エラー", MB_OK);
		m_isFailed = true;
		return;
	}

	//メディアタイプの取得
	ComPtr<IMFMediaType> pMediaType;
	hr = m_reader->GetNativeMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, pMediaType.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(nullptr, L"MediaTypeの取得に失敗しました", L"エラー", MB_OK);
		m_isFailed = true;
		return;
	}

	//解像度を取得
	hr = MFGetAttributeSize(pMediaType.Get(), MF_MT_FRAME_SIZE, &m_width, &m_height);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"解像度の取得に失敗しました", L"エラー", MB_OK);
		m_isFailed = true;
		return;
	}
	//アスペクト比を計算
	m_aspect = static_cast<float>(m_width) / m_height;

	//フレームレートを取得
	UINT32 num = 0, denom = 1;
	hr = MFGetAttributeRatio(pMediaType.Get(), MF_MT_FRAME_RATE, &num, &denom);
	if (SUCCEEDED(hr) && denom > 0) {
		m_frameTime = static_cast<float>(denom) / static_cast<float>(num);
	}

	//メディアタイプの設定
	pMediaType.Reset();
	hr = MFCreateMediaType(pMediaType.GetAddressOf());
	if (SUCCEEDED(hr)) {
		pMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
		pMediaType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);
		hr = m_reader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr, pMediaType.Get());
	}

	if (FAILED(hr)) {
		MessageBox(nullptr, L"MediaTypeの設定に失敗しました", L"エラー", MB_OK);
		m_isFailed = true;
		return;
	}

	//テクスチャ設定
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = m_width;
	texDesc.Height = m_height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DYNAMIC;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hr = D3D.GetDevice()->CreateTexture2D(&texDesc, nullptr, m_texture.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(nullptr, L"テクスチャの作成に失敗しました", L"エラー", MB_OK);
		m_isFailed = true;
		return;
	}

	//SRVの設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	hr = D3D.GetDevice()->CreateShaderResourceView(m_texture.Get(), &srvDesc, m_srv.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(nullptr, L"srvの作成に失敗しました", L"エラー", MB_OK);
		m_isFailed = true;
		return;
	}

	m_isFailed = false;
}

VideoTextureAVI::~VideoTextureAVI() {
}

void VideoTextureAVI::Update(float deltaTime) {
	if (m_isFailed) {
		return;
	}

	if (m_isPause) {
		return;
	}

	if (m_isFinished) {
		return;
	}

	//経過時間を加算
	m_elapsedTime += deltaTime;

	//フレーム更新が必要か
	if (m_elapsedTime < m_frameTime) {
		return;
	}
	m_elapsedTime -= m_frameTime;	//経過時間をリセット


	ComPtr<IMFSample> pSample;
	DWORD dwFlags = 0;

	HRESULT hr = m_reader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, nullptr, &dwFlags, nullptr, pSample.GetAddressOf());

	//動画の終端に達した
	if (dwFlags & MF_SOURCE_READERF_ENDOFSTREAM) {
		if (m_isAutoLoop) {
			PROPVARIANT var;
			PropVariantInit(&var);
			var.vt = VT_I8;
			var.hVal.QuadPart = 0;
			m_reader->SetCurrentPosition(GUID_NULL, var);
			PropVariantClear(&var);

			m_reader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, nullptr, &dwFlags, nullptr, pSample.GetAddressOf());
		} else {
			m_isFinished = true;
		}
	}

	Decode(hr, pSample.Get());
}

void VideoTextureAVI::Reset() {
	PROPVARIANT var;
	PropVariantInit(&var);
	var.vt = VT_I8;
	var.hVal.QuadPart = 0;
	m_reader->SetCurrentPosition(GUID_NULL, var);
	PropVariantClear(&var);

	ComPtr<IMFSample> pSample;
	DWORD dwFlags = 0;

	HRESULT hr = m_reader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, nullptr, &dwFlags, nullptr, pSample.GetAddressOf());

	Decode(hr, pSample.Get());

	m_isFinished = false;
}

void VideoTextureAVI::Decode(HRESULT hr, IMFSample* pSample) {
	if (SUCCEEDED(hr) && pSample) {
		ComPtr<IMFMediaBuffer> pBuffer;
		hr = pSample->ConvertToContiguousBuffer(pBuffer.GetAddressOf());

		if (SUCCEEDED(hr)) {
			BYTE* pData = nullptr;
			DWORD maxLen = 0, curLen = 0;

			hr = pBuffer->Lock(&pData, &maxLen, &curLen);
			if (SUCCEEDED(hr)) {
				D3D11_MAPPED_SUBRESOURCE mappedResource;

				hr = D3D.GetDeviceContext()->Map(m_texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

				if (SUCCEEDED(hr)) {
					BYTE* dest = static_cast<BYTE*>(mappedResource.pData);
					BYTE* src = pData;
					UINT rowPitch = mappedResource.RowPitch;
					UINT imageHeight = m_height;

					for (UINT row = 0; row < imageHeight; ++row) {
						memcpy(dest, src + (imageHeight - 1 - row) * (m_width * 4), m_width * 4);
						dest += rowPitch;
					}

					D3D.GetDeviceContext()->Unmap(m_texture.Get(), 0);
				}

				pBuffer->Unlock();
			}

		}

	}
}

