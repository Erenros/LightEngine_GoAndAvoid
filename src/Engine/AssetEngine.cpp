#include "AssetEngine.h"
#include "Sprite.h"
#include <fstream>

Asset* AssetEngine::GetAsset(std::string id)
{
	if (!m_assetMap.contains(id))
		return nullptr;
	return m_assetMap[id];
}

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

	while (ReadEntry(file, entry))
	{
		if (entry.flag == 0x01)
		{
			DEBUG_INFO << "Entry " << entry.id << " Deleted " << "| size : " << entry.size << " byte" << ENDL;
			file.seekg(entry.size, std::ios::cur); // std::ios::cur = current pos 
			continue;
		}

		std::string name;
		ReadName(file, entry, name);

		std::vector<byte> data;
		if (!ReadData(file, entry, data))
		{
			DEBUG_WARN << "Si ce message s'affiche gg well play c'est casser donc recommance ta mal fait un truc" << ENDL;
			return false;
		};


		DEBUG_INFO << "Key : " << static_cast<int>(entry.key) << ENDL;
		DEBUG_INFO << "ID : " << entry.id << ENDL;
		DEBUG_INFO << "Name : " << name << ENDL;
		DEBUG_INFO << "Flag : " << static_cast<int>(entry.flag) << ENDL;
		DEBUG_INFO << "Type : " << entry.type << ENDL;
		DEBUG_INFO << "Width : " << entry.width << ENDL;
		DEBUG_INFO << "Height : " << entry.height << ENDL;
		DEBUG_INFO << "Size : " << entry.size << ENDL;

		DEBUG_INFO << "File load with a size of : " << data.size() << " byte" << ENDL;

		Asset* asset = new Asset;
		asset->id = entry.id;
		asset->name.resize(name.size());
		asset->name = std::string(name);
		asset->type = entry.type;
		asset->width = entry.width;
		asset->height = entry.height;
		asset->data = std::move(data);

		m_assetMap[asset->name] = asset;
	}

	file.close();

	return true;
}

std::unordered_map<std::string, Sprite*> AssetEngine::AssetToTexture(Window* window)
{
	std::unordered_map< std::string, Sprite*> textureMap;
	
	for (auto& pair : m_assetMap)
	{
		textureMap[pair.first] = new Sprite(window, pair.second);
		delete pair.second;
	}

	m_assetMap.clear();
	return textureMap;
}

void AssetEngine::ReadName(std::ifstream& file, Entry& entry, std::string& name)
{
	name.resize(entry.nameLength);
	file.read(name.data(), entry.nameLength);
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