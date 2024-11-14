/******************************************************
* Ground.h	地面
* 制作者：ミヤタジョウジ
* 作成日：2024/11/07
* 最終更新日：2024/11/07
*******************************************************/
#pragma once

#include "Game/FieldObject.h"

/****************************************************
* 地面クラス
*****************************************************/
class Ground : public FieldObject {
public:
	Ground() = delete;
	Ground(Field* field, XMFLOAT2 pos, float rot, XMFLOAT2 size);
	~Ground();

	void Update() override;
	void Draw() override;

private:
	Texture m_tex;
};
