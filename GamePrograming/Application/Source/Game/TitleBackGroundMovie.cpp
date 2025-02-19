#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "TitleBackGroundMovie.h"

BackGroundMovie::BackGroundMovie()
{
	// =========================================================================
	// ???????K?v?ȏ??̏?????
	m_moviePos = { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 };	// ?????ʒu
	m_movieSize = { SCREEN_WIDTH,SCREEN_HEIGHT };			// ?????T?C?Y

	m_backMovie.create("Data/Movie/yasodance.mp4");				// ?w?i?????[?^

	m_backMovie.setLooping(true);							// ???[?v?ݒ?
	// =========================================================================


	// =========================================================================
	// ?w?i???悩?炩?Ԃ????[?o?[???C?e?N?X?`???̐ݒ?
	m_UI_backGroundOverlay.Load(L"Data/Texture/OverLay.png"); // ?e?N?X?`??
	m_overLayPos = { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 };	// ?ʒu
	m_overLaySize = { SCREEN_WIDTH,SCREEN_HEIGHT };			// ?T?C?Y
	m_overLayRot = 0.0f;
	m_overLayUV = { 0.0f,0.0f };
	m_overLayTexSize = { 1.0f,1.0f };
	m_overLayColor = { 1.0f,1.0f,1.0f,0.65f };
	// =========================================================================
}

BackGroundMovie::~BackGroundMovie()
{
	m_backMovie.destroy(); // ?????J??
}

void BackGroundMovie::Update()
{
	// ?????X?V
	m_backMovie.update(GAMESYS.GetDletaTime());

}

void BackGroundMovie::Draw()
{
	//?@??????
	D3D.Draw2D(m_backMovie.getTexture()->shader_resource_view, m_moviePos, m_movieSize, PIXELMODE_MOVIE);

	// ?I?[?o???C?`??
	D3D.Draw2D(m_UI_backGroundOverlay, m_overLayPos, m_overLaySize, m_overLayRot, m_overLayUV, m_overLayTexSize, m_overLayColor, PIXELMODE_DEFAULT);
}
