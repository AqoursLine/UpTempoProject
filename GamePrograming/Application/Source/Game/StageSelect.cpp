#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/GameSystem.h"
#include "Game/StageSelect.h"
#include "Game/Controller.h"
#include <random>
#include <vector>
#include <unordered_map>

StageSelect::StageSelect() {

    //＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
    //　ステージ関連
    m_stageNumber = STAGE_CLASSROOM;
    m_selectedStages.resize(m_totalPlayer, -1);//初期化
    

    //＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
    //　コントローラー関連
    m_totalPlayer = 1;//SaveData::GetTotalPlayer();//総プレイヤー数


    //＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
    //　背景関連
	m_backGroundTex.Load(L"Data/Texture/StageSelectBg.png");//　背景テクスチャ
    m_backGroundPos = XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);//　背景位置
    m_backGroundSize = XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT);       //　背景サイズ


    //＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
    //　カーソル関連
    m_cursorTex[0].Load(L"Data/Texture/hand1.png");//　カーソルテクスチャ1
    m_cursorTex[1].Load(L"Data/Texture/hand2.png");//　カーソルテクスチャ2
    m_cursorTex[2].Load(L"Data/Texture/hand3.png");//　カーソルテクスチャ3
    m_cursorTex[3].Load(L"Data/Texture/hand4.png");//　カーソルテクスチャ4
    m_cursorLocked.resize(m_totalPlayer, false);   //　最初はロックなし

    //コントローラーのプレイヤー分
    for (int i = 0; i < m_totalPlayer; i++) {
        m_cursorPos[i] = XMFLOAT2(SCREEN_WIDTH / 4 + (i + 1), SCREEN_HEIGHT / 2);
        m_cursorSize[i] = XMFLOAT2(200.0f, 200.0f);
        m_cursorSpeed[i] = 1000.0f;
        m_padIndex[i] = CTRL.GetGamepadHandle();
    }

    //＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
    //　変化前のボタン関連
    m_buttonTex[0].Load(L"Data/Texture/stage1.png");//　ボタンテクスチャ1
    m_buttonTex[1].Load(L"Data/Texture/stage2.png");//　ボタンテクスチャ2
    m_buttonTex[2].Load(L"Data/Texture/stage3.png");//　ボタンテクスチャ3
    m_buttonTex[3].Load(L"Data/Texture/stage4.png");//　ボタンテクスチャ4
    m_buttonPos[0] = XMFLOAT2(300.0f, 250.0f);      //　ボタン位置1
    m_buttonPos[1] = XMFLOAT2(750.0f, 750.0f);      //　ボタン位置2
    m_buttonPos[2] = XMFLOAT2(1200.0f, 250.0f);     //　ボタン位置3
    m_buttonPos[3] = XMFLOAT2(1650.0f, 750.0f);     //　ボタン位置4

    for (int i = 0; i < 4; i++)
    {
        m_buttonSize[i] = XMFLOAT2(300.0f, 300.0f); //　変化前ボタンサイズ

        for (int j = 0; j < 4; j++)
        {
            m_buttonSelected[i][j] = false;         //　初期状態は選択されていない
        }

    }

    allSelected = false;

    //＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
    //　変化後のボタン関連
    m_changebuttonTex[0].Load(L"Data/Texture/stage1(kae).png");//変化後のボタンテクスチャ1
    m_changebuttonTex[1].Load(L"Data/Texture/stage2(kae).png");//変化後のボタンテクスチャ2
    m_changebuttonTex[2].Load(L"Data/Texture/stage3(kae).png");//変化後のボタンテクスチャ3
    m_changebuttonTex[3].Load(L"Data/Texture/stage4(kae).png");//変化後のボタンテクスチャ4

    //　変化後ボタンの位置
    for (int i = 0; i < 4; i++)
    {
        m_ChangebuttonSize[i] = XMFLOAT2(400.0f, 400.0f);

    }

    //＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
    //　アニメーション関連
    m_animObjectTex.Load(L"Data/Texture/Coin.png"); //　ステージ選択用のテクスチャ
    m_animObjectPos = XMFLOAT2(m_buttonPos[m_animObjectIndex].x, m_buttonPos[m_animObjectIndex].y - 200.0f);
    m_animObjectIndex = 0;         //　最初のボタンの上
    m_animObjectTimer = 0.0f;      //　経過時間
    m_animObjectInterval = 0.25f;  //　0.25ごとに移動
    m_animTotalTime = 0.0f;        //　合計経過時間
    m_animRouletteFinished = false;//　最初はfalse
    m_animStart = false;
    m_animObjectStopped = false;   // コインが停止したかどうか
    
    
    //＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
    //　動画関連
    
    m_moviePos[0] = XMFLOAT2(300.0f, 830.0f);   //動画の位置1
    m_moviePos[1] = XMFLOAT2(750.0f, 330.0f);   //動画
    m_moviePos[2] = XMFLOAT2(1200.0f, 830.0f);
    m_moviePos[3] = XMFLOAT2(1650.0f, 330.0f);

    //動画のサイズ
    for (int i = 0; i < 4; i++)
    {
        m_movieSize[i] = XMFLOAT2(500.0f, 400.0f);
    }

    //動画の読み込み
    m_video.create("Data/Movie/ZTMY.mp4");
    m_video.setLooping(false);

    m_stageVideo1.create("Data/Movie/ZTMY2.mp4");
    m_stageVideo1.setLooping(false);

    m_stageVideo2.create("Data/Movie/ZTMY3.mp4");
    m_stageVideo2.setLooping(false);

    m_stageVideo3.create("Data/Movie/ZTMY4.mp4");
    m_stageVideo3.setLooping(false);

    m_stageVideo4.create("Data/Movie/ZTMY5.mp4");
    m_stageVideo4.setLooping(false);

    //半透明の黒テクスチャ
    m_alphaTex.Load(L"Data/Texture/black.png");
}

StageSelect::~StageSelect() {
    
    for(int i=0;i<m_totalPlayer;i++)
    { 
        CTRL.ReleaseGamepadHandle(m_padIndex[i]); // ハンドルを解放
    }

    SaveData::SetStage(m_stageNumber);

    m_video.destroy();
    m_stageVideo1.destroy();
    m_stageVideo2.destroy();
    m_stageVideo3.destroy();
    m_stageVideo4.destroy();
}

void StageSelect::Update() {

    m_video.update(GAMESYS.GetDletaTime());
    m_stageVideo1.update(GAMESYS.GetDletaTime());
    m_stageVideo2.update(GAMESYS.GetDletaTime());
    m_stageVideo3.update(GAMESYS.GetDletaTime());
    m_stageVideo4.update(GAMESYS.GetDletaTime());

    if (m_animStart)
    {
        FinalStageAnim();
        
    }
    

    if (!allSelected)
    {
        for (int i = 0; i < m_totalPlayer; i++)
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
                            m_selectedStages[i] = j;

                            //選んだステージによって動画
                            if (m_selectedStages[i] == 0)
                            {
                                m_stageVideo1.resume();
                            }

                            if (m_selectedStages[i] == 1)
                            {
                                m_stageVideo2.resume();
                            }

                            if (m_selectedStages[i] == 2)
                            {
                                m_stageVideo3.resume();
                            }

                            if (m_selectedStages[i] == 3)
                            {
                                m_stageVideo4.resume();
                            }


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
                        m_selectedStages[i] = j; // 選択したボタンのステージを記録
                        m_cursorLocked[i] = true; // 選択確定
                        break;
                    }
                }

            }

            // ×ボタンでロック解除（キャンセル）
            if (m_cursorLocked[i] &&
                CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CROSS, m_padIndex[i])) {
                m_cursorLocked[i] = false;
                m_selectedStages[i] = -1; // 選択をリセット

            }

        }
    }
    
    
    // すべてのプレイヤーが選択したか確認
    allSelected = true;
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


    D3D.Draw2D(m_backGroundTex, m_backGroundPos, m_backGroundSize);

    if (!m_animRouletteFinished)
    {
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
                D3D.Draw2D(m_changebuttonTex[i], m_buttonPos[i], m_ChangebuttonSize[i]);
            }
            else {
                D3D.Draw2D(m_buttonTex[i], m_buttonPos[i], m_buttonSize[i]);
            }

        }

        for (int i = 0; i < m_totalPlayer; i++) {
            D3D.Draw2D(m_cursorTex[i], m_cursorPos[i], XMFLOAT2(200.0f, 200.0f));

            for (int j = 0; j < 4; j++)
            {
                if (m_buttonSelected[i][j] == true)
                {
                    //選んだステージによって動画
                    if (m_selectedStages[i] == 0)
                    {
                        D3D.Draw2D(m_stageVideo1.getTexture()->shader_resource_view, m_moviePos[0], m_movieSize[0], PIXELMODE_MOVIE);
                    }

                    if (m_selectedStages[i] == 1)
                    {
                        D3D.Draw2D(m_stageVideo2.getTexture()->shader_resource_view, m_moviePos[1], m_movieSize[1], PIXELMODE_MOVIE);
                    }

                    if (m_selectedStages[i] == 2)
                    {
                        D3D.Draw2D(m_stageVideo3.getTexture()->shader_resource_view, m_moviePos[2], m_movieSize[2], PIXELMODE_MOVIE);
                    }

                    if (m_selectedStages[i] == 3)
                    {
                        D3D.Draw2D(m_stageVideo4.getTexture()->shader_resource_view, m_moviePos[3], m_movieSize[3], PIXELMODE_MOVIE);
                    }

                }


            }


            D3D.Draw2D(m_cursorTex[i], m_cursorPos[i], m_cursorSize[i]);

        }


        if (m_animStart)
        {
            D3D.Draw2D(m_animObjectTex, m_animObjectPos, XMFLOAT2(100.0f, 100.0f));
        }
    }
    else
    {
        //半透明テクスチャ
        D3D.Draw2D(m_alphaTex, m_backGroundPos, m_backGroundSize, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.9f), PIXELMODE_DEFAULT);
    }

   
}

 //ステージ決定処理
void StageSelect::DetermineFinalStage() {

   
    std::unordered_map<STAGE, int> stageCount;
    int maxCount = 0;

    // 各プレイヤーの選択をカウント
    for (int i = 0; i < m_totalPlayer; i++) {
        if (m_selectedStages[i] != -1) {
            STAGE stage = static_cast<STAGE>(m_selectedStages[i]);
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
        if (m_selectedStages[i] != -1) {
            playerChosenStages.push_back(static_cast<STAGE>(m_selectedStages[i]));
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


    m_animStart = true;
}

void StageSelect::FinalStageAnim()
{
    float deltaTime = GAMESYS.GetDletaTime();
    m_animTotalTime += deltaTime; // 全体の経過時間をカウント

  
   
    if (!m_animObjectStopped) { // まだ停止していない場合
        if (m_animTotalTime >= 10.0f) {
            // 10秒経過後、ステージ番号に対応するボタンに移動
            m_animObjectIndex = static_cast<int>(m_stageNumber);
            m_animObjectStopped = true; // 停止フラグをON
            m_animRouletteFinished = true;

        }
        else {
            // 10秒未満なら2秒ごとに次のボタンへ移動
            m_animObjectTimer += deltaTime;
            if (m_animObjectTimer >= m_animObjectInterval) {
                m_animObjectTimer = 0.0f;
                m_animObjectIndex = (m_animObjectIndex + 1) % 4; // 4つのボタンをループ
            }
        }
    }

    // スムーズな移動アニメーション
    XMFLOAT2 targetPos = XMFLOAT2(m_buttonPos[m_animObjectIndex].x, m_buttonPos[m_animObjectIndex].y - 200.0f);
    m_animObjectPos.x += (targetPos.x - m_animObjectPos.x) * 0.3f;
    m_animObjectPos.y += (targetPos.y - m_animObjectPos.y) * 0.3f;
}

