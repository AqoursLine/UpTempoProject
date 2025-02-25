#include "framework.h"
#include "DirectX/DirectX.h"
#include "DirectX/Audio.h"
#include "Game/GameSystem.h"
#include "Game/StageSelect.h"
#include "Game/Controller.h"
#include <random>
#include <vector>
#include <unordered_map>


//　ステージセレクト初期化
StageSelect::StageSelect()
	:	m_animVideo(L"Data/Movie/キラキラ.avi"),//　きらきら
		m_animVideo2(L"Data/Movie/Box.avi")	//　箱アニメーション
{

    //＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
    //　ステート管理
    m_state = StageSelectState::SELECTION;  //　ステージ選択状態
    

    //＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
    //　コントローラー関連の初期化
    m_stageNumber = STAGE_CLASSROOM;
	m_totalPlayer = 1;//SaveData::GetTotalPlayer();//総プレイヤー数
    m_selectedStages.resize(m_totalPlayer, -1);//初期化


    //＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
    //　背景関連の初期化
    m_backGroundTex.Load(L"Data/Texture/StageSelectBg.png");//　背景テクスチャ
    m_backGroundPos = XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);//　背景位置
    m_backGroundSize = XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT);       //　背景サイズ


    //＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
    //　カーソル関連の初期化
    m_cursorTex[0].Load(L"Data/Texture/hand1.png");//　カーソルテクスチャ1
    m_cursorTex[1].Load(L"Data/Texture/hand2.png");//　カーソルテクスチャ2
    m_cursorTex[2].Load(L"Data/Texture/hand3.png");//　カーソルテクスチャ3
    m_cursorTex[3].Load(L"Data/Texture/hand4.png");//　カーソルテクスチャ4
    m_cursorLocked.resize(m_totalPlayer, false);   //　最初はロックなし

    //コントローラーのプレイヤー分
    for (int i = 0; i < m_totalPlayer; i++) {
        m_cursorPos[i] = XMFLOAT2(static_cast<float>(SCREEN_WIDTH / 4 + (i + 1)), SCREEN_HEIGHT / 2);//　カーソルの位置
        m_cursorSize[i] = XMFLOAT2(200.0f, 200.0f);//　カーソルのサイズ
        m_cursorSpeed[i] = 1000.0f;                //　カーソルの速度
        m_padIndex[i] = SaveData::GetPlayerData(i + 1).PadNum;   //　パッド取得
    }


    //＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
    //　変化前のボタン関連の初期化
    m_buttonTex[0].Load(L"Data/Texture/stage1.png");//　ボタンテクスチャ1
    m_buttonTex[1].Load(L"Data/Texture/stage2.png");//　ボタンテクスチャ2
    m_buttonTex[2].Load(L"Data/Texture/stage3.png");//　ボタンテクスチャ3
    m_buttonTex[3].Load(L"Data/Texture/stage4.png");//　ボタンテクスチャ4
    m_buttonPos[0] = XMFLOAT2(300.0f, 250.0f);      //　ボタン位置1
    m_buttonPos[1] = XMFLOAT2(750.0f, 750.0f);      //　ボタン位置2
    m_buttonPos[2] = XMFLOAT2(1200.0f, 250.0f);     //　ボタン位置3
    m_buttonPos[3] = XMFLOAT2(1650.0f, 750.0f);     //　ボタン位置4

    //変化前ボタンの配列初期化
    for (int i = 0; i < 4; i++)
    {
        m_buttonSize[i] = XMFLOAT2(300.0f, 300.0f); //　変化前ボタンサイズ

        for (int j = 0; j < 4; j++)
        {
            m_buttonSelected[i][j] = false;         //　初期状態は選択されていない
        }

    }

    allSelected = false;    //　すべてが選択されているか？（はじめはfalse)


    //＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
    //　変化後のボタン関連
    m_changebuttonTex[0].Load(L"Data/Texture/stage1(kae).png");//　変化後のボタンテクスチャ1
    m_changebuttonTex[1].Load(L"Data/Texture/stage2(kae).png");//　変化後のボタンテクスチャ2
    m_changebuttonTex[2].Load(L"Data/Texture/stage3(kae).png");//　変化後のボタンテクスチャ3
    m_changebuttonTex[3].Load(L"Data/Texture/stage4(kae).png");//　変化後のボタンテクスチャ4

    for (int i = 0; i < 4; i++)
    {
        m_ChangebuttonSize[i] = XMFLOAT2(400.0f, 400.0f);   //　変化前ボタンサイズ

    }

    m_animObjectIndex = 0;

    //＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
    //　アニメーション関連
    m_animObjectTex.Load(L"Data/Texture/Coin.png"); //　ステージ選択用のテクスチャ
    m_animObjectPos = XMFLOAT2(m_buttonPos[m_animObjectIndex].x, m_buttonPos[m_animObjectIndex].y - 200.0f);
    m_animObjectIndex = 0;         //　最初のボタンの上
    m_animObjectTimer = 0.0f;      //　経過時間
    m_animObjectInterval = 0.25f;  //　0.25ごとに移動
    m_animFirstStageTime = 0.0f;   //　はじめのアニメーション
    m_animFinalStageTime = 0.0f;   //　最後のアニメーション
    m_animRouletteFinished = false;

    //＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
    //　動画関連
    m_moviePos[0] = XMFLOAT2(300.0f, 830.0f);   //　動画の位置1
    m_moviePos[1] = XMFLOAT2(750.0f, 330.0f);   //　動画の位置2
    m_moviePos[2] = XMFLOAT2(1200.0f, 830.0f);  //　動画の位置3
    m_moviePos[3] = XMFLOAT2(1650.0f, 330.0f);  //　動画の位置4
    m_lastmoviePos = XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100);
    m_lastmovieSize = XMFLOAT2(1000.0f, 500.0f);
	m_moveSize = XMFLOAT2(1920 * 0.01f, 1080 * 0.01f);

    for (int i = 0; i < 4; i++)
    {
        m_movieSize[i] = XMFLOAT2(500.0f, 400.0f);  //　動画のサイズ
    }

    m_stageVideo1.create("Data/Movie/ZTMY2.mp4");   //　ステージ1
    m_stageVideo2.create("Data/Movie/ZTMY3.mp4");   //　ステージ2
    m_stageVideo3.create("Data/Movie/ZTMY4.mp4");   //　ステージ3
    m_stageVideo4.create("Data/Movie/ZTMY5.mp4");   //　ステージ4

    m_stageVideo1.setLooping(false);                //　ステージ1ループ設定
    m_stageVideo2.setLooping(false);                //　ステージ2ループ設定
    m_stageVideo3.setLooping(false);                //　ステージ3ループ設定
    m_stageVideo4.setLooping(false);                //　ステージ4ループ設定
	
	m_animVideo.SetIsAutoLoop(true);				//　きらきらループ設定
	m_animVideo2.SetIsAutoLoop(false);				//　箱アニメーションループ設定
	
    m_alphaTex.Load(L"Data/Texture/black.png");     //　半透明の黒テクスチャ

	//BGM読み込み
	m_soundNum = AUDIO.LoadWaveFile("Data/Sound/BGM/ポップス5.wav");
	m_decisionSound=AUDIO.LoadWaveFile("Data/Sound/SE/決定10.wav");
	m_cancelSound = AUDIO.LoadWaveFile("Data/Sound/SE/キャンセル5.wav");
	m_fallBoxSound = AUDIO.LoadWaveFile("Data/Sound/SE/落下3.wav");
	m_openBoxSound = AUDIO.LoadWaveFile("Data/Sound/SE/ロールの締め.wav");

	//BGM再生
	AUDIO.PlayAudio(m_soundNum, 0);
	
	//サウンド音量
	AUDIO.SetVolume(m_soundNum, 0.5f);
	AUDIO.SetVolume(m_decisionSound, 0.5f);
	AUDIO.SetVolume(m_cancelSound, 0.5f);
	AUDIO.SetVolume(m_fallBoxSound, 0.5f);
	AUDIO.SetVolume(m_openBoxSound, 0.5f);

	m_fallBoxSoundPlayed = false;
	m_openBoxSoundPlayed = false;
}

//　ステージセレクト終了処理
StageSelect::~StageSelect() {

    //　動画の解放
    m_stageVideo1.destroy();
    m_stageVideo2.destroy();
    m_stageVideo3.destroy();
    m_stageVideo4.destroy();

    // ハンドルを解放
    for (int i = 0; i < m_totalPlayer; i++)
    {
        CTRL.ReleaseGamepadHandle(m_padIndex[i]);
    }

    // 最終的に決定したステージをセット
    SaveData::SetStage(m_stageNumber);

	//サウンドの停止
	AUDIO.StopAudio(m_soundNum);

}

// ステージセレクト更新処理
void StageSelect::Update() {

    {
        // とりあえずエンターキーを押したら終了
        if (CTRL.GetKeyboardTrigger(DIK_1))
        {
            m_stageNumber = STAGE_CLASSROOM;
            m_isFinished = true;
        }
        else if (CTRL.GetKeyboardTrigger(DIK_2))
        {
            m_stageNumber = STAGE_OCEAN;
            m_isFinished = true;
        }
        else if (CTRL.GetKeyboardTrigger(DIK_3))
        {
            m_stageNumber = STAGE_GAME;
            m_isFinished = true;
        }
        else if (CTRL.GetKeyboardTrigger(DIK_4))
        {
            m_stageNumber = STAGE_PARK;
            m_isFinished = true;
        }
    }
    
    switch (m_state) {
    case StageSelectState::SELECTION:
        Select();
        break;

    case StageSelectState::ANIMATION:
        FirstStageAnim();
        break;

    case StageSelectState::INTRO_ANIMATION:
        FinalStageAnim();
        break;
    }

}

// ステージセレクト描画処理
void StageSelect::Draw() {

    // 背景描画
    D3D.Draw2D(m_backGroundTex, m_backGroundPos, m_backGroundSize);

    // ステートごとの描画
    switch (m_state)
    {
    case StageSelectState::SELECTION:
        // ４つ分
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
        // プレイヤー分
        for (int i = 0; i < m_totalPlayer; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                // ボタンが選択状態なら
                if (m_buttonSelected[i][j] == true)
                {
                    // 選んだステージによって動画
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

            // プレイヤー分のカーソル描画
            D3D.Draw2D(m_cursorTex[i], m_cursorPos[i], m_cursorSize[i]);

        }
        break;

    case StageSelectState::ANIMATION:

        // ボタンの描画
        for (int i = 0; i < 4; i++)
        {
            D3D.Draw2D(m_buttonTex[i], m_buttonPos[i], m_buttonSize[i]);

        }
        
        // ルーレット用オブジェクトの描画
        D3D.Draw2D(m_animObjectTex, m_animObjectPos, XMFLOAT2(100.0f, 100.0f));

        break;


    case StageSelectState::INTRO_ANIMATION:

        // 半透明テクスチャ
        D3D.Draw2D(m_alphaTex, m_backGroundPos, m_backGroundSize, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 0.9f), PIXELMODE_DEFAULT);

		// 箱アニメーション
		D3D.Draw2D(m_animVideo2.GetSRV(), XMFLOAT2(SCREEN_WIDTH/2, SCREEN_HEIGHT / 2+200), XMFLOAT2(SCREEN_WIDTH,SCREEN_HEIGHT), PIXELMODE_DEFAULT);

        // 3秒たったら
        if (m_animFinalStageTime >= 2.0f)
        {
            
            // 選ばれたステージによって動画
            if (m_stageNumber == STAGE_CLASSROOM)
            {
                D3D.Draw2D(m_stageVideo1.getTexture()->shader_resource_view, m_lastmoviePos, m_moveSize, PIXELMODE_MOVIE);
            }

            if (m_stageNumber == STAGE_OCEAN)
            {
                D3D.Draw2D(m_stageVideo2.getTexture()->shader_resource_view, m_lastmoviePos, m_moveSize, PIXELMODE_MOVIE);
            }

            if (m_stageNumber == STAGE_GAME)
            {
                D3D.Draw2D(m_stageVideo3.getTexture()->shader_resource_view, m_lastmoviePos, m_moveSize, PIXELMODE_MOVIE);
            }

            if (m_stageNumber == STAGE_PARK)
            {
                D3D.Draw2D(m_stageVideo4.getTexture()->shader_resource_view, m_lastmoviePos, m_moveSize, PIXELMODE_MOVIE);
            }

			// きらきらのアニメーション
			D3D.Draw2D(m_animVideo.GetSRV(), m_lastmoviePos, m_moveSize, PIXELMODE_DEFAULT);
        }

		if (m_animFinalStageTime >= 12.0f)
		{
			// トランジションを入れるときはココにステート移行書く
			m_isFinished = true;
		}

        break;
    default:
        break;
    }
        
}

// ステージ選択
void StageSelect::Select()
{
	m_stageVideo1.update(GAMESYS.GetDletaTime());
	m_stageVideo2.update(GAMESYS.GetDletaTime());
	m_stageVideo3.update(GAMESYS.GetDletaTime());
	m_stageVideo4.update(GAMESYS.GetDletaTime());

    // すべてのプレイヤーが選択し終わってなければ
    if (!allSelected)
    {
        for (int i = 0; i < m_totalPlayer; i++)
        {
            // カーソルがロックされていない場合のみ移動を許可
            if (!m_cursorLocked[i])
            {
                // 左スティックの移動量を取得
                float deltaX = (CTRL.GetLeftStickHorizontal(m_padIndex[i])) / 32767.0f;
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

                    // カーソルがボタンの範囲内なら
                    if (std::abs(m_cursorPos[i].x - m_buttonPos[j].x) < buttonHalfSize &&
                        std::abs(m_cursorPos[i].y - m_buttonPos[j].y) < buttonHalfSize)
                    {
                        m_buttonSelected[i][j] = true; // ボタンが選択状態
                        m_selectedStages[i] = j;       // 選択したステージ番号を格納

                        // 選んだステージによって動画
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

                    // 範囲外なら
                    else
                    {
                        m_buttonSelected[i][j] = false; // ボタンから離れると元に戻る

                    }
                }
            }

            // 〇ボタンで選択を確定（カーソルをロック）
            if (!m_cursorLocked[i] &&
                CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CIRCLE, m_padIndex[i]))
            {
                for (int j = 0; j < 4; j++) {
                    if (m_buttonSelected[i][j]) {

						AUDIO.PlayAudio(m_decisionSound, 0);
                        m_selectedStages[i] = j;    // 選択したボタンのステージを記録
                        m_cursorLocked[i] = true;   // 選択確定
                        break;
                    }
                }

            }

            // ×ボタンでロック解除（キャンセル）
            if (m_cursorLocked[i] &&
                CTRL.GetGamepadButtonTrigger(GAMEPAD_BUTTON_PS4_CROSS, m_padIndex[i]))
            {
				AUDIO.PlayAudio(m_cancelSound, 0);
                m_cursorLocked[i] = false;  // カーソルロックの解除
                m_selectedStages[i] = -1;   // ステージ選択をリセット
            }

        }
    }

    // すべてのプレイヤーが選択したか確認
    allSelected = true;
    for (int i = 0; i < m_totalPlayer; i++)
    {
        // カーソルが一つでもロックされていなければ
        if (!m_cursorLocked[i])
        {
            // 全部選択をfalseに
            allSelected = false;
            break;
        }

    }

    // 全部選択されたら
    if (allSelected)
    {
        // ここでステージを決める
        DetermineFinalStage();

        // ルーレットステートへ
        m_state = StageSelectState::ANIMATION;
    }
}

// ステージ決定計算
void StageSelect::DetermineFinalStage()
{


    std::unordered_map<STAGE, int> stageCount;
    int maxCount = 0;

    // 各プレイヤーの選択をカウント
    for (int i = 0; i < m_totalPlayer; i++)
    {
        if (m_selectedStages[i] != -1)
        {
            STAGE stage = static_cast<STAGE>(m_selectedStages[i]);
            stageCount[stage]++;
            if (stageCount[stage] > maxCount)
            {
                maxCount = stageCount[stage];
            }
        }
    }

    // 最大票数のステージをリストアップ
    std::vector<STAGE> candidateStages;
    for (const auto& entry : stageCount)
    {
        if (entry.second == maxCount)
        {
            candidateStages.push_back(entry.first);
        }
    }

    // プレイヤーが選択したステージのリストを作成
    std::vector<STAGE> playerChosenStages;
    for (int i = 0; i < m_totalPlayer; i++)
    {
        if (m_selectedStages[i] != -1)
        {
            playerChosenStages.push_back(static_cast<STAGE>(m_selectedStages[i]));
        }
    }

    // 同率ならプレイヤーが選んだステージの中からランダムで決定
    if (candidateStages.size() > 1)
    {
        std::vector<STAGE> intersection;
        for (STAGE stage : playerChosenStages)
        {
            if (std::find(candidateStages.begin(), candidateStages.end(), stage) != candidateStages.end())
            {
                intersection.push_back(stage);
            }
        }

        if (!intersection.empty())
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::shuffle(intersection.begin(), intersection.end(), gen);
            m_stageNumber = intersection.front();
        }
        else
        {
            // 念のため、候補リストからランダムに選ぶ（万が一交差が空の場合）
            std::random_device rd;
            std::mt19937 gen(rd());
            std::shuffle(candidateStages.begin(), candidateStages.end(), gen);
            m_stageNumber = candidateStages.front();
        }
    }
    else
    {
        m_stageNumber = candidateStages.front();  //　単独最多ならそのまま決定
    }

}

// ルーレットアニメーション
void StageSelect::FirstStageAnim()
{
    float deltaTime = GAMESYS.GetDletaTime();
    // 全体の経過時間をカウント
    m_animFirstStageTime += deltaTime;

    if (!m_animRouletteFinished)
    {
        // 10秒経過したら
        if (m_animFirstStageTime >= 10.0f)
        {
            // ステージ番号に対応するボタンに移動
            m_animObjectIndex = static_cast<int>(m_stageNumber);
            m_animRouletteFinished = true;
        }

        else
        {
            // 10秒未満なら2秒ごとに次のボタンへ移動
            m_animObjectTimer += deltaTime;
            if (m_animObjectTimer >= m_animObjectInterval) 
            {
                m_animObjectTimer = 0.0f;
                m_animObjectIndex = (m_animObjectIndex + 1) % 4; //　4つのボタンをループ
            }
        }
    }
    
    // スムーズな移動アニメーション
    XMFLOAT2 targetPos = XMFLOAT2(m_buttonPos[m_animObjectIndex].x, m_buttonPos[m_animObjectIndex].y - 200.0f);
    m_animObjectPos.x += (targetPos.x - m_animObjectPos.x) * 0.3f;
    m_animObjectPos.y += (targetPos.y - m_animObjectPos.y) * 0.3f;

    if (m_animFirstStageTime >= 12.0f)
    {
        m_state = StageSelectState::INTRO_ANIMATION;
    }

}

// ステージの映像出るアニメーション
void StageSelect::FinalStageAnim()
{
	m_stageVideo1.update(GAMESYS.GetDletaTime());
	m_stageVideo2.update(GAMESYS.GetDletaTime());
	m_stageVideo3.update(GAMESYS.GetDletaTime());
	m_stageVideo4.update(GAMESYS.GetDletaTime());
	m_animVideo.Update(GAMESYS.GetDletaTime());
	m_animVideo2.Update(GAMESYS.GetDletaTime());

    float deltaTime = GAMESYS.GetDletaTime();

    // 全体の経過時間をカウント
    m_animFinalStageTime += deltaTime;

	if (!m_fallBoxSoundPlayed) {
		AUDIO.PlayAudio(m_fallBoxSound, 0);
		m_fallBoxSoundPlayed = true;
	}

	
	if (m_animFinalStageTime >= 1.0f)
	{
		if (!m_openBoxSoundPlayed) {
			AUDIO.PlayAudio(m_openBoxSound, 0);
			m_openBoxSoundPlayed = true;
		}

	}

	if (m_lastmovieSize.x > m_moveSize.x)
	{
		m_moveSize.x *= 1.02f;
		m_moveSize.y *= 1.02f;
	}
	
    if (m_animFinalStageTime >= 2.0f)
    {
		
		AUDIO.StopAudio(m_openBoxSound);

        // 選ばれたステージによって動画
        if (m_stageNumber==STAGE_CLASSROOM)
        {
            m_stageVideo1.resume();
        }

        if (m_stageNumber==STAGE_OCEAN)
        {
            m_stageVideo2.resume();
        }

        if (m_stageNumber==STAGE_GAME)
        {
            m_stageVideo3.resume();
        }

        if (m_stageNumber==STAGE_PARK)
        {
            m_stageVideo4.resume();
        }
    }

	if (m_animFinalStageTime >= 3.0f)
	{
		AUDIO.StopAudio(m_fallBoxSound);
	}
}



