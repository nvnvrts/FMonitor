#pragma once

#include <string>
#include "LogLoader.h"

using namespace std;

class CFilter
{
public:
	enum RelationType
	{
		GT,
		GE,
		LT,
		LE,
		EQ,
		NE,
	};

	enum ActionType
	{
		NONE,
		SHOW,
		HIDE,
	};

private:
	const RelationType m_relation;
	const float m_value;
	const ActionType m_action;

public:
	CFilter(const RelationType relation, const float value, const ActionType action);

	ActionType Test(const float value) const;
};

class CFMFilterParser : public CFMParser
{
public:
	CFMFilterParser(const char* pch, int len) : CFMParser(pch, len)
	{
	}

	CFilter CreateFilter(const CFilter::ActionType action);
};
