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

	std::vector<byte> data;
	if (!ReadData(file, entry, data))
	{
		DEBUG_WARN << "Can't read data" << ENDL;
		return false;
	}


	DEBUG_INFO << "File load with a size of : " << data.size() << " byte" << ENDL;
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

bool AssetEngine::ReadData(std::ifstream& file, Entry& entry, std::vector<byte>& outData)
{
	outData.resize(entry.size);
	if (entry.size == 0)
	{
		DEBUG_WARN << "Size is 0" << ENDL;
		return true;
	}

	file.read(reinterpret_cast<char*>(outData.data()), entry.size);
	if (file.gcount() != entry.size)
	{
		DEBUG_WARN << "Error reading data" << ENDL;
		return false;
	}

	for (int32 i = 0; i < entry.size; i++)
	{
		byte temp = outData[i] ^ entry.key;
		outData[i] = ((temp >> 3) | (temp << 5));
	}

	return true;
}
