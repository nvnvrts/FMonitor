#pragma once

#include <string>

using namespace std;

class Error
{
	const string m_reason;

public:
	Error(const string& reason) : m_reason(reason)
	{
	}

	~Error()
	{
	}

	const string& GetReason() const
	{
		return m_reason;
	}
};
