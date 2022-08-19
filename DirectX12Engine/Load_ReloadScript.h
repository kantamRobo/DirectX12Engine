#pragma once
#include <string>
#include <vector>
class Load_ReloadScript
{

	


	bool RunVSCode(const std::string& filename);
	bool LoadScript(const std::string& filename, std::vector<uint8_t>& data);
	
};

