
#include"file_system.h"
#include"../string/astring.h"

namespace Aurora3D
{
	std::string PathHelper::UnixStylePath(const std::string& path)
	{
		return "";
	}

	std::string PathHelper::GetDir(const std::string& path)
	{
		unsigned last_slash_pos = path.find_last_of('/');
		unsigned dot_pos = path.find_last_of('.');
		if (std::string::npos != last_slash_pos && 
			std::string::npos != dot_pos &&
			dot_pos > last_slash_pos)
		{
			return path.substr(0, last_slash_pos+1);
		}
		return std::string{};
	}

	std::string PathHelper::GetSuffix(const std::string& path)
	{
		unsigned len = path.length();
		unsigned last_dot_pos = path.find_last_of('.');
		if (std::string::npos != last_dot_pos)
		{
			return path.substr(last_dot_pos + 1, len - 1 - last_dot_pos);
		}
		return std::string{};
	}

	std::string PathHelper::GetFileName(const std::string& path)
	{
		unsigned len = path.length();
		unsigned last_sep_pos = path.find_last_of('/');
		if (std::string::npos != last_sep_pos)
		{
			return path.substr(last_sep_pos + 1);
		}
		return path;
	}

	void PathHelper::SplitPath(const std::string& full_path,
		std::string& dir, std::string& name, std::string& suffix)
	{
		unsigned len = full_path.length();
		unsigned last_dot_pos = full_path.find_last_of('.');
		unsigned last_slash_pos = full_path.find_last_of('/');

		//no dot, no slash
		if (last_dot_pos <= last_slash_pos)
		{
			dir.clear();
			name.clear();
			suffix.clear();
			return;
		}
		//no slash but have dot
		if (std::string::npos == last_slash_pos)
		{
			name = full_path.substr(0, last_dot_pos);
			suffix = full_path.substr(last_dot_pos + 1, len - 1 - last_dot_pos);
			dir.clear();
			return;
		}

		//have slash and dot
		dir = full_path.substr(0, last_slash_pos + 1);
		name = full_path.substr(last_slash_pos + 1, last_dot_pos - 1 - last_slash_pos);
		suffix = full_path.substr(last_dot_pos + 1, len - last_dot_pos - 1);
	}
}