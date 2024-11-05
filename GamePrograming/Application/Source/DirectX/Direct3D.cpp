/******************************************************
* Direct3D.cpp		ダイレクトX11を管理
* 制作者：ミヤタジョウジ
* 作成日：2024/10/5
* 最終更新日：2024/10/21
*******************************************************/
#include "framework.h"

#include "Direct3D.h"

/******************************************************
* ダイレクトX初期化
*******************************************************/
bool Direct3D::Initialize(HWND hWnd, int width, int height) {
	//ファクトリー作成
	//※ビデオ・グラフィックの設定の列挙や指定に使用されるオブジェクト
	ComPtr<IDXGIFactory> factory;
	if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)))) {
		return false;
	}

	//デバイス生成
	//※主にリソース作成時に使用するオブジェクト

	//フィーチャーレベル列挙
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,	//Direct3D 11.1	ShaderModel 5
		D3D_FEATURE_LEVEL_11_0,	//Direct3D 11	ShaderModel 5
		D3D_FEATURE_LEVEL_10_1,	//Direct3D 10.1	ShaderModel 4
		D3D_FEATURE_LEVEL_10_0,	//Direct3D 10.0	ShaderModel 4
		D3D_FEATURE_LEVEL_9_3,	//Direct3D 9.3	ShaderModel 3
		D3D_FEATURE_LEVEL_9_2,	//Direct3D 9.2	ShaderModel 3
		D3D_FEATURE_LEVEL_9_1,	//Direct3D 9.1	ShaderModel 3
	};

	//デバイスとデバイスコンテキストを作成
	D3D_FEATURE_LEVEL featureLevel;
	if (FAILED(D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				0,
				featureLevels,
				_countof(featureLevels),
				D3D11_SDK_VERSION,
				m_device.GetAddressOf(),
				&featureLevel,
				m_deviceContext.GetAddressOf()))) {
		return false;
	}

	//スワップチェイン作成
	//※フロントバッファに表示可能なバックバッファを持つもの

	//スワップチェインの設定データ
	DXGI_SWAP_CHAIN_DESC scDesc = {};
	scDesc.BufferDesc.Width = width;						//画面の幅
	scDesc.BufferDesc.Height = height;						//画面の高さ
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//バッファの形式
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scDesc.BufferDesc.RefreshRate.Numerator = 60;			//リフレッシュレート(分母)
	scDesc.BufferDesc.RefreshRate.Denominator = 1;			//リフレッシュレート(分子)
	scDesc.SampleDesc.Count = 1;							//MSAAは使用しない
	scDesc.SampleDesc.Quality = 0;							//MSAAは使用しない
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//バッファの使用方法
	scDesc.BufferCount = 1;									//バッファの数
	scDesc.OutputWindow = hWnd;
	scDesc.Windowed = TRUE;									//ウィンドウモード
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//スワップチェインの作成
	if (FAILED(factory->CreateSwapChain(m_device.Get(), &scDesc, m_swapChain.GetAddressOf()))) {
		return false;
	}

	//スワップチェインからバックバッファリソース取得
	ComPtr<ID3D11Texture2D> pBackBuffer;
	if (FAILED(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)))) {
		return false;
	}

	//バックバッファリソース用のRTVを作成
	if (FAILED(m_device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, m_backBufferView.GetAddressOf()))) {
		return false;
	}

	//ステンシル用テクスチャー作成
	ID3D11Texture2D* depthTexture = nullptr;
	D3D11_TEXTURE2D_DESC td = {};
	td.Width = scDesc.BufferDesc.Width;
	td.Height = scDesc.BufferDesc.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc = scDesc.SampleDesc;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	if (FAILED(m_device->CreateTexture2D(&td, nullptr, &depthTexture))) {
		return false;
	}

	//ステンシルターゲット作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags = 0;
	if (FAILED(m_device->CreateDepthStencilView(depthTexture, &dsvd, m_depthStencilView.GetAddressOf()))) {
		return false;
	}
	m_deviceContext->OMSetRenderTargets(1, m_backBufferView.GetAddressOf(), m_depthStencilView.Get());

	//ビューポートの設定
	D3D11_VIEWPORT vp = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };
	m_deviceContext->RSSetViewports(1, &vp);


	//ラスタライザステート設定
	D3D11_RASTERIZER_DESC rd = {};
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;

	ID3D11RasterizerState* rs = nullptr;
	m_device->CreateRasterizerState(&rd, &rs);
	m_deviceContext->RSSetState(rs);


	//ブレンドステート設定
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* blendState = nullptr;
	if (FAILED(m_device->CreateBlendState(&blendDesc, &blendState))) {
		return false;
	}
	m_deviceContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);

	//深度ステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = FALSE;
	//深度有効
	depthStencilDesc.DepthEnable = TRUE;
	if (FAILED(m_device->CreateDepthStencilState(&depthStencilDesc, m_depthStencilEnable.GetAddressOf()))) {
		return false;
	}
	//深度無効
	depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	if (FAILED(m_device->CreateDepthStencilState(&depthStencilDesc, m_depthStencilDisable.GetAddressOf()))) {
		return false;
	}

	m_deviceContext->OMSetDepthStencilState(m_depthStencilEnable.Get(), NULL);

	//サンプラ―ステート設定
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;	
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		//テクスチャアドレッシングモードをWrapに
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		//テクスチャアドレッシングモードをWrapに
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;		//テクスチャアドレッシングモードをWrapに
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//ステートオブジェクト作成
	ComPtr<ID3D11SamplerState> samplerState;
	m_device->CreateSamplerState(&samplerDesc, &samplerState);

	//各シェーダーの0番目にセット(実際は必要なシェーダーだけセット)
	m_deviceContext->VSSetSamplers(0, 1, samplerState.GetAddressOf());	//頂点シェーダーの0番目にセット
	m_deviceContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());	//ピクセルシェーダーの0番目にセット
	m_deviceContext->GSSetSamplers(0, 1, samplerState.GetAddressOf());	//ジオメトリシェーダーの0番目にセット
	m_deviceContext->CSSetSamplers(0, 1, samplerState.GetAddressOf());	//コンピュートシェーダーの0番目にセット
	

	//シェーダーの作成
	//頂点シェーダーを読込&コンパイル
	ComPtr<ID3DBlob> compiledVS;
	if (FAILED(D3DCompileFromFile(L"Shader/SpriteShader.hlsl", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &compiledVS, nullptr))) {
		return false;
	}
	//ピクセルシェーダーを読込&コンパイル
	ComPtr<ID3DBlob> compiledPS;
	if (FAILED(D3DCompileFromFile(L"Shader/SpriteShader.hlsl", nullptr, nullptr, "PS", "ps_5_0", 0, 0, &compiledPS, nullptr))) {
		return false;
	}

	//頂点シェーダー作成
	if (FAILED(m_device->CreateVertexShader(compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), nullptr, m_spriteVS.GetAddressOf()))) {
		return false;
	}
	//ピクセルシェーダー作成
	if (FAILED(m_device->CreatePixelShader(compiledPS->GetBufferPointer(), compiledPS->GetBufferSize(), nullptr, m_spritePS.GetAddressOf()))) {
		return false;
	}

	//1頂点の詳細な情報
	std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
		{"POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXUV",		0,	DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	//頂点インプットレイアウト作成
	if (FAILED(m_device->CreateInputLayout(&layout[0], (UINT)layout.size(), compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), m_spriteInputLayout.GetAddressOf()))) {
		return false;
	}

	//定数バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(XMMATRIX);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);
	if (FAILED(m_device->CreateBuffer(&hBufferDesc, nullptr, m_constantBuffer.GetAddressOf()))) {
		return false;
	}
	m_deviceContext->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

	//2D用頂点シェーダーセット
	m_deviceContext->VSSetShader(m_spriteVS.Get(), 0, 0);
	//2D用ピクセルシェーダー
	m_deviceContext->PSSetShader(m_spritePS.Get(), 0, 0);
	//入力レイアウトセット
	m_deviceContext->IASetInputLayout(m_spriteInputLayout.Get());


	//頂点バッファ作成
	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//デバイスにバインドする時の種類
	vbDesc.ByteWidth = sizeof(VertexType2D) * 4;	//作成するバッファのバイトサイズ
	vbDesc.MiscFlags = 0;							//その他のフラグ
	vbDesc.StructureByteStride = 0;					//構造化バッファの場合、その構造体のサイズ
	vbDesc.Usage = D3D11_USAGE_DYNAMIC;				//作成するバッファの使用法
	vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (FAILED(m_device->CreateBuffer(&vbDesc, nullptr, m_vertexBuffer.GetAddressOf()))) {
		return false;
	}

	//頂点格納
	m_vertex[0].Pos = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	m_vertex[1].Pos = XMFLOAT3(0.5f, -0.5f, 0.0f);
	m_vertex[2].Pos = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	m_vertex[3].Pos = XMFLOAT3(0.5f, 0.5f, 0.0f);
	for (int i = 0; i < VERTEX_MAX; i++) {
		m_vertex[i].UV = XMFLOAT2(i % 2, i / 2);
	}
	//頂点バッファ更新
	SetVertex();

	return true;
}

/******************************************************
* ダイレクトX終了
*******************************************************/
void Direct3D::Finalize() {
}

/******************************************************
* 描画
*******************************************************/
void Direct3D::Draw2D(const Texture& tex, float x, float y, float w, float h, float r, float u, float v, float tw, float th) {
	//頂点バッファを描画で使えるようにセットする
	UINT stride = sizeof(VertexType2D);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

	//プロジェクションマトリクス設定
	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	SetProjectionMatrix(projection);

	//ビューマトリクス設定
	XMMATRIX view;
	view = XMMatrixIdentity();
	SetViewMatrix(view);

	//移動回転マトリクス設定
	XMMATRIX world, scale, rot, pos;
	scale = XMMatrixScaling(w, h, 0);
	rot = XMMatrixRotationZ(XMConvertToRadians(r));
	pos = XMMatrixTranslation(x, y, 0);
	world = scale * rot * pos;
	SetWorldMatrix(world);

	//プリミティブトポロジ―をセット
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//テクスチャ座標設定
	m_vertex[0].UV = XMFLOAT2(u, v);
	m_vertex[1].UV = XMFLOAT2(u + tw, v);
	m_vertex[2].UV = XMFLOAT2(u, v + th);
	m_vertex[3].UV = XMFLOAT2(u + tw, v + th);

	//頂点データ設定
	SetVertex();

	//テクスチャをスロット0にセット
	m_deviceContext->PSSetShaderResources(0, 1, tex.GetHandle().GetAddressOf());

	//実際の描画
	m_deviceContext->Draw(4, 0);
}

/******************************************************
* 頂点データ設定
*******************************************************/
void Direct3D::SetVertex() {
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(m_deviceContext->Map(m_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
		VertexType2D* vertex = (VertexType2D*)pData.pData;

		//頂点バッファのデータを更新
		for (int i = 0; i < VERTEX_MAX; i++) {
			vertex[i].Pos = m_vertex[i].Pos;
			vertex[i].UV = m_vertex[i].UV;
		}

		m_deviceContext->Unmap(m_vertexBuffer.Get(), 0);
	}
}

/******************************************************
* バックバッファクリア
*******************************************************/
void Direct3D::Clear() {
	//画面を青色で塗りつぶす
	float color[4] = { 0.2f, 0.2f, 1.0f, 1.0f };
	m_deviceContext->ClearRenderTargetView(D3D.GetRenderTargetView(), color);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D10_CLEAR_DEPTH, 1.0, 0);
}

/******************************************************
* 深度設定
*******************************************************/
void Direct3D::SetDepthEnable(bool enable) {
	if (enable) {
		m_deviceContext->OMSetDepthStencilState(m_depthStencilEnable.Get(), NULL);
	} else {
		m_deviceContext->OMSetDepthStencilState(m_depthStencilDisable.Get(), NULL);
	}
}

/******************************************************
* プレゼント
*******************************************************/
void Direct3D::Present() {
	m_swapChain->Present(0, 0);
}

/******************************************************
* 2D設定
*******************************************************/
void Direct3D::SetWorldViewProjection2D(void) {
	XMMATRIX worldViewProjection;

	worldViewProjection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	worldViewProjection = XMMatrixTranspose(worldViewProjection);

	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, worldViewProjection);
	m_deviceContext->UpdateSubresource(m_constantBuffer.Get(), 0, nullptr, &matrix, 0, 0);
}
/******************************************************
* ワールド行列設定
*******************************************************/
void Direct3D::SetWorldMatrix(XMMATRIX worldMatrix) {
	m_worldMatrix = worldMatrix;
	UpdateWorldViewProjection();
}
/******************************************************
* ビュー行列設定
*******************************************************/
void Direct3D::SetViewMatrix(XMMATRIX viewMatrix) {
	m_viewMatrix = viewMatrix;
	UpdateWorldViewProjection();
}
/******************************************************
* プロジェクション行列設定
*******************************************************/
void Direct3D::SetProjectionMatrix(XMMATRIX projectionMatrix) {
	m_projectionMatrix = projectionMatrix;
	UpdateWorldViewProjection();
}
/******************************************************
* ワールドビュープロジェクション行列更新
*******************************************************/
void Direct3D::UpdateWorldViewProjection() {
	XMMATRIX worldViewProjection = m_worldMatrix * m_viewMatrix * m_projectionMatrix;
	worldViewProjection = XMMatrixTranspose(worldViewProjection);

	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, worldViewProjection);
	m_deviceContext->UpdateSubresource(m_constantBuffer.Get(), 0, NULL, &matrix, 0, 0);
}

