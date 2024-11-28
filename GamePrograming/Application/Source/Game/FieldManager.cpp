/******************************************************
* FieldManager.cpp	フィールド管理
* 制作者：ミヤタジョウジ
* 作成日：2024/11/12
* 最終更新日：2024/11/12
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"
#include "Game/FieldManager.h"
#include "Game/Ground.h"

/****************************************************
* フィールド管理初期化
*****************************************************/
FieldManager::FieldManager() {
	m_filed = new Field();

	m_ground = new Ground(m_filed, XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - 100.0f), 0.0f, XMFLOAT2(1920.0f, 10.0f));
	m_wallLeft = new FieldObject(m_filed, XMFLOAT2(100.0f, SCREEN_HEIGHT * 0.5f), 0.0f, XMFLOAT2(10.0f, 1080.0f));
	m_wallRight = new FieldObject(m_filed, XMFLOAT2(SCREEN_WIDTH - 100.0f, SCREEN_HEIGHT * 0.5f), 0.0f, XMFLOAT2(10.0f, 1080.0f));
	m_ceiling = new FieldObject(m_filed, XMFLOAT2(SCREEN_WIDTH * 0.5f, 100.0f), 0.0f, XMFLOAT2(1920.0f, 10.0f));
}

/****************************************************
* フィールド管理終了
*****************************************************/
FieldManager::~FieldManager() {
	if (m_filed) delete m_filed;
	if (m_ground) delete m_ground;
	if (m_wallLeft) delete m_wallLeft;
	if (m_wallRight) delete m_wallRight;
	if (m_ceiling) delete m_ceiling;
}

/****************************************************
* フィールド管理更新
*****************************************************/
void FieldManager::Update() {
	m_filed->Update();
	m_ground->Update();
	m_wallLeft->Update();
	m_wallRight->Update();
	m_ceiling->Update();
}

/****************************************************
* フィールド管理描画
*****************************************************/
void FieldManager::Draw() {
	m_filed->Draw();
	m_ground->Draw();
	m_wallLeft->Draw();
	m_wallRight->Draw();
	m_ceiling->Draw();
}


