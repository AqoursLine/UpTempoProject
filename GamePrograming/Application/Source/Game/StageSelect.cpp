#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/StageSelect.h"
#include "Game/Controller.h"



StageSelect::StageSelect() {

    //ステージの番号
    m_stageNumber = SaveData::GetStageNum();

    //総プレイヤー数の代入
    m_totalPlayer = SaveData::GetTotalPlayer()-1;

    //背景テクスチャ
	m_backGroundTex.Load(L"Data/Texture/StageSelectBg.png");
    m_backGroundPos = XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  
    //カーソルテクスチャ
    m_cursorTex[0].Load(L"Data/Texture/hand1.png");
    m_cursorTex[1].Load(L"Data/Texture/hand2.png");
    m_cursorTex[2].Load(L"Data/Texture/hand3.png");
    m_cursorTex[3].Load(L"Data/Texture/hand4.png");

    //ボタンテクスチャ
    for (int i = 0; i < 4; i++)
    {
        m_buttonTex[i].Load(L"Data/Texture/square-1.png");
        m_buttonPos[i]= XMFLOAT2(SCREEN_WIDTH/4*(i+0.5), SCREEN_HEIGHT / 2);
        m_buttonSize[i] = XMFLOAT2(200.0f, 200.0f);

        for (int j = 0; j < 4; j++)
        {
            m_buttonSelected[i][j] = false; // 初期状態は選択されていない
        }
        
    }
   
	
    // カーソル初期化
    for (int i = 0; i < m_totalPlayer; i++) {
        m_cursorPos[i] = XMFLOAT2(SCREEN_WIDTH / 4 + (i+150), SCREEN_HEIGHT / 2);
        m_cursorSpeed[i] = 1000.0f;
        m_padIndex[i] = CTRL.GetGamepadHandle();
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

        
        // 画面外にカーソルが出ないよう制限
        m_cursorPos[i].x = max(0.0f, min(SCREEN_WIDTH, m_cursorPos[i].x));
        m_cursorPos[i].y = max(0.0f, min(SCREEN_HEIGHT, m_cursorPos[i].y));

        // カーソルとボタンの当たり判定
        for (int j = 0; j < 4; j++) {
            float buttonHalfSize = m_buttonSize->x / 2; // ボタンの半径（幅と高さが200）
            if (std::abs(m_cursorPos[i].x - m_buttonPos[j].x) < buttonHalfSize &&
                std::abs(m_cursorPos[i].y - m_buttonPos[j].y) < buttonHalfSize) {
                m_buttonSelected[i][j] = true; // ボタンが選択状態
            }
            else {
                m_buttonSelected[i][j] = false; // ボタンから離れると元に戻る
            }
        }

        //選択状態かつ〇ボタンを押したら
        if (m_buttonSelected[i])
        {
            if (CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CIRCLE,m_totalPlayer))
            {
                m_backGroundPos.x = 100.0f;
            };
        }
    }
    
}

void StageSelect::Draw() {
    
    //背景
	D3D.Draw2D(m_backGroundTex,m_backGroundPos, XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));

    for (int i = 0; i < 4; i++) {
        if (m_buttonSelected[i]) {

            //【カーソルが合う時に動画流れる】



            // ボタンが選択されている場合、別のテクスチャを使用
            D3D.Draw2D(m_cursorTex[i],m_buttonPos[i],m_buttonSize[i]);
        }
        else {
            // 通常状態のボタン
            D3D.Draw2D(m_buttonTex[i], m_buttonPos[i],m_buttonSize[i]);
        }
    }


     // 各コントローラーのカーソルを描画
    for (int i = 0; i < m_totalPlayer; i++) {
        D3D.Draw2D(m_cursorTex[i], m_cursorPos[i], XMFLOAT2(200.0f, 200.0f));
    }
}
