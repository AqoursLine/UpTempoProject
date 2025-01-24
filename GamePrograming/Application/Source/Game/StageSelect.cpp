#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/StageSelect.h"
#include "Game/Controller.h"



StageSelect::StageSelect() {
	m_backGroundTex.Load(L"Data/Texture/StageSelectBg.png");
    m_BoxTex.Load(L"Data/Texture/square-1.png");
	m_cursorTex[0].Load(L"Data/Texture/hand1.png");
	m_cursorTex[1].Load(L"Data/Texture/hand2.png");
	m_cursorTex[2].Load(L"Data/Texture/hand3.png");
	m_cursorTex[3].Load(L"Data/Texture/hand4.png");
	m_stageNumber = SaveData::GetStageNum();

    m_totalPlayer = SaveData::GetTotalPlayer();
    


    // カーソル初期化
    for (int i = 0; i < m_totalPlayer; i++) {
        m_cursorPos[i] = XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2); // 初期位置
        m_cursorSpeed[i] = 1000.0f;                                          // カーソルスピード
        m_padIndex[i] = CTRL.GetGamepadHandle();                          // 各コントローラーのハンドルを取得
    }
}

StageSelect::~StageSelect() {
	SaveData::SetStage(m_stageNumber);
    for(int i=0;i<m_totalPlayer;i++)
    { 
        CTRL.ReleaseGamepadHandle(m_padIndex[i]); // ハンドルを解放
    }
	
}

void StageSelect::Update() {
	//とりあえずエンターキーを押したら終了
	if (CTRL.GetKeyboardTrigger(DIK_RETURN)) {
		m_isFinished = true;
	}

    for(int i = 0; i < m_totalPlayer;i++)
    {
        // 左スティックの移動量を取得
        float deltaX = (CTRL.GetLeftStickHorizontal(m_padIndex[i])) / 32767.0f; // 正規化 (-1.0 ～ 1.0)
        float deltaY = (CTRL.GetLeftStickVertical(m_padIndex[i])) / 32767.0f;

        // カーソル位置を更新
        m_cursorPos[i].x += deltaX * m_cursorSpeed[i];
        m_cursorPos[i].y += deltaY * m_cursorSpeed[i];

        // 十字キーの入力を取得してカーソル移動（オプション）
        if (CTRL.GetGamepadCrossPress(GAMEPAD_CROSS_UP, m_padIndex[i])) {
            m_cursorPos[i].y -= m_cursorSpeed[i];
        }
        if (CTRL.GetGamepadCrossPress(GAMEPAD_CROSS_DOWN, m_padIndex[i])) {
            m_cursorPos[i].y += m_cursorSpeed[i];
        }
        if (CTRL.GetGamepadCrossPress(GAMEPAD_CROSS_LEFT, m_padIndex[i])) {
            m_cursorPos[i].x -= m_cursorSpeed[i];
        }
        if (CTRL.GetGamepadCrossPress(GAMEPAD_CROSS_RIGHT, m_padIndex[i])) {
            m_cursorPos[i].x += m_cursorSpeed[i];
        }

        // 画面外にカーソルが出ないよう制限
        m_cursorPos[i].x = max(0.0f, min(SCREEN_WIDTH, m_cursorPos[i].x));
        m_cursorPos[i].y = max(0.0f, min(SCREEN_HEIGHT, m_cursorPos[i].y));
    }
    
}

void StageSelect::Draw() {
    
    //背景
	D3D.Draw2D(m_backGroundTex, XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));

    //ボックス
    D3D.Draw2D(m_BoxTex, XMFLOAT2(500.0f, 500.0f), XMFLOAT2(200.0f, 200.0f));

    
     // 各コントローラーのカーソルを描画
    for (int i = 0; i < m_totalPlayer; i++) {
        D3D.Draw2D(m_cursorTex[i], m_cursorPos[i], XMFLOAT2(200.0f, 200.0f));
    }
}
