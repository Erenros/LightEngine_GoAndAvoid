#include "TileMapLoader.h"

#include "Core/include.h"
#include "Render/Sprite.h"
#include "Engine/RessourceManager.h"
#include "Engine/GameManager.h"
#include "Engine/Scene.h"

#include <array>
#include <filesystem>

void Demo::TileMapLoader::CreateMap(Scene* pScene, const std::string& path)
{
	Tilemap map = LoadTileMap(path);

	CreateLayer(pScene, map);



}

Tilemap Demo::TileMapLoader::LoadTileMap(const std::string& path)
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		GCLE_WARN << "Can't open file with path : " << path << ENDL;
		return Tilemap();
	}

	Tilemap map{};

	Header& header = map.header;

	std::string token;

	// Junk
	file >> token;
	file >> token;
	file >> token;

	// Map size
	file >> header.tilemapSize[0] >> header.tilemapSize[1];

	file >> token;
	if (token != "tileset")
		return Tilemap();

	file >> token;
	if (token != "endtileset")
	{ 
		header.tilemapPath = token;
		file >> header.cellSize[0] >> header.cellSize[1] >> header.spacing >> header.margin >> header.rowCol[1] >> header.rowCol[0];

		file >> token;
	}

	while (file >> token)
	{
		if (token == "layer")
		{
			map.layer.push_back(Layer());
			Layer& layer = map.layer[map.layer.size() - 1];
			 
			file >> layer.name;
			ReadLayer(file, header, layer);
		}
		else if (token == "object")
		{
			//ReadObject(file, *tileMap);
		}
		else if (token == "collider")
		{
			//ReadCollider(file, *tileMap);
		}
	}

	return map;
}

void Demo::TileMapLoader::ReadLayer(std::ifstream& file, Header& header, Layer& layer)
{
	std::string junk;
	file >> junk;
	file >> junk;
	file >> junk;
	file >> junk;


	layer.cell.resize(header.tilemapSize[1]);

	for (auto& cell : layer.cell)
	{
		cell.resize(header.tilemapSize[0]);
	}

	for (auto& cellY : layer.cell)
	{
		for (auto& cellX : cellY)
		{
			int tileIndex = -1;
			file >> tileIndex;
			cellX = tileIndex;
		}
	}

	file >> junk;
}

void Demo::TileMapLoader::CreateLayer(Scene* pScene, Tilemap& map)
{  
	auto OneDtoTwoDArray = [](int val, const Header& header) -> std::array<int, 2>
		{
			return {
				val / header.rowCol[1],
				val % header.rowCol[1] 
			};
		};
	
	std::string name = std::filesystem::path(map.header.tilemapPath).stem().string();

	int _Layer = 0;
	for (auto& layer : map.layer)
	{
		int x = 0;
		for (auto& cellY : layer.cell)
		{
			int y = 0;
			for (auto& cellX : cellY)
			{
				if (cellX == -1)
					continue;

				std::array<int, 2> pos = OneDtoTwoDArray(cellX, map.header);
				Entity* pEntity = pScene->CreateEntity<Entity>(gcle::Shapes::Rectangle);
				pEntity->SetRigidBody(false);

				pEntity->SetPosition((x - map.header.tilemapSize[0]) * 100, (y - map.header.tilemapSize[1]) * 100);
				pEntity->SetTexture(name);
				pEntity->GetRenderShape()->SetTextureRect(pos[0], pos[1], map.header.cellSize[0], map.header.cellSize[1], map.header.rowCol[0] * map.header.cellSize[0], map.header.rowCol[1] * map.header.cellSize[1]);
				pEntity->SetLayer(_Layer);

				y++;
			}
			x++;
		}
		_Layer++;
	}

	GameManager::GetInstance();
	int test = 0;
	test++;
}
