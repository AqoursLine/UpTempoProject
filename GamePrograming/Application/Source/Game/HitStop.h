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

	void SetIsHitStop(bool flag,int totalframe);
	bool IsHitStop(b2Body* body);

	bool GetIsHitStop() const { return m_isHitStop; }

private:
	//ヒットストップフラグ
	bool m_isHitStop = true;

	// ヒットストップカウント
	int m_HitStopCount = 0;

	//総フレーム数
	int m_TotalFrame = 0;

};