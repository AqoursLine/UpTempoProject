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
	// フィルタ取得
	b2Filter filter = m_body->GetFixtureList()->GetFilterData();

	if (m_isHitStop && m_HitStopCount < m_TotalFrame)
	{
		filter.maskBits = 0;
		m_body->GetFixtureList()->SetFilterData(filter);

		// ボディタイプを静的に設定
		m_body->SetType(b2_staticBody);


		m_HitStopCount++;
		return true;
	}
	
	m_HitStopCount = 0;
	SetIsHitStop(false,0);
	filter.maskBits = ~0;
	m_body->GetFixtureList()->SetFilterData(filter);
		
	// ボディタイプを動的に設定
	m_body->SetType(b2_dynamicBody);
	return false;
	
}




