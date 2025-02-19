/******************************************************
* HitStop.cpp	?q?b?g?X?g?b?v
* ?????F?J???}?^?g?E
* ?쐬???F2024/12/2
* ?ŏI?X?V???F2024/12/3
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
	//?q?b?g?X?g?b?v???邩
	bool isHitStop;

	//?t?B?N?X?`???S?Ẵt?B???^????
	b2Fixture* fixture = m_body->GetFixtureList();
	while (fixture) {
		// ?t?B???^?擾
		b2Filter filter = fixture->GetFilterData();

		if (m_isHitStop && m_HitStopCount < m_TotalFrame) {
			filter.maskBits = 0;
			fixture->SetFilterData(filter);

			// ?{?f?B?^?C?v???I?ɐݒ?
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

		
	// ?{?f?B?^?C?v?𓮓I?ɐݒ?
	m_body->SetType(b2_dynamicBody);
	return isHitStop;
	
}
