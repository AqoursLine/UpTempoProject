/******************************************************
* CharacterSelect.h		キャラクター選択
* 制作者：ミヤタジョウジ
* 作成日：2025/1/16
* 最終更新日：2025/1/16
*******************************************************/
#pragma once
#include "Game/Select.h"

class CharacterSelect : public Select {
public:
	CharacterSelect();
	~CharacterSelect();

	void Update() override;
	void Draw() override;

private:

};

