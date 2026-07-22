#pragma once

#include <string>
#include <vector>

#include <ostream>
#include <fstream> 

struct Header
{
	int tilemapSize[2];
	std::string tilemapPath;
	int cellSize[2];
	int spacing;
	int margin;

	int rowCol[2];
};

struct Layer
{
	std::string name;
	std::vector<std::vector<int>> cell;
};

struct Tilemap
{
	Header header;
	std::vector<Layer> layer;
};

class Scene;

namespace Demo
{
	class TileMapLoader
	{
	public:
		TileMapLoader() = default;
		~TileMapLoader() = default;

		void CreateMap(Scene* pScene, const std::string& path);

	private:
		Tilemap LoadTileMap(const std::string& path);
		void ReadLayer(std::ifstream& file, Header& header, Layer& layer);

		void CreateLayer(Scene* pScene, Tilemap& map);
	}; 
}
