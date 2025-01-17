#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Movie.h"

#pragma comment(lib, "mf.lib")
#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "Mfuuid.lib")

bool Movie::Load(const std::wstring& filePath) {
	HRESULT hr = MFCreateSourceReaderFromURL(filePath.c_str(), nullptr, &m_pReader);

	if (FAILED(hr)) {
		return false;
	}

	//ビデオタイプ取得
	ComPtr<IMFMediaType> pVideoType = nullptr;

	hr = m_pReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &pVideoType);
	GUID major, sub;
	if (SUCCEEDED(hr)) {
		pVideoType->GetGUID(MF_MT_MAJOR_TYPE, &major);
		pVideoType->GetGUID(MF_MT_SUBTYPE, &sub);
	}

	if (sub == MFVideoFormat_H264) {
		sub = sub;
	} else if (sub == MFVideoFormat_MJPG) {
		sub = sub;
	} else if (sub == MFVideoFormat_RGB32) {
		sub = sub;
	} else {
		sub = sub;
	}


	hr = MFCreateMediaType(pVideoType.GetAddressOf());
	if (FAILED(hr)) {
		return false;
	}

	pVideoType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
	pVideoType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);
	m_pReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr, pVideoType.Get());

	if (!GetVideoFrameDimensions()) {
		return false;
	}

	if (!CreateVideoTexture()) {
		return false;
	}

	if (!CreateShaderResource()) {
		return false;
	}


	return true;
}

void Movie::Draw(const XMFLOAT2& pos, const XMFLOAT2& size) {
	GetVideoFrame();
	UpdateVideoTexture();
	D3D.Draw2D(m_pSrv.Get(), pos, size);
}

bool Movie::CreateVideoTexture() {
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = m_frameWidth;
	texDesc.Height = m_frameHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DYNAMIC;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(D3D.GetDevice()->CreateTexture2D(&texDesc, nullptr, &m_pTexture))) {
		return false;
	}

	return true;
}

bool Movie::CreateShaderResource() {
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	HRESULT hr = D3D.GetDevice()->CreateShaderResourceView(m_pTexture.Get(), &srvDesc, &m_pSrv);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}

bool Movie::GetVideoFrameDimensions() {
	ComPtr<IMFMediaType> mediaType;
	HRESULT hr = m_pReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, &mediaType);
	if (FAILED(hr)) {
		return false;
	}

	UINT64 frameSize = 0;
	hr = mediaType->GetUINT64(MF_MT_FRAME_SIZE, &frameSize);
	if (FAILED(hr)) {
		return false;
	}

	m_frameWidth = (UINT32)(frameSize & 0xFFFFFFFF);
	m_frameHeight = (UINT32)((frameSize >> 32) & 0xFFFFFFFF);

	return true;
}

bool Movie::GetVideoFrame() {
	DWORD streamIndex, flags;
	LONGLONG timestamp;

	HRESULT hr = m_pReader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &streamIndex, &flags, &timestamp, &m_pSample);

	if (SUCCEEDED(hr)) {
		if (flags & MF_SOURCE_READERF_ENDOFSTREAM) {
			return false;
		}

		if (m_pSample) {
			//サンプルのフォーマットを確認
			ComPtr<IMFMediaType> mediaType;
			hr = m_pReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, &mediaType);
			if (FAILED(hr)) {
				return false;
			}

			GUID subType;
			hr = mediaType->GetGUID(MF_MT_SUBTYPE, &subType);
			if (FAILED(hr)) {
				return false;
			}

			//フォーマットに応じて処理
			if (subType == MFVideoFormat_H264) {

			}

			hr = m_pSample->ConvertToContiguousBuffer(&m_pMediaBuffer);
			if (FAILED(hr)) {
				return false;
			}
		}
	}

	return true;
}

bool Movie::UpdateVideoTexture() {
	BYTE* data = nullptr;
	DWORD maxLength, currentLength;
	HRESULT hr = m_pMediaBuffer->Lock(&data, &maxLength, &currentLength);

	if (FAILED(hr)) {
		return false;
	}

	//テクスチャにデータを転送
	D3D11_BOX box = {0, 0, 0, m_frameWidth, m_frameHeight, 1};
	D3D.GetDeviceContext()->UpdateSubresource(m_pTexture.Get(), 0, &box, data, m_frameWidth * 4, 0);

	m_pMediaBuffer->Unlock();

	return true;
}

bool Movie::DecodeH264Frame() {
	//H.264 デコーダの作成
	ComPtr<IMFAttributes> pAttributes;
	HRESULT hr = MFCreateAttributes(&pAttributes, 1);
	if (FAILED(hr)) {
		return false;
	}

//	hr = pAttributes->SetGUID(MF_TRANSFORM_CLSID, CLSID_CMSH264DecoderMFT);
	if (FAILED(hr)) {
		return false;
	}

	//デコーダの作成
	ComPtr<IMFTransform> pDecoder;


	return false;
}

bool Movie::DecodeMJPGFrame() {
	return false;
}

bool Movie::HandleUnSupportedFormat() {
	return false;
}
