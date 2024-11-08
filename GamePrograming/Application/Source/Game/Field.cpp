/******************************************************
* Field.cpp	フィールド
* 制作者：ミヤタジョウジ
* 作成日：2024/11/05
* 最終更新日：2024/11/06
*******************************************************/
#include "framework.h"
#include "DirectX/DirectX.h"
#include "Game/Physics.h"

#include "Game/Field.h"

/****************************************************
* フィールド初期化
*****************************************************/
Field::Field() {
	m_ground = new Ground();
	m_fieldObject = new FieldObject(FIELDOBJECTTYPE_NORMAL, SCREEN_WIDTH * 0.5f - 300.0f, SCREEN_HEIGHT * 0.5f + 200.0f, 100.0f, 100.0f, 0.0f);
}

/****************************************************
* フィールド終了
*****************************************************/
Field::~Field() {
	if (m_ground) delete m_ground;
	if (m_fieldObject) delete m_fieldObject;
}

/****************************************************
* フィールド更新
*****************************************************/
void Field::Update() {
	m_ground->Update();
	m_fieldObject->Update();
}

/****************************************************
* フィールド描画
*****************************************************/
void Field::Draw() {
	m_ground->Draw();
	m_fieldObject->Draw();
}
