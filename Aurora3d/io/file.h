
#include<type.h>
#include<module.h>
#include<io/logger.h>
#include<ccdk/text/string.h>


a3d_namespace_io_start

using namespace ccdk;

class general_file {
public:
	enum class mode:uint8{ 
		read,
		write_append,
		write_replace,
	};
private:
	FILE * handle;
public:
	general_file() :handle{ nullptr } {}
	general_file(txt::string const& filename, mode m = mode::read){}

	bool is_opened() { return handle != nullptr; }
	void open(txt::string const& filename, mode m = mode::read) {
		if (is_opened()) {
			A3D_WARNING("")
		}

	}
};


a3d_namespace_io_end
