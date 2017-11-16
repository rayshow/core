#pragma once

#include<string>
#include"../common_def.h"

namespace Aurora3D
{
	class PathHelper
	{
	public:	
		AURORCCDK_API static std::string UnixStylePath(const std::string& full_path);
		AURORCCDK_API static void SplitPath(const std::string& unixStylePath,
			std::string& dir, std::string& name, std::string& suffix);
		AURORCCDK_API static std::string GetDir(const std::string& us_path);
		AURORCCDK_API static std::string GetSuffix(const std::string& us_path);
		AURORCCDK_API static std::string GetFileName(const std::string& us_path);
	};
}