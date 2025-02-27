#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "TitleBackGroundMovie.h"

BackGroundMovie::BackGroundMovie()
{
	// =========================================================================
	// 動画描画に必要な上方の初期化
	m_moviePos = { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 };	// 動画の位置
	m_movieSize = { SCREEN_WIDTH,SCREEN_HEIGHT };			// 動画のサイズ

	m_backMovie.create("Data/Movie/TitlePreview.mp4");				// 背景動画データ

	m_backMovie.setLooping(true);							// ループ設定
	// =========================================================================


	// =========================================================================
	// 背景動画からかぶせるオーバーレイテクスチャの設定
	m_UI_backGroundOverlay.Load(L"Data/Texture/OverLay.png"); // テクスチャ
	m_overLayPos = { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 };	// 位置
	m_overLaySize = { SCREEN_WIDTH,SCREEN_HEIGHT };			// サイズ
	m_overLayRot = 0.0f;
	m_overLayUV = { 0.0f,0.0f };
	m_overLayTexSize = { 1.0f,1.0f };
	m_overLayColor = { 1.0f,1.0f,1.0f,0.65f };
	// =========================================================================
}

BackGroundMovie::~BackGroundMovie()
{
	m_backMovie.destroy(); // 動画の開放
}

void BackGroundMovie::Update()
{
	// 動画の更新
	m_backMovie.update(GAMESYS.GetDletaTime());

}

void BackGroundMovie::Draw()
{
	//　動画描画
	D3D.Draw2D(m_backMovie.getTexture()->shader_resource_view, m_moviePos, m_movieSize, PIXELMODE_MOVIE);

	// オーバレイ描画
	D3D.Draw2D(m_UI_backGroundOverlay, m_overLayPos, m_overLaySize, m_overLayRot, m_overLayUV, m_overLayTexSize, m_overLayColor, PIXELMODE_DEFAULT);
}


