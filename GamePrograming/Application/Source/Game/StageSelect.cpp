#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/StageSelect.h"
#include "Game/Controller.h"
#include <random>


StageSelect::StageSelect() {

    //総プレイヤー数の代入
    m_totalPlayer = 1;//SaveData::GetTotalPlayer();

    //背景テクスチャ
	m_backGroundTex.Load(L"Data/Texture/StageSelectBg.png");
    m_backGroundPos = XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  
    //カーソルテクスチャ
    m_cursorTex[0].Load(L"Data/Texture/hand1.png");
    m_cursorTex[1].Load(L"Data/Texture/hand2.png");
    m_cursorTex[2].Load(L"Data/Texture/hand3.png");
    m_cursorTex[3].Load(L"Data/Texture/hand4.png");


    // 各プレイヤーのカーソルロックを解除（全員最初は動かせる）
    m_cursorLocked.resize(m_totalPlayer, false);

    // 各プレイヤーの選択ステージを未選択 (-1) に初期化
    m_selectedStage.resize(m_totalPlayer, -1);
  

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
        m_cursorPos[i] = XMFLOAT2(SCREEN_WIDTH / 4 + (i+1), SCREEN_HEIGHT / 2);
        m_cursorSpeed[i] = 1000.0f;
        m_padIndex[i] = CTRL.GetGamepadHandle();
    }

    //動画の読み込み
    m_video.create("");
    m_video.setLooping(true);

}

StageSelect::~StageSelect() {
    
    for(int i=0;i<m_totalPlayer;i++)
    { 
        CTRL.ReleaseGamepadHandle(m_padIndex[i]); // ハンドルを解放
    }

    SaveData::SetStage(m_stageNumber);
}

void StageSelect::Update() {

    m_video.update(GAMESYS.GetDletaTime());

    for(int i = 0; i < m_totalPlayer;i++)
    {
        if (!m_cursorLocked[i])
        { // カーソルがロックされていない場合のみ移動を許可


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
                float buttonHalfSize = m_buttonSize[j].x / 2; // ボタンの半径（幅と高さが200）
                if (std::abs(m_cursorPos[i].x - m_buttonPos[j].x) < buttonHalfSize &&
                    std::abs(m_cursorPos[i].y - m_buttonPos[j].y) < buttonHalfSize) {
                    m_buttonSelected[i][j] = true; // ボタンが選択状態
                    
                }
                else {
                    m_buttonSelected[i][j] = false; // ボタンから離れると元に戻る
                }
            }
        }
        // 〇ボタンで選択を確定（カーソルをロック）
        if (!m_cursorLocked[i] &&
            CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CIRCLE, m_padIndex[i])) {
            for (int j = 0; j < 4; j++) {
                if (m_buttonSelected[i][j]) {
                    m_selectedStage[i] = j; // 選択したボタンのステージを記録
                    m_cursorLocked[i] = true; // 選択確定
                    break;
                }
            }
           
        }

        // ×ボタンでロック解除（キャンセル）
        if (m_cursorLocked[i] &&
            CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CROSS, m_padIndex[i])) {
            m_cursorLocked[i] = false;
            m_selectedStage[i] = -1; // 選択をリセット
          
        }
        
    }
    
    // すべてのプレイヤーが選択したか確認
    bool allSelected = true;
    for (int i = 0; i < m_totalPlayer; i++) {
        
        if (!m_cursorLocked[i]) {
            allSelected = false;
            break;
        }
        
    }

    if (allSelected) {
        //ここでステージを決める
        DetermineFinalStage();
    }

}

void StageSelect::Draw() {
    D3D.Draw2D(m_backGroundTex, m_backGroundPos, XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));

    for (int i = 0; i < 4; i++) {
        bool isSelected = false;
        for (int j = 0; j < m_totalPlayer; j++) {
            if (m_buttonSelected[j][i]) {
                isSelected = true;
                break;
            }
        }

        if (isSelected) {
            D3D.Draw2D(m_cursorTex[i], m_buttonPos[i], m_buttonSize[i]);
        }
        else {
            D3D.Draw2D(m_buttonTex[i], m_buttonPos[i], m_buttonSize[i]);
        }
    }

    for (int i = 0; i < m_totalPlayer; i++) {
        D3D.Draw2D(m_cursorTex[i], m_cursorPos[i], XMFLOAT2(200.0f, 200.0f));
    }
}

 //ステージ決定処理
void StageSelect::DetermineFinalStage() {

    std::unordered_map<STAGE, int> stageCount;

    // 各プレイヤーの選択をカウント
    for (int i = 0; i < m_totalPlayer; i++) {
        if (m_selectedStage[i] != -1) {  // 有効な選択のみカウント
            stageCount[static_cast<STAGE>(m_selectedStage[i])]++;
        }
    }

    // 最も多く選ばれたステージを探す
    int maxCount = 0;
    STAGE mostFrequentStage = STAGE_GAME; // デフォルト値

    for (const auto& entry : stageCount) {
        if (entry.second > maxCount) {
            maxCount = entry.second;
            mostFrequentStage = entry.first;
        }
    }

    m_stageNumber = mostFrequentStage;

    m_isFinished = true;

    //デバッグ用
    //m_stageNumber = STAGE_GAME;
    
    //m_isFinished = true;
}



