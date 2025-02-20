/******************************************************
* ChooseScene.h		選択シーン管理
* 制作者：ミヤタジョウジ
* 作成日：2025/1/16
* 最終更新日：2025/1/16
*******************************************************/
#pragma once
#include "Game/Scene.h"
#include "Game/Select.h"

enum CHOOSESTATE {
	CHOOSE_CHARACTER = 0,
	CHOOSE_STAGE,
	CHOOSE_START,
	CHOOSE_FINISH,
};

class ChooseScene : public Scene {
public:
	ChooseScene();
	~ChooseScene();

	void Update();
	void Draw();

private:
	//選択
	Select* m_select = nullptr;

	//ステート
	CHOOSESTATE m_state;

	//ステート関数
	void Character();
	void Stage();
	void Start();
	void Finish();
};



