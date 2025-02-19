/******************************************************
* ChooseScene.h		?I??V?[???Ǘ?
* ?????F?~???^?W???E?W
* ?쐬???F2025/1/16
* ?ŏI?X?V???F2025/1/16
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
	//?I?????
	int m_totalPlayer = 0;
	STAGE m_stageNum;

	//?I??
	Select* m_select = nullptr;

	//?X?e?[?g
	CHOOSESTATE m_state;

	//?X?e?[?g?֐?
	void Character();
	void Stage();
	void Start();
	void Finish();
};

