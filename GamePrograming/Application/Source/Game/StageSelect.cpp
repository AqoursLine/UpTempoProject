#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/StageSelect.h"
#include "Game/Controller.h"

StageSelect::StageSelect() {
	m_backGroundTex.Load(L"Data/Texture/StageSelectBg.png");
    m_BoxTex.Load(L"Data/Texture/square-1.png");
	m_cursorTex.Load(L"Data/Texture/hand1.png");
	m_stageNumber = SaveData::GetStageNum();

    
    m_cursorPos = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);// カーソル初期位置
    m_cursorSpeed = 1000.0f;// カーソルスピード
    m_padIndex = CTRL.GetGamepadHandle(); // コントローラーのハンドルを取得
    

}

StageSelect::~StageSelect() {
	SaveData::SetStage(m_stageNumber);
	CTRL.ReleaseGamepadHandle(m_padIndex); // ハンドルを解放
}

void StageSelect::Update() {
	//とりあえずエンターキーを押したら終了
	if (CTRL.GetKeyboardTrigger(DIK_RETURN)) {
		m_isFinished = true;
	}


    // 左スティックの移動量を取得
    float deltaX = (CTRL.GetLeftStickHorizontal(m_padIndex)) / 32767.0f; // 正規化 (-1.0 ～ 1.0)
    float deltaY = (CTRL.GetLeftStickVertical(m_padIndex)) / 32767.0f;

    // カーソル位置を更新
    m_cursorPos.x += deltaX * m_cursorSpeed;
    m_cursorPos.y += deltaY * m_cursorSpeed;

    // 十字キーの入力を取得してカーソル移動（オプション）
    if (CTRL.GetGamepadCrossPress(GAMEPAD_CROSS_UP, m_padIndex)) {
        m_cursorPos.y -= m_cursorSpeed;
    }
    if (CTRL.GetGamepadCrossPress(GAMEPAD_CROSS_DOWN, m_padIndex)) {
        m_cursorPos.y += m_cursorSpeed;
    }
    if (CTRL.GetGamepadCrossPress(GAMEPAD_CROSS_LEFT, m_padIndex)) {
        m_cursorPos.x -= m_cursorSpeed;
    }
    if (CTRL.GetGamepadCrossPress(GAMEPAD_CROSS_RIGHT, m_padIndex)) {
        m_cursorPos.x += m_cursorSpeed;
    }

    
}

void StageSelect::Draw() {
    
    //背景
	D3D.Draw2D(m_backGroundTex, XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));

    //ボックス
    D3D.Draw2D(m_BoxTex, XMFLOAT2(500.0f, 500.0f), XMFLOAT2(200.0f, 200.0f));

    //カーソル
	D3D.Draw2D(m_cursorTex, m_cursorPos, XMFLOAT2(200.0f,200.0f));

}
