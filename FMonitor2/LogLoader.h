#pragma once

#include <string>
#include <vector>
#include <hash_map>
#include <boost/foreach.hpp>
#include "Error.h"
#include "FMCfgData.h"
#include "FMLogData.h"

using namespace std;
using namespace stdext;

class CFMParser
{
protected:
	int m_count;
	int m_result;

	char* m_current;
	int m_length;

	int m_line;

public:
	CFMParser(const char* pch, int len);
	virtual ~CFMParser();

protected:
	inline void Next()
	{
		while (true)
		{
			++m_count;
			++m_current;

			switch (*m_current)
			{
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				continue;

			default:
				return;
			}
		}
	}

	inline void Accept(char ch)
	{
		if (ch == *m_current)
		{
			Next();
		}
		else
		{
			throw Error("log parse error!");
		}
	}

protected:
	string ParseKey();

	fmlog::Value ParseValue();

	int ParseTimeStamp();

	inline string ParseString()
	{
		string str;

		switch (*m_current)
		{
		case '\'':
			{
				Next();

				char* p = m_current;

				while (*m_current != '\'')
				{
					Next();
				}

				str.assign(p, m_current - p);

				Next();
			}
			break;

		default:
			throw;
			break;
		}

		return str;
	}

	inline int ParseInteger()
	{
		char* p = m_current;

		bool quit = false;
		while (!quit)
		{
			switch (*m_current)
			{
			case '-':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				Next();
				continue;

			default:
				quit = true;
				break;
			}
		}

		string str(p, m_current - p);
		int number = atoi(str.c_str());

		return number;
	}

	inline float ParseFloat()
	{
		char* p = m_current;

		bool quit = false;
		while (!quit)
		{
			switch (*m_current)
			{
			case '-':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '.':
				Next();
				continue;

			default:
				quit = true;
				break;
			}
		}

		string str(p, m_current - p);
		float number = atof(str.c_str());

		return number;
	}

	fmlog::Table* ParseTable();
};

//

class CFMCfgParser : public CFMParser
{
public:
	CFMCfgParser(const char* pch, int len) : CFMParser(pch, len)
	{
	}

	CFMCfgData* ParseData();
};

//

class CFMLogParser : public CFMParser
{
	typedef pair<string, string> Pair;
	typedef hash_map<int, Pair> Index;

	Index m_index;

public:
	CFMLogParser(const char* pch, int len) : CFMParser(pch, len)
	{
	}

	CFMLogData* ParseData();
};

//

class CLogLoader
{
public:
	CLogLoader();
	~CLogLoader();

public:
	CFMLogData* Load(const char* pch, const size_t length);

	void Reset();
};
