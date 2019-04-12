#pragma once
#include <../load/sConfig.h>
#include <../load/json/json.h>

namespace nLoad
{
	inline bool FileExists(const char* fileName)
	{
		struct stat buffer;
		return (stat(fileName, &buffer) == 0);
	}

	bool LoadConfig(const char* fileName, sConfig& configOut);
	bool ReadJsonFile(const char* fileName, Json::Value& jsonOut);
}