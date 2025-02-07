#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/StageSelect.h"
#include "Game/Controller.h"
#include <random>
#include <vector>
#include <unordered_map>

StageSelect::StageSelect() {

    //総プレイヤー数の代入
    m_totalPlayer = 3;//SaveData::GetTotalPlayer();

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

    m_buttonTex[0].Load(L"Data/Texture/stage1.png");
    m_buttonTex[1].Load(L"Data/Texture/stage2.png");
    m_buttonTex[2].Load(L"Data/Texture/stage3.png");
    m_buttonTex[3].Load(L"Data/Texture/stage4.png");


    m_buttonPos[0] = XMFLOAT2(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);
    m_buttonPos[1] = XMFLOAT2(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2);
    m_buttonPos[2] = XMFLOAT2(SCREEN_WIDTH/2+400, SCREEN_HEIGHT / 4);
    m_buttonPos[3] = XMFLOAT2(SCREEN_WIDTH/2+400, SCREEN_HEIGHT / 2);

    for (int i = 0; i < 4; i++)
    {
        m_buttonSize[i] = XMFLOAT2(600.0f, 600.0f);

        for (int j = 0; j < 4; j++)
        {
            m_buttonSelected[i][j] = false; // 初期状態は選択されていない
        }
        
    }

    //変わったボタンテクスチャ
    m_ChangebuttonTex[0].Load(L"Data/Texture/stage1(kae).png");
    m_ChangebuttonTex[1].Load(L"Data/Texture/stage2(kae).png");
    m_ChangebuttonTex[2].Load(L"Data/Texture/stage3(kae).png");
    m_ChangebuttonTex[3].Load(L"Data/Texture/stage4(kae).png");
   
	
    // カーソル初期化
    for (int i = 0; i < m_totalPlayer; i++) {
        m_cursorPos[i] = XMFLOAT2(SCREEN_WIDTH / 4 + (i+1), SCREEN_HEIGHT / 2);
        m_cursorSpeed[i] = 1000.0f;
        m_padIndex[i] = CTRL.GetGamepadHandle();
    }

    //動画の読み込み
    m_video.create("Data/Movie/ZTMY.mp4");
    m_video.setLooping(false);

}

StageSelect::~StageSelect() {
    
    for(int i=0;i<m_totalPlayer;i++)
    { 
        CTRL.ReleaseGamepadHandle(m_padIndex[i]); // ハンドルを解放
    }

    SaveData::SetStage(m_stageNumber);

    m_video.destroy();
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

    D3D.Draw2D(m_video.getTexture()->shader_resource_view, XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), PIXELMODE_MOVIE);


    for (int i = 0; i < 4; i++) {
        bool isSelected = false;
        for (int j = 0; j < m_totalPlayer; j++) {
            if (m_buttonSelected[j][i]) {
                isSelected = true;
                break;
            }
        }

        if (isSelected) {
            D3D.Draw2D(m_ChangebuttonTex[i], m_buttonPos[i], m_buttonSize[i]);
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

    //std::unordered_map<STAGE, int> stageCount;

    //// 各プレイヤーの選択をカウント
    //for (int i = 0; i < m_totalPlayer; i++) {
    //    if (m_selectedStage[i] != -1) {  // 有効な選択のみカウント
    //        stageCount[static_cast<STAGE>(m_selectedStage[i])]++;
    //    }
    //}

    //// 最も多く選ばれたステージを探す
    //int maxCount = 0;
    //STAGE mostFrequentStage = STAGE_GAME; // デフォルト値

    //for (const auto& entry : stageCount) {
    //    if (entry.second > maxCount) {
    //        maxCount = entry.second;
    //        mostFrequentStage = entry.first;
    //    }
    //}

    //m_stageNumber = mostFrequentStage;

    //m_isFinished = true;

    ////デバッグ用
    ////m_stageNumber = STAGE_GAME;
    //
    ////m_isFinished = true;

    std::unordered_map<STAGE, int> stageCount;
    int maxCount = 0;

    // 各プレイヤーの選択をカウント
    for (int i = 0; i < m_totalPlayer; i++) {
        if (m_selectedStage[i] != -1) {
            STAGE stage = static_cast<STAGE>(m_selectedStage[i]);
            stageCount[stage]++;
            if (stageCount[stage] > maxCount) {
                maxCount = stageCount[stage];
            }
        }
    }

    // 最大票数のステージをリストアップ
    std::vector<STAGE> candidateStages;
    for (const auto& entry : stageCount) {
        if (entry.second == maxCount) {
            candidateStages.push_back(entry.first);
        }
    }

    // プレイヤーが選択したステージのリストを作成
    std::vector<STAGE> playerChosenStages;
    for (int i = 0; i < m_totalPlayer; i++) {
        if (m_selectedStage[i] != -1) {
            playerChosenStages.push_back(static_cast<STAGE>(m_selectedStage[i]));
        }
    }

    // 同率ならプレイヤーが選んだステージの中からランダムで決定
    if (candidateStages.size() > 1) {
        std::vector<STAGE> intersection;
        for (STAGE stage : playerChosenStages) {
            if (std::find(candidateStages.begin(), candidateStages.end(), stage) != candidateStages.end()) {
                intersection.push_back(stage);
            }
        }

        if (!intersection.empty()) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::shuffle(intersection.begin(), intersection.end(), gen);
            m_stageNumber = intersection.front();
        }
        else {
            // 念のため、候補リストからランダムに選ぶ（万が一交差が空の場合）
            std::random_device rd;
            std::mt19937 gen(rd());
            std::shuffle(candidateStages.begin(), candidateStages.end(), gen);
            m_stageNumber = candidateStages.front();
        }
    }
    else {
        m_stageNumber = candidateStages.front();  // 単独最多ならそのまま決定
    }

    // 即時にステージ遷移
    m_isFinished = true;
}