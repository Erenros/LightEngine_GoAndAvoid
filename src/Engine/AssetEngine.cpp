#include "AssetEngine.h"
#include "RessourceManager.h"
#include "Sprite.h"
#include <fstream>

void AssetEngine::Init(Window* window,const std::string& path)
{
	if (LoadFile("../../test.gcle"))
	{
		std::unordered_map<std::string, Sprite*> loadedSprite = AssetToTexture(window);

		for (auto& pair : loadedSprite)
			RessourceManager::GetInstance().ForcePutTexture(pair.second, pair.first);

		ClearAsset();
	}
}

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

		DEBUG_INFO << "File '" << name << "' load with a size of : " << data.size() << " byte" << ENDL;

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

void AssetEngine::AddAsset(const std::string& id, Sprite* sprite)
{
	if (m_assetMap.contains(id))
		return;

	Asset* asset = new Asset;
	asset->id = 0;
	asset->name = id;
	asset->width = 0;
	asset->height = 0;
	asset->type = 1;
	asset->path = "../../assets/textures/" + id + ".png";
	m_assetMap[id] = asset;
}

std::unordered_map<std::string, Sprite*> AssetEngine::AssetToTexture(Window* window)
{
	std::unordered_map< std::string, Sprite*> textureMap;
	
	for (auto& pair : m_assetMap)
		textureMap[pair.first] = new Sprite(window, pair.second);

	return textureMap;
}

void AssetEngine::DeleteAsset(const std::string& id)
{
	if (!m_assetMap.contains(id))
		return;

	m_assetMap[id]->flag = 0x01;
}

void AssetEngine::ClearAsset()
{
	for (auto& pair : m_assetMap)
		delete pair.second;

	m_assetMap.clear();
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

uint64 AssetEngine::HashName(const std::string& name)
{
	//FNV 1a because why not
	uint64 hash = 14695981039346656037ULL;
	for (byte c : name)
	{
		hash ^= c;
		hash *= 1099511628211ULL;
	}
	return hash;
}

void AssetEngine::SaveInFile(const std::string& path)
{
	LoadFile(path); //On reload car flm de garder des donnees en memoires quand le truc tourne

	std::ofstream file(path, std::ios::binary);
	if (!file)
	{
		DEBUG_WARN << "Can't open file with path : " << path << ENDL;
		return;
	}

	file.write("GCLE", 4);
	int8 version = 1;
	file.write(&version, 1);

	Entry entry;

	for (auto& pair : m_assetMap)
	{
		if (pair.second->flag == 0x01)
			continue;

		uint64 ID = pair.second->id;

		if (ID == 0)
			ID = HashName(pair.first);

		entry.key = 83; //Absolute hardcode
		entry.id = ID;
		entry.nameLength = static_cast<int32>(pair.first.size());
		entry.flag = pair.second->flag;
		entry.type = pair.second->type;
		entry.width = pair.second->width;
		entry.height = pair.second->height;
		 
		std::vector<byte> data;

		if (pair.second->path.size() != 0)
		{
			ReadFile(pair.second->path, data);
		}
		else if (pair.second->data.size() != 0)
		{
			data = std::move(pair.second->data);
		}
		else
			continue;

		std::vector<byte> encryptedData = EncryptData(data, entry.key);
		entry.size = static_cast<int32>(encryptedData.size());

		file.write(reinterpret_cast<char*>(&entry), sizeof(entry));
		file.write(pair.first.data(), entry.nameLength);
		file.write(reinterpret_cast<char*>(encryptedData.data()), encryptedData.size());
	}

	file.close();
}

void AssetEngine::ReadFile(const std::string& path, std::vector<byte>& data)
{
	std::ifstream file(path, std::ios::binary);
	if (!file)
		DEBUG_ERROR << "Can't read file with path : " << path << ENDL;

	file.seekg(0, std::ios::end);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	data.resize(static_cast<size_t>(size));
	file.read(reinterpret_cast<char*>(data.data()), size);
}

std::vector<byte> AssetEngine::EncryptData(std::vector<byte>& data, int8 key)
{
	std::vector<byte> encryptData(data.size());
	for (size_t i = 0; i < data.size(); i++)
	{
		byte temp = ((data[i] << 3) | (data[i] >> 5));
		encryptData[i] = temp ^ key;
	}

	return encryptData;
}