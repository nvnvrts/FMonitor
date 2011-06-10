#pragma once

#include <string>
#include <vector>
#include <hash_map>
#include <boost/foreach.hpp>

using namespace std;
using namespace stdext;

namespace fmlog
{
    struct Value
	{
		string str;
	};

	struct List
	{
		typedef pair<int, float> Item;
		typedef vector<Item> ItemList;

		float maxium;
		float sum;
		int count;

		ItemList list;

		List() : maxium(0.0f), sum(0.0f), count(0)
		{
			list.reserve(10000);
		}

		inline void Add(int timestamp, float number)
		{
			if (number > maxium)
			{
				maxium = number;
			}

			sum += number;
			count++;

			list.push_back(make_pair(timestamp, number));
		}

		float GetMax()
		{
			return maxium;
		}

		float GetAvg()
		{
			return (count > 0 ? sum / count : 0.0f);
		}
	};

	struct Table
	{
		typedef hash_map<string, Value> ValueHash;
		typedef hash_map<string, Table*> TableHash;

		ValueHash values;
		TableHash tables;

		~Table()
		{
			BOOST_FOREACH(TableHash::value_type v, tables)
			{
				delete v.second;
			}
		}

		void Add(const string& key, Value value)
		{
			values[key] = value;
		}

		void Add(const string& key, Table* table)
		{
			tables[key] = table;
		}
	};
}
