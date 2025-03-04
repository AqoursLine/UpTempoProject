#include "framework.h"
#include "PhaseGame.h"
#include "Game/StageObjectManager.h"

PhaseGame::PhaseGame(const int phaseNum) : Phase(phaseNum, 0.0f, 9.8f)
{
	m_texture.Load(L"Data/Texture/game2_big.png");


/****************************************************
* このフェーズに登場するモノの登録
*****************************************************/

	//// 恒常オブジェクト
	m_throwObjectManager->PushLotteryObject(WOODENBOX);
	m_throwObjectManager->PushLotteryObject(BEAR);
	m_throwObjectManager->PushLotteryObject(KOKESHI);

	// フェーズ限定オブジェクト
	m_throwObjectManager->PushLotteryObject(APPLE);
	m_throwObjectManager->PushLotteryObject(SWORD);
	m_throwObjectManager->PushLotteryObject(SHIELD);
	m_throwObjectManager->PushLotteryObject(COIN);
	m_throwObjectManager->PushLotteryObject(SLIME);
	m_throwObjectManager->PushLotteryObject(MOON);
	m_throwObjectManager->PushLotteryObject(BOARD);
	//m_throwObjectManager->PushLotteryObject(SCAFFOLD);//普通に出現して機能するようにしてない、必要ならやる
	m_throwObjectManager->PushLotteryObject(CLOUD);
	m_throwObjectManager->PushLotteryObject(GRASS);

	//あしば
	// 足場(SCAFFOLD)の設定はrepoptimeとspare,spare2のみ意味があるspareは自分がベースの位置から
	// 何個ずらした位置にいるかなので基本的に同じベースの足場なら前の足場の次の番号を入力すればいい（０～）
	// 基本何個設定しても問題ない
	// spare2は自分がどのベースと同じ動きをするかを指定するので、AddStageObjectで設定したベースが
	// 何番目に設定されたかを入力する（０～）
	//
	//ベース
	// S_SCAFFOLDBASEの設定は足場の初期位置に影響する　spareには動きのパターン番号を入力する
	// 今は１が画面の右側を左右に動き　２が左側を左右に動く
	// もっと多くのパターンが必要になったら巡回するポジションを指定できるようにする
	//

	m_stageObjectManager->AddStageObject(S_SCAFFOLDBASE, SCREEN_WIDTH * 0.2f, 600.0f, 0.0f, 600, 1);
	m_stageObjectManager->AddStageObject(S_SCAFFOLD, 700.0f, 800.0f, 0.0f, 600, 0, 0);
	m_stageObjectManager->AddStageObject(S_SCAFFOLD, 700.0f, 800.0f, 0.0f, 600, 1, 0);
	m_stageObjectManager->AddStageObject(S_SCAFFOLD, 700.0f, 800.0f, 0.0f, 600, 2, 0);
	m_stageObjectManager->AddStageObject(S_SCAFFOLDBASE, SCREEN_WIDTH * 0.7f, 500.0f, 0.0f, 600, 2);
	m_stageObjectManager->AddStageObject(S_SCAFFOLD, 700.0f, 800.0f, 0.0f, 600, 0, 4);
	m_stageObjectManager->AddStageObject(S_SCAFFOLD, 700.0f, 800.0f, 0.0f, 600, 1, 4);
	m_stageObjectManager->AddStageObject(S_SCAFFOLD, 700.0f, 800.0f, 0.0f, 600, 2, 4);


	m_stageObjectManager->Initialize();
}


