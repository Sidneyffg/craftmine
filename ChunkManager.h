#include <vector>
#include "perlinNoise.h"
#include <map>
#include <string>

#ifndef CHUNKS_CLASS_H
#define CHUNKS_CLASS_H

class ChunkManager {
public:
	void init(unsigned short int renderDistance, unsigned int seed);

	void pregenerateChunkData(int chunkX, int chunkY);
	void generateChunkData(int chunkX, int chunkY, unsigned int vectorDem1, unsigned int vectorDem2);
	void generateChunkVertices(unsigned int vectorDem1, unsigned int vectorDem2);

	std::vector<std::vector<std::vector<char>>> chunkData;
	std::vector<std::vector<std::vector<float>>> chunkVertices;

	int bottomLeftChunkX;
	int bottomLeftChunkZ;

	int precalculatedBottomLeftChunkX;
	int precalculatedBottomLeftChunkZ;

	int preGenNumber = 10;

	unsigned int seed;

	unsigned short int renderDistance;

	struct chunkPos {
		int x, y;

		bool operator<(const chunkPos& p) const {
			return x < p.x || (x == p.x && y < p.y);
		}
	};

private:
	std::map<chunkPos, std::vector<unsigned short int>> precalculatedGrassHeight;
	std::map<chunkPos, std::vector<unsigned short int>> precalculatedBiomes;
	std::map<chunkPos, std::vector<bool>> precalculatedTreePositions;

	PerlinNoise pn;

	const float textureWidth = 6.0f;

	const unsigned short int squareCurve = 5;

	const unsigned int CUBEID_TO_SIDES[6 * 20]{ //top-front-right-back-left-bottom
		0,0,0,0,0,0, //air
		18,18,18,18,18,18, //dirt 1
		24,29,29,29,29,18, //grass 2
		19,19,19,19,19,19, //stone 3
		7,7,7,7,7,7, //sand 4
		8,9,9,9,9,10, //sandstone 5
		11,11,11,11,11,11, //red sand 6
		0,1,1,1,1,2, //red sandstone 7
		3,3,3,3,3,3, //orange concrete 8
		4,4,4,4,4,4, //red concrete 9
		5,5,5,5,5,5, //clay 10
		21,20,20,20,20,21, //oak log 11
		23,22,22,22,22,23, //jungle log 12
		13,12,12,12,12,13, //birch log 13
		15,14,14,14,14,15, //acacia log 14
		17,16,16,16,16,17, //dark oak 15
		6,6,6,6,6,6, //leaves 16
		27,26,26,26,26,18, //show grass 17
		27,27,27,27,27,27, //snow 18
		28,28,28,28,28,28 //ice 19
	};

	const bool TEXTURE_COORDS_X[4] = {
		false,
		true,
		true,
		false
	};

	const bool TEXTURE_COORDS_Y[4]{
		false,
		false,
		true,
		true
	};

	const float CUBE_VERTICES_POSITIONS[72] = {
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,

		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,

		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,

		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,

		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,

		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
	};



	enum BiomeInfo {
		NO_TREES = -1,
		FEW_FEW_TREES = 99999,
		FEW_TREES = 9999,
		MANY_TREES = 15,
		MANY_MANY_TREES = 10,

		NO_TREE_TYPE = -1,
		OAK_TREE_TYPE = 11,
		JUNGLE_TREE_TYPE,
		BIRCH_TREE_TYPE,
		ACACIA_TREE_TYPE,
		DARK_OAK_TREE_TYPE,

		DIRT_GROUND = 1,
		GRASS_GROUND = 2,
		SNOW_GRASS_GROUND = 17,
		SAND_GROUND = 4,
		SANDSTONE_GROUND = 5,
		RED_SAND_GROUND = 6,
		RED_SANDSTONE_GROUND = 7,
		STONE_GROUND = 3,
		ICE_GROUND = 19
	};

	BiomeInfo biomeInfo[16][3] = { //tree amount, tree type, ground block type
		{NO_TREES,NO_TREE_TYPE,STONE_GROUND}, //SEA,
		{NO_TREES,NO_TREE_TYPE,STONE_GROUND}, //HALF_FROZEN_SEA,
		{NO_TREES,NO_TREE_TYPE,STONE_GROUND}, //FROZEN_SEA,
		{NO_TREES,NO_TREE_TYPE,SAND_GROUND}, //DESERT,
		{FEW_TREES,ACACIA_TREE_TYPE,GRASS_GROUND}, //SAVANA,
		{FEW_TREES,OAK_TREE_TYPE,GRASS_GROUND}, //PLAINS,
		{MANY_TREES,BIRCH_TREE_TYPE,GRASS_GROUND}, //FOREST,
		{FEW_FEW_TREES,OAK_TREE_TYPE,SNOW_GRASS_GROUND}, //SNOW_PLAINS,
		{NO_TREES,NO_TREE_TYPE,RED_SAND_GROUND}, //RED_DESERT,
		{MANY_MANY_TREES,JUNGLE_TREE_TYPE,GRASS_GROUND}, //JUNGLE,
		{FEW_TREES,DARK_OAK_TREE_TYPE,SNOW_GRASS_GROUND}, //TUNDRA,
		{NO_TREES,NO_TREE_TYPE,RED_SANDSTONE_GROUND}, //BADLANDS,
		{NO_TREES,NO_TREE_TYPE,SANDSTONE_GROUND}, //SANDSTONE_HILLS,
		{NO_TREES,NO_TREE_TYPE,STONE_GROUND}, //STONE_HILLS,
		{NO_TREES,NO_TREE_TYPE,ICE_GROUND}, //ICY_STONE_HILLS,
		{NO_TREES,NO_TREE_TYPE,ICE_GROUND} //ICE_HILLS
	};

	enum Biomes :unsigned short int {
		SEA,
		HALF_FROZEN_SEA,
		FROZEN_SEA,
		DESERT,
		SAVANA,
		PLAINS,
		FOREST,
		SNOW_PLAINS,
		RED_DESERT,
		JUNGLE,
		TUNDRA,
		BADLANDS,
		SANDSTONE_HILLS,
		STONE_HILLS,
		ICY_STONE_HILLS,
		ICE_HILLS
	};

	unsigned short int treeLeavesHeight[25]{
		2,2,2,2,2,
		2,3,4,3,2,
		2,4,4,4,2,
		2,3,4,3,2,
		2,2,2,2,2
	};
};

#endif