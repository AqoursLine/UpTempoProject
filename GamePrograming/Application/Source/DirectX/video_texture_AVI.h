#pragma once

struct IMFSourceReader;
struct IMFSample;

class VideoTextureAVI {
public:
	VideoTextureAVI() = delete;
	VideoTextureAVI(const std::wstring& fileName);
	~VideoTextureAVI();

	//動画更新
	void Update(float deltaTime);

	//動画再生終了後にもう一度自動で再生するかのフラグを設定
	void SetIsAutoLoop(bool autoLoop) { m_isAutoLoop = autoLoop; }
	//動画の更新を行うかのフラグを設定
	void SetPause(bool pause) { m_isPause = pause; }

	//動画の再生が終了したかどうかを取得
	bool GetIsFinished() const { return m_isFinished; }
	//動画のアスペクト比を取得
	float GetAspectRatio() const { return m_aspect; }
	//動画の高さを取得
	UINT32 GetVideoHeight() const { return m_height; }
	//動画の幅を取得
	UINT32 GetVideoWidth() const { return m_width; }

	//ShaderResourceViewを取得
	ID3D11ShaderResourceView* GetSRV() const { return m_srv.Get(); }

	//動画を最初の位置に戻す
	void Reset();
private:
	ComPtr<ID3D11Texture2D> m_texture;
	ComPtr<ID3D11ShaderResourceView> m_srv;

	ComPtr<IMFSourceReader> m_reader;

	bool m_isFailed = true;

	UINT32 m_width = 0;
	UINT32 m_height = 0;
	float m_aspect = 0.0f;

	float m_elapsedTime = 0;
	float m_frameTime = 1.0f / 60.0f;

	bool m_isPause = false;
	bool m_isAutoLoop = false;
	bool m_isFinished = false;

	void Decode(HRESULT hr, IMFSample* pSample);
};

