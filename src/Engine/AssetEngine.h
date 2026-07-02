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
	int32 nameLength;
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
	int8 flag;
	int64 type;
	int16 width;
	int16 height;

	std::vector<byte> data;
	std::string path;
};


class AssetEngine
{
private:

	std::unordered_map<std::string, Asset*> m_assetMap;

private:

	void ReadFile(const std::string& path, std::vector<byte>& data);
	std::vector<byte> EncryptData(std::vector<byte>& data, int8 key);

	void ReadName(std::ifstream& file, Entry& entry, std::string& name);
	bool ReadHeader(std::ifstream& file);
	bool ReadEntry(std::ifstream& file, Entry& entry);
	bool ReadData(std::ifstream& file, Entry& entry, std::vector<byte>& outData);

public:

	static AssetEngine& GetInstance() {
		static AssetEngine instance;
		return instance;
	}

	void Init(Window* window,const std::string& path);

	Asset* GetAsset(std::string id);

	bool LoadFile(const std::string& path);
	void AddAsset(const std::string& id, Sprite* sprite);
	void SaveInFile(const std::string& path);

	std::unordered_map<std::string, Sprite*> AssetToTexture(Window* window);

	void DeleteAsset(const std::string& id);
	void ClearAsset();
};