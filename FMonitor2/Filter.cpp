#include "stdafx.h"
#include "Filter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace boost;

CFilter::CFilter(const RelationType relation, const float value, const ActionType action)
	:m_relation(relation),
	 m_value(value),
	 m_action(action)
{
}

CFilter::ActionType CFilter::Test(const float value) const
{
	bool test = true;

	switch (m_relation)
	{
	case GT: 
		test = value > m_value; 
		break;
	
	case GE:		
		test = value >= m_value;
		break;

	case LT:
		test = value < m_value;
		break;

	case LE:
		test = value <= m_value;
		break;

	case EQ:
		test = value == m_value;
		break;

	case NE:
		test = value != m_value;
		break;

	default:
		break;
	}

	return (test ? m_action : NONE);
}

CFilter CFMFilterParser::CreateFilter(const CFilter::ActionType action)
{
	CFilter::RelationType relation;

	switch (*m_current)
	{
	case '!':
		{
			Accept('=');
			relation = CFilter::NE;
		}
		break;

	case '=':
		{
			Accept('=');
			relation = CFilter::EQ;
		}
		break;
		
	case '>':
		{
			Next();
			
			if (*m_current == '=')
			{
				Next();
				relation = CFilter::GE;
			}
			else
			{
				relation = CFilter::GT;
			}
		}
		break;
		
	case '<':
		{
			Next();

			if (*m_current == '=')
			{
				Next();
				relation = CFilter::LE;
			}
			else
			{
				relation = CFilter::LT;
			}
		}
		break;
		
	default:
		throw Error("invalid filter expression!");
		break;
	}
	
	float value = ParseFloat();
	
	return CFilter(relation, value, action);
}
