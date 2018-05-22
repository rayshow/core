#pragma once

#include<string>
#include<module.h>

a3d_namespace_io_start
	class PathHelper
	{
	public:	
		A3D_API static std::string UnixStylePath(const std::string& full_path);
		A3D_API static void SplitPath(const std::string& unixStylePath,
			std::string& dir, std::string& name, std::string& suffix);
		A3D_API static std::string GetDir(const std::string& us_path);
		A3D_API static std::string GetSuffix(const std::string& us_path);
		A3D_API static std::string GetFileName(const std::string& us_path);
	};

a3d_namespace_io_end