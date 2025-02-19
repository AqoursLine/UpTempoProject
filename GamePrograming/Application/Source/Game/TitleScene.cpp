#include "framework.h"
#include "DirectX/DirectX.h"
#include "TitleScene.h"
#include "Game/Controller.h"

TitleScene::TitleScene()
	:m_IN_transition(
		L"Data/Texture/Transition/IN_SpiralTransition.png",
		XMFLOAT2(SCREEN_WIDTH/2,SCREEN_HEIGHT/2),
		XMFLOAT2(SCREEN_WIDTH,SCREEN_HEIGHT),
		0.0f,
		5,
		6,
		30,
		0.8f
	)
{
	//?e?N?X?`???Ǎ?
	m_logoTex.Load(L"Data/Texture/TitleLogo.png");
	m_startTex.Load(L"Data/Texture/StartButton.png");
	m_quitTex.Load(L"Data/Texture/QuitButton.png");
	m_titleChoose.Load(L"Data/Texture/title_choose.png");

	//?I????p???W
	m_pos.x = SCREEN_WIDTH * 0.5f + 525.0f;
	m_pos.y = SCREEN_HEIGHT * 0.5f + 100.0f;
	float scale = 0.6f;
	m_size.x = 540.0f * scale;
	m_size.y = 220.0f * scale;

	//?J????
	m_camera = new Camera();

	//?X?e?[?g
	m_state = TITLE_START;

	// ?w?i????new
	m_backMovie = std::make_unique<BackGroundMovie>();
}

TitleScene::~TitleScene() {
	if (m_camera) {
		delete m_camera;
	}
}

void TitleScene::Update() {
	switch (m_state) {
		case TITLE_RUN:
			Run();
			break;
		case TITLE_START:
			Start();
			break;
		case TITLE_TRANSITION:
			Transition();
			break;
	}
}

void TitleScene::Draw() {
	m_camera->Draw();

	m_backMovie->Draw();

	//?^?C?g?????̕`??
	//if???͎g?킸???̂܂ܕ`??
	//???S
	D3D.Draw2D(m_logoTex, XMFLOAT2(SCREEN_WIDTH * 0.5f + 615.0f, SCREEN_HEIGHT * 0.5f - 250.0f), XMFLOAT2(1920 * 0.4f, 1080 * 0.4f));
	
	//?I????w?i
	D3D.Draw2D(m_titleChoose, XMFLOAT2(m_pos.x + (m_choose * -50.0f), m_pos.y + m_distance * m_choose), XMFLOAT2(m_size.x * 1.2f, m_size.y * 1.2f), 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.1f, 1.0f));
	//?X?^?[?g
	D3D.Draw2D(m_startTex, m_pos, m_size);
	//?N?C?b?g
	D3D.Draw2D(m_quitTex, XMFLOAT2(m_pos.x-50.0f, m_pos.y + m_distance), m_size);

	//?J?n?A?j???[?V?????̕`??
	//?^?C?g?????̏??킹?????ɕ`?悷?邱?ƂŃX???[?Y?Ƀ^?C?g?????ɑJ?ڏo?????ł͂Ƃ???????
	if (m_state == TITLE_START) {
	}

	//?g?????W?V?????`??
	if (m_state == TITLE_TRANSITION) {
		m_IN_transition.Draw();
	}
}

/******************************************************
* ?I????̑???
*******************************************************/
void TitleScene::Run() {
	//?I??
	bool isUpTrigger = false, isDownTrigger = false;
	//?L?[?{?[?h
	if (CTRL.GetKeyboardTrigger(DIK_W)) {
		isUpTrigger = true;
	}
	if (CTRL.GetKeyboardTrigger(DIK_S)) {
		isDownTrigger = true;
	}
	//?Q?[???p?b?h
	if (CTRL.GetGamepadMax() > 0) {
		if (CTRL.GetLeftStickVertical(0) > 0) {
			isUpTrigger = true;
		}
		if (CTRL.GetLeftStickVertical(0) < 0) {
			isDownTrigger = true;
		}
	}

	//?ړ?
	if (isUpTrigger || isDownTrigger) {
		m_choose = 1 - m_choose;
	}


	//????
	if (CTRL.GetKeyboardTrigger(DIK_RETURN) || CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CIRCLE, 0)) {
		m_state = TITLE_TRANSITION;
	}

	// ?????A?b?v?f?[?g
	m_backMovie->Update();
}

/******************************************************
* ?N???A?j???[?V???????̑???
*******************************************************/
void TitleScene::Start() {
	//?A?j???[?V?????Đ?????


	//?`?悪?I????
	if (true) {
		m_state = TITLE_RUN;
	}

}

/******************************************************
* ?g?????W?V???????̑???
*******************************************************/
void TitleScene::Transition() {
	//?g?????W?V?????Đ?????
	m_IN_transition.Update();


	//?g?????W?V???????I????
	if (m_IN_transition.IsAnimFinished()) {
		//?V?[???J??
		if (m_choose == 0) {
			m_isFinished = true;
		} else if (m_choose == 1) {
			m_isEnd = true;
		}
	}
}
