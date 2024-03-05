#include "Common.h"
#include <fstream>

std::vector<char> ReadFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	/*ate: start playback at the end of the file
	binary: indicates that the file should be read as bytes and that these should not be formatted*/
	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();//because start in ate i have a size
	std::vector<char> buffer(fileSize);

	//Go back to the beginning of the file and read all the bytes at once
	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}
