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

	int preGenNumber = 100;

	unsigned int seed;

	unsigned short int renderDistance;

	struct chunkPos {
		int x, y;

		void changePos(int x, int y) {
			this->x = x;
			this->y = y;
		}

		bool operator<(const chunkPos& p) const {
			return x < p.x || (x == p.x && y < p.y);
		}
	};

private:
	std::map<chunkPos, std::vector<unsigned short int>> precalculatedGrassHeight;
	std::map<chunkPos, std::vector<unsigned short int>> precalculatedBiomes;
	std::map<chunkPos, std::vector<char>> precalculatedTreePositions;
	enum treeNums : char {
		NO_TREE,
		YES_TREE,
		FLIPPED_TREE
	};

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
	class BiomeInfoClass {
	public:
		BiomeInfoClass(int treeAmount, unsigned int treeTrunkType[2], unsigned int treePercentage, unsigned int groundBlockType[2], unsigned int groundBlockPercentage, std::vector<unsigned short int> treeTypes) {
			this->treeAmount = treeAmount;
			this->treeTrunkType[0] = treeTrunkType[0];
			this->treeTrunkType[1] = treeTrunkType[1];
			this->treePercentage = treePercentage;
			this->groundBlockType[0] = groundBlockType[0];
			this->groundBlockType[1] = groundBlockType[1];
			this->groundBlockPercentage = groundBlockPercentage;

			this->treeTypes = treeTypes;
		}

		int treeAmount;
		unsigned int treeTrunkType[2];
		unsigned int treePercentage;
		unsigned int groundBlockType[2];
		unsigned int groundBlockPercentage;

		std::vector<unsigned short int> treeTypes;
	};
	std::vector<BiomeInfoClass*> biomeInfo{
		new BiomeInfoClass(NO_TREES, new unsigned int[2] {NULL, NULL}, NULL, new unsigned int[2] {STONE_GROUND, NULL}, 100, std::vector<unsigned short int> {}), //SEA
		new BiomeInfoClass(NO_TREES, new unsigned int[2] {NULL, NULL}, NULL, new unsigned int[2] {STONE_GROUND, NULL}, 100, std::vector<unsigned short int> {}), //HALF_FROZEN_SEA
		new BiomeInfoClass(NO_TREES, new unsigned int[2] {NULL, NULL}, NULL, new unsigned int[2] {STONE_GROUND, NULL}, 100, std::vector<unsigned short int> {}), //FROZEN_SEA
		new BiomeInfoClass(NO_TREES, new unsigned int[2] {NULL, NULL}, NULL, new unsigned int[2] {SAND_GROUND, NULL}, 100, std::vector<unsigned short int> {}), //DESERT
		new BiomeInfoClass(40000, new unsigned int[2] {ACACIA_TREE_TYPE, NULL}, 100, new unsigned int[2] {GRASS_GROUND, NULL}, 100, std::vector<unsigned short int> {1,2}), //SAVANA
		new BiomeInfoClass(40000, new unsigned int[2] {OAK_TREE_TYPE, NULL}, 100, new unsigned int[2] {GRASS_GROUND, NULL}, 100, std::vector<unsigned short int> {0}), //PLAINS
		new BiomeInfoClass(200, new unsigned int[2] {BIRCH_TREE_TYPE, OAK_TREE_TYPE}, 50, new unsigned int[2] {GRASS_GROUND, NULL}, 100, std::vector<unsigned short int> {}), //FOREST
		new BiomeInfoClass(40000, new unsigned int[2] {OAK_TREE_TYPE, NULL}, 100, new unsigned int[2] {SNOW_GRASS_GROUND, NULL}, 100, std::vector<unsigned short int> {}), //SNOW_PLAINS
		new BiomeInfoClass(NO_TREES, new unsigned int[2] {NULL, NULL}, NULL, new unsigned int[2] {RED_SAND_GROUND, NULL}, 100, std::vector<unsigned short int> {}), //RED_DESERT
		new BiomeInfoClass(17, new unsigned int[2] {JUNGLE_TREE_TYPE, NULL}, 100, new unsigned int[2] {GRASS_GROUND, NULL}, 100, std::vector<unsigned short int> {}), //JUNGLE
		new BiomeInfoClass(350, new unsigned int[2] {DARK_OAK_TREE_TYPE, NULL}, 100, new unsigned int[2] {SNOW_GRASS_GROUND, GRASS_GROUND}, 50, std::vector<unsigned short int> {}), //TUNDRA
		new BiomeInfoClass(NO_TREES, new unsigned int[2] {NULL, NULL}, NULL, new unsigned int[2] {RED_SANDSTONE_GROUND, NULL}, 100, std::vector<unsigned short int> {}), //BADLANDS
		new BiomeInfoClass(NO_TREES, new unsigned int[2] {NULL, NULL}, NULL, new unsigned int[2] {SANDSTONE_GROUND, NULL}, 100, std::vector<unsigned short int> {}), //SANDSTONE_HILLS
		new BiomeInfoClass(NO_TREES, new unsigned int[2] {NULL, NULL}, NULL, new unsigned int[2] {STONE_GROUND, NULL}, 100, std::vector<unsigned short int> {}), //STONE_HILLS
		new BiomeInfoClass(NO_TREES, new unsigned int[2] {NULL, NULL}, NULL, new unsigned int[2] {ICE_GROUND, 18}, 50, std::vector<unsigned short int> {}), //ICY_STONE_HILLS
		new BiomeInfoClass(NO_TREES, new unsigned int[2] {NULL, NULL}, NULL, new unsigned int[2] {ICE_GROUND, NULL}, 100, std::vector<unsigned short int> {}) //ICE_HILLS
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

	float normals[6][3] = {
		{0,1,0},
		{0,0,1},
		{1,0,0},
		{0,0,-1},
		{-1,0,0},
		{0,-1,0}
	};


	class TreePreset {
	public:
		unsigned short int treeX;
		unsigned short int treeY;
		unsigned short int treeZ;

		short int trunkOffsetX;
		short int trunkOffsetZ;

		std::vector<unsigned short int> treePreset;
		std::vector<unsigned short int> spawnChance;
	};

	std::vector<TreePreset*> treePresets{
		new TreePreset {
			5,
			8,
			5,

			0,
			0,

			{
				0,0,0,0,0,0,0,0,0,0,0,0,OAK_TREE_TYPE,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,OAK_TREE_TYPE,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,OAK_TREE_TYPE,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,OAK_TREE_TYPE,0,0,0,0,0,0,0,0,0,0,0,0,
				16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
				16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
				0,0,0,0,0,0,16,16,16,0,0,16,16,16,0,0,16,16,16,0,0,0,0,0,0,
				0,0,0,0,0,0,0,16,0,0,0,16,16,16,0,0,0,16,0,0,0,0,0,0,0,
			},
			{
				0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,
				100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
				50,100,100,100,50,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,50,100,100,100,50,
				0,0,0,0,0,0,100,100,100,0,0,100,100,100,0,0,100,100,100,0,0,0,0,0,0,
				0,0,0,0,0,0,0,100,0,0,0,100,100,100,0,0,0,100,0,0,0,0,0,0,0,
			}
		},
		new TreePreset {
			10,
			8,
			7,

			-1,
			0,

			{
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,ACACIA_TREE_TYPE,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,ACACIA_TREE_TYPE,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,ACACIA_TREE_TYPE,0,0,0,0,0,0,ACACIA_TREE_TYPE,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,16,16,16,0,0,0,16,16,16,16,16,0,0,16,16,ACACIA_TREE_TYPE,16,16,0,0,16,16,16,16,16,0,0,0,16,16,16,0,0,0,0,0,ACACIA_TREE_TYPE,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,16,16,16,0,0,0,0,16,16,16,0,0,0,0,16,16,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,ACACIA_TREE_TYPE,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,ACACIA_TREE_TYPE,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,16,16,16,16,0,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,ACACIA_TREE_TYPE,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,0,16,16,16,16,16,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,0,0,0,0,16,16,16,0,0,0,16,16,16,16,16,0,0,0,16,16,16,0,0,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0
			},
			{
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,100,100,100,0,0,0,100,100,100,100,100,0,0,100,100,100,100,100,0,0,100,100,100,100,100,0,0,0,100,100,100,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,100,100,100,0,0,0,0,100,100,100,0,0,0,0,100,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100,100,100,100,0,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,0,100,100,100,100,100,0,
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,100,100,100,0,0,0,100,100,100,100,100,0,0,0,100,100,100,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0
			}
		},
		new TreePreset {
			7,
			8,
			10,

			0,
			-1,

			{
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,ACACIA_TREE_TYPE,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,ACACIA_TREE_TYPE,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,ACACIA_TREE_TYPE,ACACIA_TREE_TYPE,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,16,16,0,0,0,0,0,0,16,16,16,16,16,0,0,0,0,0,16,16,ACACIA_TREE_TYPE,16,16,ACACIA_TREE_TYPE,0,0,0,0,16,16,16,16,16,0,0,0,0,0,0,16,16,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,16,16,0,0,0,0,0,0,0,16,16,16,0,0,ACACIA_TREE_TYPE,0,0,0,0,16,16,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,ACACIA_TREE_TYPE,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,16,16,16,16,0,0,0,0,16,16,16,16,16,16,16,0,0,0,16,16,16,16,16,16,16,0,0,0,16,16,16,ACACIA_TREE_TYPE,16,16,16,0,0,0,16,16,16,16,16,16,16,0,0,0,16,16,16,16,16,16,16,0,0,0,0,16,16,16,16,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,0,0,0,0,0,0,0,16,16,16,0,0,0,0,0,0,16,16,16,16,16,0,0,0,0,0,0,16,16,16,0,0,0,0,0,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
			},
			{
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100,100,0,0,0,0,0,0,100,100,100,100,100,0,0,0,0,0,100,100,100,100,100,100,0,0,0,0,100,100,100,100,100,0,0,0,0,0,0,100,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100,100,0,0,0,0,0,0,0,100,100,100,0,0,100,0,0,0,0,100,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100,100,100,100,0,0,0,0,100,100,100,100,100,100,100,0,0,0,100,100,100,100,100,100,100,0,0,0,100,100,100,100,100,100,100,0,0,0,100,100,100,100,100,100,100,0,0,0,100,100,100,100,100,100,100,0,0,0,0,100,100,100,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,100,100,100,0,0,0,0,0,0,100,100,100,100,100,0,0,0,0,0,0,100,100,100,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
			}
		}
	};
};

#endif