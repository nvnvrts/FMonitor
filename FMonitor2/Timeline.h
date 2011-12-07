#pragma once

#include "FMLogType.h"
#include "FMLogData.h"

class CTimeline : public CWnd
{
	const CFMLogData::TimeLine* m_timeline;
	int m_offset;
	int m_zoom;
	int m_meter;

	DECLARE_DYNAMIC(CTimeline)

public:
	CTimeline();
	virtual ~CTimeline();

	void SetOffset(unsigned int offset);
	void SetZoom(int zoom);
	void SetMeter(int x);
	enum RulerType { HOUR, HALFDAY, DAY };
	vector<int> GetRuler(const RulerType type = HOUR);
	void SetTimeline(const CFMLogData::TimeLine& m_timeline);

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
};
