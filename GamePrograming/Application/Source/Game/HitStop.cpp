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

void HitStop::SetisHitStop(bool flag)
{
	m_isHitStop = flag;
}

void HitStop::isHitStop(b2Body* m_body)
{
	// フィルタ取得
	b2Filter filter = m_body->GetFixtureList()->GetFilterData();

	if (m_isHitStop && m_HitStopCount < 100)
	{
		filter.maskBits = 0;
		m_body->GetFixtureList()->SetFilterData(filter);

		// ボディタイプを静的に設定
		m_body->SetType(b2_staticBody);


		m_HitStopCount++;
		return;
	}
	
	m_HitStopCount = 0;
	SetisHitStop(false);
	filter.maskBits = ~0;
	m_body->GetFixtureList()->SetFilterData(filter);
		
	// ボディタイプを動的に設定
	m_body->SetType(b2_dynamicBody);
	
}




