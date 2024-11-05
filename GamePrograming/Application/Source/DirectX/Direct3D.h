/******************************************************
* Direct3D.h		ダイレクトX11を管理
* 制作者：ミヤタジョウジ
* 作成日：2024/10/5
* 最終更新日：2024/10/21
*******************************************************/
#pragma once

#include "DirectX.h"

constexpr int VERTEX_MAX = (4);

class Texture;

//2D用頂点構造体
struct VertexType2D {
	XMFLOAT3 Pos;	//座標
	XMFLOAT2 UV;	//UV座標
};

/****************************************************
* Direct3Dクラス
*****************************************************/
class Direct3D {
public:
	//初期化
	bool Initialize(HWND hWnd, int width, int height);
	//終了
	void Finalize();

	//=====================================================
	// 描画
	// 引数
	//	Texture		テクスチャクラス
	//	float x		座標X
	//	float y		座標Y
	//	float w		サイズX
	//	float h		サイズY
	//	float r		回転
	//	float u		テクスチャ座標X
	//	float v		テクスチャ座標y
	//	float tw	テクスチャ表示幅
	//	float th	テクスチャ表示高さ
	//=====================================================
	void Draw2D(const Texture& text, float x, float y, float w, float h, float r, float u, float v, float tw, float th);
	//頂点データ設定
	void SetVertex();
	//クリア
	void Clear();
	//深度
	void SetDepthEnable(bool eneble);
	//プレゼント
	void Present();

	//マトリクス
	void SetWorldViewProjection2D(void);
	void SetWorldMatrix(XMMATRIX worldMatrix);
	void SetViewMatrix(XMMATRIX viewMatrix);
	void SetProjectionMatrix(XMMATRIX projectionMatrix);
	void UpdateWorldViewProjection();

	//ゲッター
	ID3D11Device* GetDevice() { return m_device.Get(); }
	ID3D11DeviceContext* GetDeviceContext() { return m_deviceContext.Get(); }
	IDXGISwapChain* GetSwapChain() { return m_swapChain.Get(); }
	ID3D11RenderTargetView* GetRenderTargetView() { return m_backBufferView.Get(); }

private:
	//DirectX用変数
	ComPtr<ID3D11Device>				m_device = nullptr;				//Direct3Dデバイス
	ComPtr<ID3D11DeviceContext>			m_deviceContext = nullptr;		//Direct3Dデバイスコンテキスト
	ComPtr<IDXGISwapChain>				m_swapChain = nullptr;			//スワップチェイン
	ComPtr<ID3D11RenderTargetView>		m_backBufferView = nullptr;		//バックバッファのRTビュー
	ComPtr<ID3D11DepthStencilView>		m_depthStencilView = nullptr;	//ステンシルビュー

	//シェーダー用変数
	ComPtr<ID3D11VertexShader>	m_spriteVS = nullptr;			//頂点シェーダー
	ComPtr<ID3D11PixelShader>	m_spritePS = nullptr;			//ピクセルシェーダー
	ComPtr<ID3D11InputLayout>	m_spriteInputLayout = nullptr;	//入力レイアウト
	ComPtr<ID3D11Buffer>		m_constantBuffer = nullptr;		//定数バッファ

	//深度ステンシル
	ComPtr<ID3D11DepthStencilState>		m_depthStencilEnable = nullptr;		//深度有効ステンシルステート
	ComPtr<ID3D11DepthStencilState>		m_depthStencilDisable = nullptr;	//深度無効ステンシルステート

	//描画用変数
	ComPtr<ID3D11Buffer>		m_vertexBuffer = nullptr;	//頂点バッファ
	VertexType2D				m_vertex[VERTEX_MAX] = {};

	//行列変数
	XMMATRIX					m_worldMatrix;					//ワールド行列
	XMMATRIX					m_viewMatrix;					//ビュー行列
	XMMATRIX					m_projectionMatrix;				//プロジェクション行列


	// 今回このクラスは、どこからでもアクセスできるようにシングルトンパターンにする
	//唯一のインスタンス用のポインタ
	static inline Direct3D* s_instance;
	//コンストラクタはprivateにする
	Direct3D() {}

public:
	//インスタンス作成
	static void CreateInstance() {
		DeleteInstance();

		s_instance = new Direct3D();
	}
	//インスタンス削除
	static void DeleteInstance() {
		if (s_instance != nullptr) {
			delete s_instance;
			s_instance = nullptr;
		}
	}
	//唯一のインスタンスを取得
	static Direct3D& GetInstance() {
		return *s_instance;
	}
};

//Direct3Dの唯一のインスタンスを簡単に取得するためのマクロ
#define D3D Direct3D::GetInstance()
