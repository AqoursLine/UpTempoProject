/******************************************************
* FieldManager.h	フィールド管理
* 制作者：ミヤタジョウジ
* 作成日：2024/11/12
* 最終更新日：2024/11/12
*******************************************************/
#pragma once

#include "Game/FieldObject.h"

class FieldManager {
public:
	FieldManager();
	~FieldManager();

	void Update();
	void Draw();

	void ReCreateField(float sizeTop, float sizeLeft, float sizeRight, float sizeButtom);

private:
	std::list<FieldObject*> m_fieldObjects;
	int soundNum;	//外枠が壊れる音

};


