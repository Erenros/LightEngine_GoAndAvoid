#include "AssetEngine.h"
#include <fstream>

bool AssetEngine::LoadFile(const std::string& path)
{
	std::ifstream file(path, std::ios::binary);
	if (!file)
	{
		DEBUG_WARN << "Can't open file with path : " << path << ENDL;
		return false;
	}

	if (!ReadHeader(file))
	{
		DEBUG_WARN << "Can't read Header" << ENDL;
		return false;
	}

	Entry entry;

	if (!ReadEntry(file, entry))
	{
		DEBUG_WARN << "Can't read Entry" << ENDL;
		return false;
	}

	DEBUG_INFO << "Key : " << static_cast<int>(entry.key) << "\n";
	DEBUG_INFO << "ID : " << entry.id << "\n";
	DEBUG_INFO << "Flag : " << static_cast<int>(entry.flag) << "\n";
	DEBUG_INFO << "Type : " << entry.type << "\n";
	DEBUG_INFO << "Width : " << entry.width << "\n";
	DEBUG_INFO << "Height : " << entry.height << "\n";
	DEBUG_INFO << "Size : " << entry.size << ENDL;


	DEBUG_INFO << "File load" << ENDL;
	return true;
}

bool AssetEngine::ReadHeader(std::ifstream& file)
{
	char signature[4];
	int8 version;

	file.read(signature, 4);
	file.read(&version,1);

	if (file.gcount() == 0 || memcmp(signature, "GCLE", 4) != 0)
	{
		DEBUG_WARN << "Invalid Signature" << ENDL;
		return false;
	}

	DEBUG_INFO << "Open file -> version : " << static_cast<int>(version)  << ENDL;

	return true;
}

bool AssetEngine::ReadEntry(std::ifstream& file, Entry& entry)
{
	file.read(reinterpret_cast<char*>(&entry), sizeof(entry));

	if (file.gcount() != sizeof(entry))
		return false;

	return true;
}
