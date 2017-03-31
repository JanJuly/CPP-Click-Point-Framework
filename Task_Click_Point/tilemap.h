#pragma once
#include "entity.h"

class tilemap : public entity
{
public:
	int map_width = 10;
	int map_height = 10;
	float tile_size = 32.f;

	sf::Texture tileset_texture;
	sf::Sprite tileset_spr;

};
