/******************************************************
* HitStop.h	ヒットストップ
* 制作者：カワマタトウ
* 作成日：2024/12/2
* 最終更新日：2024/12/3
*******************************************************/
#pragma once

#include <box2d/box2d.h>

class HitStop
{
public:
	HitStop();
	~HitStop();

	void SetisHitStop(bool flag);
	void isHitStop(b2Body* body);

private:
	//ヒットストップフラグ
	bool m_isHitStop = false;

	// ヒットストップカウント
	int m_HitStopCount = 0;
};