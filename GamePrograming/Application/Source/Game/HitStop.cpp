/******************************************************
* HitStop.cpp	ヒットストップ
* 制作者：カワマタトウ
* 作成日：2024/12/2
* 最終更新日：2024/12/3
*******************************************************/
#include "HitStop.h"

HitStop::HitStop()
{
}

HitStop::~HitStop()
{
}

void HitStop::SetIsHitStop(bool flag,int totalframe)
{
	m_isHitStop = flag;
	m_TotalFrame = totalframe;
}

bool HitStop::IsHitStop(b2Body* m_body)
{
	//ヒットストップするか
	bool isHitStop = false;

	//フィクスチャ全てのフィルタを設定
	b2Fixture* fixture = m_body->GetFixtureList();
	while (fixture) {
		// フィルタ取得
		b2Filter filter = fixture->GetFilterData();

		if (m_isHitStop && m_HitStopCount < m_TotalFrame) {
			filter.maskBits = 0;
			fixture->SetFilterData(filter);

			// ボディタイプを静的に設定
			m_body->SetType(b2_staticBody);
			isHitStop = true;
		} else {
			m_HitStopCount = 0;
			SetIsHitStop(false, 0);
			filter.maskBits = ~0;
			fixture->SetFilterData(filter);
			isHitStop = false;
		}

		fixture = fixture->GetNext();
	}

	m_HitStopCount++;

		
	// ボディタイプを動的に設定
	m_body->SetType(b2_dynamicBody);
	return isHitStop;
	
}






