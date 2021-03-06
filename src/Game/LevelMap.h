#pragma once

#include <cstdint>
#include "Dun.h"
#include "Helper2D.h"
#include "LevelObject.h"
#include <queue>
#include "TileSet.h"
#include "Sol.h"
#include <vector>

struct LevelCell
{
	int16_t minIndex{ -1 };
	std::shared_ptr<LevelObject> object;
	int8_t sol{ 0 };

	bool PassableIgnoreObject() const
	{
		return !(sol & 0x01);
	}

	bool Passable() const
	{
		// Temporary logic
		bool passable = PassableIgnoreObject();
		if (object != nullptr)
		{
			return object->Passable();
		}
		return passable;
	}
};

class LevelMap
{
private:
	static int tileSize;

	std::vector<LevelCell> cells;

	sf::Vector2u mapSize;
	sf::Vector2u size;

	static const LevelCell& get(size_t x, size_t y, const LevelMap& map)
	{
		return map.cells[x + y * map.Width()];
	}
	static LevelCell& get(size_t x, size_t y, LevelMap& map)
	{
		return map.cells[x + y * map.Width()];
	}

public:
	LevelMap() {}
	LevelMap(size_t width_, size_t height_);

	void setArea(size_t x, size_t y, const Dun& dun, const TileSet& til, const Sol& sol);

	Misc::Helper2D<LevelMap, LevelCell&> operator[] (size_t x)
	{
		return Misc::Helper2D<LevelMap, LevelCell&>(*this, x, get);
	}
	Misc::Helper2D<const LevelMap, const LevelCell&> operator[] (size_t x) const
	{
		return Misc::Helper2D<const LevelMap, const LevelCell&>(*this, x, get);
	}

	size_t Width() const { return mapSize.x; }
	size_t Height() const { return mapSize.y; }

	const sf::Vector2u& MapSize() const { return mapSize; }

	const sf::Vector2u& Size() const { return size; }

	static int TileSize() { return tileSize; }

	sf::Vector2f getCoords(const MapCoord& tile) const;
	MapCoord getTile(const sf::Vector2f& coords) const;

	std::queue<MapCoord> getPath(const MapCoord& a, const MapCoord& b) const;
};
