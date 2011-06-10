#include "stdafx.h"
#include <boost/foreach.hpp>
#include "LogLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace boost;

CFMParser::CFMParser(const char* pch, int len)
    :m_count(0),
	 m_current(const_cast<char*>(pch)),
	 m_length(len),
	 m_line(0)
{
}

CFMParser::~CFMParser()
{
}

string CFMParser::ParseKey()
{
	string key;

	if (*m_current == '[')
	{
		Next();

		Accept('"');

		char* p = m_current;

		while (*m_current != '"')
		{
			Next();
		}

		key.assign(p, m_current - p);

		Next();

		Accept(']');
	}

	return key;
}

fmlog::Value CFMParser::ParseValue()
{
	fmlog::Value v;

	switch (*m_current)
	{
	case 't':
		{
			Next();

			Accept('r');
			Accept('u');
			Accept('e');

			v.str = "true";
		}
		break;

	case 'f':
		{
			Next();

			Accept('a');
			Accept('l');
			Accept('s');
			Accept('e');

			v.str = "false";
		}
		break;

	case '[':
		{
			Next();
			Accept('[');

			while (*m_current != ']')
			{
				v.str.push_back(*m_current);
				Next();
			}

			Accept(']');
			Accept(']');
		}
		break;

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
		{
			v.str.push_back(*m_current);
			Next();

			while (*m_current != ',')
			{
				v.str.push_back(*m_current);
				Next();
			}
		}
		break;

	default:
		{
			while (*m_current != ',')
			{
				Next();
			}
		}
		break;
	}

	return v;
}

int CFMParser::ParseTimeStamp()
{
	Accept('-');
	Accept('-');
	Accept('~');

	return ParseInteger();
}

fmlog::Table* CFMParser::ParseTable()
{
	fmlog::Table* table = new fmlog::Table();

	while (true)
	{
		if (*m_current == '}')
		{
			Next();
			break;
		}

		string key = ParseKey();

		Accept('=');

		if (*m_current == '{')
		{
			Next();

			table->Add(key, ParseTable());
		}
		else
		{
			table->Add(key, ParseValue());
		}

		if (*m_current == ',')
		{
			Next();
		}
		else
		{
			CString str;
			str.Format("log file parse error (line %d)!", m_line);
			throw Error((LPCSTR)(str));
		}
	}

	return table;
}

//

CFMCfgData* CFMCfgParser::ParseData()
{
	auto_ptr<CFMCfgData> result(new CFMCfgData());

	m_line = 1;

	fmlog::Table* preset = 0;

	while (m_count < m_length)
	{
		Accept('{');

		preset = ParseTable();
	}

	if (preset)
	{
		BOOST_FOREACH(fmlog::Table::TableHash::value_type& t1, preset->tables)
		{
			fmcfg::Preset preset;
			preset.name = t1.first;

			BOOST_FOREACH(fmlog::Table::TableHash::value_type& t2, t1.second->tables)
			{
				BOOST_FOREACH(fmlog::Table::ValueHash::value_type& v, t2.second->values)
				{
					preset.keys.push_back(make_pair(v.first, v.second.str));
				}
			}

			result->AddPreset(preset);
		}

		delete preset;
	}

	return result.release();
}

//

CFMLogData* CFMLogParser::ParseData()
{
	auto_ptr<CFMLogData> result(new CFMLogData());

	int v = 0;
	int h = 0;
	int k = 0;
	int d = 0;

	m_line = 1;

	while (m_count < m_length)
	{
		Accept('\'');

		switch (*m_current)
		{
		case 'V':
			{
				Next();

				Accept('\'');
				Accept(',');

				int timestamp = ParseInteger();
				Accept(',');

				string name = ParseString();
				Accept(',');

				int version = ParseInteger();
				Accept(',');

				string date = ParseString();

				if (version != 2)
				{
					throw Error("supports only log version 2!");
				}

				CFMLogData::Info info;
				info.version = version;
				info.timestamp = timestamp;

				result->SetInfo(&info);

				v++;
			}
			break;

		case 'H':
			{
				Next();

				Accept('\'');
				Accept(',');

				int timestamp = ParseInteger();
				Accept(',');

				Accept('{');

				result->SetConfig(ParseTable());

				h++;
			}
			break;

		case 'K':
			{
				Next();

				Accept('\'');
				Accept(',');

				int timestamp = ParseInteger();
				Accept(',');

				int id = ParseInteger();
				Accept(',');

				string k1 = ParseString();
				Accept(',');

				string k2 = ParseString();

				result->AddIndex(id, k1, k2);

				k++;
			}
			break;

		case 'D':
			{
				Next();

				Accept('\'');
				Accept(',');

				int timestamp = ParseInteger();
				Accept(',');

				result->AddTimeStamp(timestamp);

				while (true)
				{
					if (*m_current == '{')
					{
						Next();

						int id = ParseInteger();
						Accept(',');

						int number = ParseFloat();

						Accept('}');

						result->GetList(id)->Add(timestamp, number);

						if (*m_current == ',')
						{
							Next();
						}
						else
						{
							break;
						}
					}
				}

				d++;
			}
			break;

		default:
			break;
		}

		//while (*m_current != '\n') Next();

		//Next();

		m_line++;
	}

	char buff[255];
	_snprintf(buff, 255, "v(%d), h(%d), k(%d), d(%d) line(%d).\n", v, h, k, d, m_line);
	TRACE(buff);

	return result.release();
}

//

CLogLoader::CLogLoader()
{
}

CLogLoader::~CLogLoader()
{
}

void CLogLoader::Reset()
{
}

CFMLogData* CLogLoader::Load(const char* pch, const size_t length)
{
	CFMLogParser parser(pch, length);
	return parser.ParseData();
}
