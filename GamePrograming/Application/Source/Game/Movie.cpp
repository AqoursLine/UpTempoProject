#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Movie.h"

#pragma comment(lib, "mf.lib")
#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "Mfuuid.lib")


Movie::~Movie() {
	if (m_pReader) {
		m_pReader->Release();
		m_pReader = nullptr;
	}

	if (m_pSrv) {
		m_pSrv->Release();
		m_pSrv = nullptr;
	}

	if (m_pTexture) {
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

}

bool Movie::Load(const std::wstring& filePath) {
	HRESULT hr = MFCreateSourceReaderFromURL(filePath.c_str(), nullptr, &m_pReader);

	if (FAILED(hr)) {
		return false;
	}

	//ビデオタイプ取得
	IMFMediaType* pVideoType = nullptr;
	hr = m_pReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &pVideoType);
	if (FAILED(hr)) {
		return false;
	}

	//フレームレート取得
	UINT32 frameRateNumerator = 0;
	UINT32 frameRateDenominator = 0;
	MFGetAttributeRatio(pVideoType, MF_MT_FRAME_RATE, &frameRateNumerator, &frameRateDenominator);
	m_frameDuration = static_cast<float>(frameRateDenominator) / frameRateNumerator;

	//動画サイズ取得
	UINT32 width = 0, height = 0;
	UINT64 frameSize = 0;
	hr = pVideoType->GetUINT64(MF_MT_FRAME_SIZE, &frameSize);
	if (SUCCEEDED(hr)) {
		width = static_cast<UINT32>(frameSize & 0xFFFFFFFF);
		height = static_cast<UINT32>((frameSize >> 32) & 0xFFFFFFFF);
		m_size.x = width;
		m_size.y = height;
	}

	pVideoType->Release();

	CreateTexture();


	return true;
}

void Movie::Update() {
}

void Movie::Draw(const XMFLOAT2& pos, const XMFLOAT2& size) {
	//現在のフレーム時間を記録
	static float lastTime = 0.0f;
	float currentTime = GAMESYS.GetElapsedTime();
	if (currentTime - lastTime < m_frameDuration) {
		return;
	}
	lastTime = currentTime;


	IMFSample* pSample = nullptr;

	HRESULT hr;
	hr = m_pReader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, nullptr, nullptr, nullptr, &pSample);

	if (SUCCEEDED(hr) && pSample) {
		IMFMediaBuffer* pBuffer = nullptr;

		hr = pSample->ConvertToContiguousBuffer(&pBuffer);
		if (SUCCEEDED(hr)) {
			//バッファデータを取得
			BYTE* pData = nullptr;
			DWORD dataLength = 0;

			hr = pBuffer->Lock(&pData, nullptr, &dataLength);
			if (SUCCEEDED(hr)) {
				//DirectX11テクスチャにコピー
				UpdateTextureWithFrameData(pData, dataLength);
				pBuffer->Unlock();
			}
			pBuffer->Release();
		}
		pSample->Release();
	}

	D3D.Draw2D(m_pSrv, pos, size, PIXELMODE_DEFAULT);
}

void Movie::SeekToStart() {
	PROPVARIANT var;
	PropVariantInit(&var);
	var.vt = VT_I8;
	var.hVal.QuadPart = 0;

	//シーク
	m_pReader->SetCurrentPosition(GUID_NULL, var);
	PropVariantClear(&var);

	return;
}

bool Movie::CreateTexture() {
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = m_size.x;
	desc.Height = m_size.y;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = D3D.GetDevice()->CreateTexture2D(&desc, nullptr, &m_pTexture);

	if (FAILED(hr)) {
		return false;
	}

	//テクスチャ生成
	D3D.GetDevice()->CreateShaderResourceView(m_pTexture, nullptr, &m_pSrv);

	return true;
}

void Movie::UpdateTextureWithFrameData(BYTE* pData, DWORD dataLength) {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = D3D.GetDeviceContext()->Map(m_pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	if (SUCCEEDED(hr)) {
		//動画フレームデータをコピー
		const size_t rowPitch = m_size.x * 4;
		for (UINT y = 0; y < m_size.y; y++) {
			memcpy(static_cast<BYTE*>(mappedResource.pData) + y * mappedResource.RowPitch, pData + y * rowPitch, rowPitch);
		}
		D3D.GetDeviceContext()->Unmap(m_pTexture, 0);
	}
}
