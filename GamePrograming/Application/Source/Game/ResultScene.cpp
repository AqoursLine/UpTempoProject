#include "framework.h"
#include "DirectX/DirectX.h"
#include "ResultScene.h"
#include "Game/Controller.h"

ResultScene::ResultScene() {
	m_resultTex.Load(L"Data/Texture/result.png");
	m_goTitleTex.Load(L"Data/Texture/GoTitle.png");
	m_resultBgTex.Load(L"Data/Texture/ResultBg.png");

	m_resultCharacter = new ResultCharacter();


	m_state = RESULT_START;

	m_camera = new Camera();
}

ResultScene::~ResultScene() {

	if (m_camera) delete m_camera;
	if (m_resultCharacter) delete m_resultCharacter;
}

void ResultScene::Update() {
	switch (m_state) {
		case RESULT_START:
			Start();
			break;
		case RESULT_RESULT:
			Result();
			break;
		case RESULT_WAIT:
			Wait();
			break;
		case RESULT_TRANSITION:
			Transition();
			break;
	}
}

void ResultScene::Draw() {
	m_camera->Draw();

	//?w?i?`??
	D3D.Draw2D(m_resultBgTex, XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));

	//???ʔ??\???`??
	D3D.Draw2D(m_resultTex, XMFLOAT2(SCREEN_WIDTH * 0.5f, 150.0f), XMFLOAT2(450.0f, 160.0f));


	m_resultCharacter->Draw();

	//?R???e?B?j???[?I????(?ォ??????)
	//?l?Y?~?Ԃ????ŕ`?悵?Ȃ?
	if (m_state < RESULT_WAIT) {
		return;
	}
	//?X?e?[?g??WAIT?ȏ?????
//	D3D.Draw2D(m_goTitleTex, XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), XMFLOAT2(830.0f, 160.0f));


	//?g?????W?V?????`??
	//?X?e?[?g??TRANSITION?ȏ?????
	if (m_state < RESULT_TRANSITION) {
		return;
	}

}

/******************************************************
* ???t???[???҂?
*******************************************************/
void ResultScene::Start() {
	m_stateCount++;

	if (m_stateCount >= 30) {
		m_state = RESULT_RESULT;
	}
}

/******************************************************
* ???ʔ??\
*******************************************************/
void ResultScene::Result() {


	//???ʔ??\???I???܂ő҂?
	m_resultCharacter->Update();

	//?I???????e?[?g?ύX
	if (m_resultCharacter->GetIsFinished()) {
		m_state = RESULT_WAIT;
	}
}

/******************************************************
* ???͑҂?
*******************************************************/
void ResultScene::Wait() {
	//???͑҂????̔w?i?ɂ??Ă̓??쏈??



	//?v???C???[???{?^?????͂???????
	if (CTRL.GetKeyboardTrigger(DIK_RETURN) || CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CIRCLE, 0)) {
		m_state = RESULT_TRANSITION;
	}
}

/******************************************************
* ?g?????W?V????
*******************************************************/
void ResultScene::Transition() {
	//?g?????W?V?????Đ?????

	//?g?????W?V?????Đ??I??
	if (true) {
		m_isFinished = true;
	}
}
