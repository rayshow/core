
#include"../io/buffer.h"
#include"../io/logger.h"

namespace Aurora3D
{
	
//	bool ParseIniFile(const AString& name,
//		AStringMap& pairs)
//	{
//		MappingBuffer file_mapping{ name };
//		if (!file_mapping.IsValid()) return false;
//
//		unsigned size = file_mapping.GetBufferSize();
//		unsigned last_pos = 0u;
//		unsigned len = 0u;
//		unsigned i = 0u;
//		unsigned line_index = 0;
//		for (i = 0; i < size; ++i)
//		{
//#if defined(AURORA3D_WINDOW) || defined(AURORA3D_APPLE)
//			if ('\r' == file_mapping[i])
//#if defined(AURORA3D_WINDOW)
//				if ('\n' == file_mapping[i + 1] && i + 1 < size)
//#endif
//#else 
//			if ('\n' == mem[i])
//#endif
//			{
//				len = i - last_pos;
//				if (len >= 3)
//				{
//					std::string line(file_mapping[i], len);
//					std::string key, value;
//					if (SplitToTwo(line, '=', key, value))
//					{
//						pairs.emplace(key, value);
//					}
//					else {
//						A3D_WARNING("config file " + name + " line " + std::to_string(line_index) + " con't parse!");
//						continue;
//					}
//				}
//				last_pos = i + 1;
//#ifdef AURORA3D_WINDOW
//				++last_pos;
//#endif
//				++line_index;
//			}
//
//		}//for
//		len = i - last_pos;
//		if (len >= 3)
//		{
//			std::string line(file_mapping[i], len);
//			std::string key, value;
//			if (SplitToTwo(line, '=', key, value))
//			{
//				pairs.emplace(key, value);
//			}
//		}
//		else{
//			A3D_WARNING("config file " + name + " last line con't parse!");
//		}
//		return true;
//	}

}