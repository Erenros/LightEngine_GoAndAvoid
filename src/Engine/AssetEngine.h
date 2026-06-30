#pragma once
#include "include.h"
#include <unordered_map>

class Window;
class Sprite;

struct Header
{
	char signature[4];
	int8 version;
};

struct Entry
{
	int8 key;
	int64 id;
	std::string name;
	int8 flag;
	int64 type;
	int16 width;
	int16 height;
	int32 size;
};

struct Asset
{
	int64 id;
	std::string name;
	int64 type;
	int16 width;
	int16 height;

	std::vector<byte> data;
};

class AssetEngine
{
private:


public:

	static AssetEngine& GetInstance() {
		static AssetEngine instance;
		return instance;
	}
	
	std::unordered_map<std::string, Asset*> m_assetMap;

	Asset* GetAsset(std::string id);

	bool LoadFile(const std::string& path);

	std::unordered_map<std::string, Sprite*> AssetToTexture(Window* window);

	bool ReadHeader(std::ifstream& file);
	bool ReadEntry(std::ifstream& file, Entry& entry);
	bool ReadData(std::ifstream& file, Entry& entry, std::vector<byte>& outData);
};