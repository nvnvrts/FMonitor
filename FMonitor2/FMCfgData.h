#pragma once

#include <string>
#include <utility>
#include <vector>

using namespace std;

namespace fmcfg
{
	struct Preset
	{
		typedef pair<string, string> Item;
		typedef vector<Item> List;

		UINT id;
		string name;
		List keys;
		string caption;
	};
}

class CFMCfgData
{
private:
	typedef vector<fmcfg::Preset> PresetList;
	PresetList m_preset;

public:
	CFMCfgData();

	virtual ~CFMCfgData();

	inline PresetList& GetPreset()
	{
		return m_preset;
	}

	void AddPreset(const fmcfg::Preset& preset)
	{
		m_preset.push_back(preset);
	}
};
