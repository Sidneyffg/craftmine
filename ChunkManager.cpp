#include "ChunkManager.h"
#include <iostream>



void ChunkManager::init(unsigned short int renderDistance, unsigned int seed) {
	ChunkManager::renderDistance = renderDistance;
	ChunkManager::seed = seed;

	ChunkManager::bottomLeftChunkX = 0 - renderDistance / 2;
	ChunkManager::bottomLeftChunkZ = 0 - renderDistance / 2;

	ChunkManager::precalculatedBottomLeftChunkX = 0 - renderDistance / 2 - preGenNumber;
	ChunkManager::precalculatedBottomLeftChunkZ = 0 - renderDistance / 2 - preGenNumber;

	for (int i = 0; i < renderDistance; i++) {
		ChunkManager::chunkData.push_back(std::vector<std::vector<char>>(renderDistance, std::vector<char>(65536)));
		ChunkManager::chunkVertices.push_back(std::vector<std::vector<float>>(renderDistance, std::vector<float>()));
	}

	ChunkManager::pn.seed(seed);
}

void ChunkManager::pregenerateChunkData(int chunkX, int chunkY) {
	chunkPos pos{ chunkX,chunkY };
	std::vector<unsigned short int> chunkPrecalculatedGrassHeight(256);
	std::vector<unsigned short int> chunkPrecalculatedBiomes(256);
	std::vector<char> chunkPrecalculatedTreePositions(256);

	srand(ChunkManager::seed * (chunkX * ChunkManager::seed + chunkY) + ChunkManager::seed);

	for (int blockX = 0; blockX < 16; blockX++) {
		float x = (float)(chunkX * 16 + blockX);
		for (int blockZ = 0; blockZ < 16; blockZ++) {
			float z = (float)(chunkY * 16 + blockZ);
			chunkPrecalculatedGrassHeight[blockX * 16 + blockZ] = 30 + round(
				pow(ChunkManager::pn.noise(x / 10, z / 10, 0), squareCurve) * 10 +
				pow(ChunkManager::pn.noise(x / 40, z / 40, 0), squareCurve) * 20 +
				pow(ChunkManager::pn.noise(x / 800, z / 800, 0), 4) * 142 +
				pow(ChunkManager::pn.noise(x / 300, z / 300, 0.5f), 6) * 350
			);
			short int warmth = round(ChunkManager::pn.noise(x / 2000, z / 2000, 0) * 100); //original is 600
			if (warmth > 66) { warmth = 2; }
			else if (warmth > 56) { warmth = 1; }
			else if (warmth > 44) { warmth = 0; }
			else if (warmth > 34) { warmth = -1; }
			else { warmth = -2; }

			//getting biome

			if (chunkPrecalculatedGrassHeight[blockX * 16 + blockZ] < 60) {
				if (warmth >= 0) {
					chunkPrecalculatedBiomes[blockX * 16 + blockZ] = SEA;
				}
				else if (warmth == -1) {
					chunkPrecalculatedBiomes[blockX * 16 + blockZ] = HALF_FROZEN_SEA;
				}
				else {
					chunkPrecalculatedBiomes[blockX * 16 + blockZ] = FROZEN_SEA;
				}
			}
			else if (chunkPrecalculatedGrassHeight[blockX * 16 + blockZ] < 75) {
				if (warmth == 2) {
					chunkPrecalculatedBiomes[blockX * 16 + blockZ] = DESERT;
				}
				else if (warmth == 1) {
					chunkPrecalculatedBiomes[blockX * 16 + blockZ] = SAVANA;
				}
				else if (warmth == 0) {
					chunkPrecalculatedBiomes[blockX * 16 + blockZ] = PLAINS;
				}
				else if (warmth == -1) {
					chunkPrecalculatedBiomes[blockX * 16 + blockZ] = FOREST;
				}
				else {
					chunkPrecalculatedBiomes[blockX * 16 + blockZ] = SNOW_PLAINS;
				}
			}
			else if (chunkPrecalculatedGrassHeight[blockX * 16 + blockZ] < 100) {
				if (warmth == 2) {
					chunkPrecalculatedBiomes[blockX * 16 + blockZ] = RED_DESERT;
				}
				else if (warmth == 1) {
					chunkPrecalculatedBiomes[blockX * 16 + blockZ] = JUNGLE;
				}
				else if (warmth == 0) {
					chunkPrecalculatedBiomes[blockX * 16 + blockZ] = PLAINS;
				}
				else if (warmth == -1) {
					chunkPrecalculatedBiomes[blockX * 16 + blockZ] = TUNDRA;
				}
				else {
					chunkPrecalculatedBiomes[blockX * 16 + blockZ] = SNOW_PLAINS;
				}
			}
			else {
				if (warmth == 2) {
					chunkPrecalculatedBiomes[blockX * 16 + blockZ] = BADLANDS;
				}
				else if (warmth == 1) {
					chunkPrecalculatedBiomes[blockX * 16 + blockZ] = SANDSTONE_HILLS;
				}
				else if (warmth == 0) {
					chunkPrecalculatedBiomes[blockX * 16 + blockZ] = STONE_HILLS;
				}
				else if (warmth == -1) {
					chunkPrecalculatedBiomes[blockX * 16 + blockZ] = ICY_STONE_HILLS;
				}
				else {
					chunkPrecalculatedBiomes[blockX * 16 + blockZ] = ICE_HILLS;
				}
			}
			if (blockZ != 0 && chunkPrecalculatedTreePositions[blockX * 16 + blockZ - 1]) {
				chunkPrecalculatedTreePositions[blockX * 16 + blockZ] = NO_TREE;
			}
			else if (blockX != 0 && chunkPrecalculatedTreePositions[blockX * 16 + blockZ - 16]) {
				chunkPrecalculatedTreePositions[blockX * 16 + blockZ] = NO_TREE;
			}
			else if (blockZ != 0 && blockX != 0 && chunkPrecalculatedTreePositions[blockX * 16 + blockZ - 17]) {
				chunkPrecalculatedTreePositions[blockX * 16 + blockZ] = NO_TREE;
			}
			else {
				int treeChance = biomeInfo[chunkPrecalculatedBiomes[blockX * 16 + blockZ]]->treeAmount;
				if (treeChance != -1 && rand() % treeChance == 0) {
					if (rand() % 2 < 1) {
						chunkPrecalculatedTreePositions[blockX * 16 + blockZ] = FLIPPED_TREE;
					}
					else {
						chunkPrecalculatedTreePositions[blockX * 16 + blockZ] = YES_TREE;
					}
				}
				else {
					chunkPrecalculatedTreePositions[blockX * 16 + blockZ] = NO_TREE;
				}
			}
		}
	}

	precalculatedGrassHeight.insert(std::make_pair(pos, chunkPrecalculatedGrassHeight));
	precalculatedBiomes.insert(std::make_pair(pos, chunkPrecalculatedBiomes));
	precalculatedTreePositions.insert(std::make_pair(pos, chunkPrecalculatedTreePositions));
}

void ChunkManager::generateChunkData(int chunkX, int chunkY, unsigned int vectorDem1, unsigned int vectorDem2) {
	chunkPos pos{ chunkX,chunkY };
	std::vector<unsigned short int> chunkPrecalculatedGrassHeight = precalculatedGrassHeight[pos];
	std::vector<unsigned short int> chunkPrecalculatedBiomes = precalculatedBiomes[pos];
	std::vector<char> chunkPrecalculatedTreePositions = precalculatedTreePositions[pos];

	srand(ChunkManager::seed * (chunkX * ChunkManager::seed + chunkY) + ChunkManager::seed);
	//tree leaves
	chunkPos treeChunkPos;
	for (int treeChunkX = 0; treeChunkX < 3; treeChunkX++) {
		for (int treeChunkZ = 0; treeChunkZ < 3; treeChunkZ++) {
			treeChunkPos.changePos(chunkX + (treeChunkX - 1), chunkY + (treeChunkZ - 1));
			for (int x = 0; x < 16; x++) {
				for (int z = 0; z < 16; z++) {
					unsigned short int pos = x * 16 + z;
					if (precalculatedTreePositions[treeChunkPos][pos] != NO_TREE) {
						unsigned short int treeID = 2;
						unsigned short int halfTreeX = floor(treePresets[treeID]->treeX / 2);
						unsigned short int halfTreeZ = floor(treePresets[treeID]->treeZ / 2);

						short int fullX = x + (treeChunkX - 1) * 16 - treePresets[treeID]->trunkOffsetX;
						short int fullZ = z + (treeChunkZ - 1) * 16 - treePresets[treeID]->trunkOffsetZ;
						if (fullX + halfTreeX >= 0 && fullX - halfTreeX < 16 && fullZ + halfTreeZ >= 0 && fullZ - halfTreeZ < 16) {
							unsigned short int treeHeight = precalculatedGrassHeight[treeChunkPos][pos] + 1;
							for (int leaveX = 0; leaveX < treePresets[treeID]->treeX; leaveX++) {
								for (int leaveZ = 0; leaveZ < treePresets[treeID]->treeZ; leaveZ++) {
									if (fullX + (leaveX - halfTreeX) >= 0 && fullX + (leaveX - halfTreeX) < 16 && fullZ + (leaveZ - halfTreeZ) >= 0 && fullZ + (leaveZ - halfTreeZ) < 16) {
										for (int leaveY = 0; leaveY < treePresets[treeID]->treeY; leaveY++) {
											unsigned short int treeBlockPos;
											if (precalculatedTreePositions[treeChunkPos][pos] == FLIPPED_TREE) {
												treeBlockPos = abs(leaveX - (treePresets[treeID]->treeX - 1)) * treePresets[treeID]->treeZ + leaveY * treePresets[treeID]->treeX * treePresets[treeID]->treeZ + abs(leaveZ - (treePresets[treeID]->treeZ - 1));
											}
											else {
												treeBlockPos = leaveX * treePresets[treeID]->treeZ + leaveY * treePresets[treeID]->treeX * treePresets[treeID]->treeZ + leaveZ;
											}
											if (treePresets[treeID]->treePreset[treeBlockPos] != 0 && rand() % 100 < treePresets[treeID]->spawnChance[treeBlockPos]) {
												ChunkManager::chunkData[vectorDem1][vectorDem2][(leaveX - halfTreeX + fullX) * 16 + (leaveZ - halfTreeZ + fullZ) + (treeHeight + leaveY) * 256] = treePresets[treeID]->treePreset[treeBlockPos];
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	//terrain and tree trunks (will overwrite tree leaves)
	for (int blockX = 0; blockX < 16; blockX++) {
		float x = (float)(chunkX * 16 + blockX);
		for (int blockZ = 0; blockZ < 16; blockZ++) {
			float z = (float)(chunkY * 16 + blockZ);

			unsigned short int grassHeight = chunkPrecalculatedGrassHeight[blockX * 16 + blockZ];
			unsigned short int biome = chunkPrecalculatedBiomes[blockX * 16 + blockZ];
			bool generateTree = chunkPrecalculatedTreePositions[blockX * 16 + blockZ];

			//getting biome

			int treeID;
			for (int blockY = 0; blockY < 256; blockY++) {
				int blockID;
				if (blockY == 59) {
					blockID = ICE_GROUND;
				}
				else if (blockY < grassHeight) {
					blockID = 3;
				}
				else if (blockY == grassHeight) {
					if (rand() % 100 < biomeInfo[biome]->groundBlockPercentage) {
						blockID = biomeInfo[biome]->groundBlockType[0];
					}
					else {
						blockID = biomeInfo[biome]->groundBlockType[1];
					}
				}
				else {
					blockID = 0;
				}

				if (blockID != 0) {
					ChunkManager::chunkData[vectorDem1][vectorDem2][blockX * 16 + blockZ + blockY * 256] = blockID;
				}
			}
		}
	}
}

void ChunkManager::generateChunkVertices(unsigned int vectorDem1, unsigned int vectorDem2) {
	ChunkManager::chunkVertices[vectorDem1][vectorDem2].clear();
	int chunkOffsetX = (ChunkManager::bottomLeftChunkX + vectorDem1) * 16;
	int chunkOffsetZ = (ChunkManager::bottomLeftChunkZ + vectorDem2) * 16;
	for (int y = 0; y < 256; y++) {
		for (int x = 0; x < 16; x++) {
			for (int z = 0; z < 16; z++) {
				unsigned int block = ChunkManager::chunkData[vectorDem1][vectorDem2][y * 256 + x * 16 + z];
				if (block != 0) {
					int blockPosition = y * 256 + x * 16 + z;
					bool sidesToRender[6] = { true,true,true,true,true,true }; //top-front-right-back-left-bottom

					if ((blockPosition + 256) < 65536 && ChunkManager::chunkData[vectorDem1][vectorDem2][blockPosition + 256] != 0) { sidesToRender[0] = false; }

					if ((blockPosition + 1) % 16 != 0) {
						if (ChunkManager::chunkData[vectorDem1][vectorDem2][blockPosition + 1] != 0) {
							sidesToRender[1] = false;
						}
					}
					else if (vectorDem2 + 1 != ChunkManager::renderDistance && ChunkManager::chunkData[vectorDem1][vectorDem2 + 1][blockPosition - z] != 0) {
						sidesToRender[1] = false;
					}

					if (floor(blockPosition / 256) == floor((blockPosition + 16) / 256)) {
						if (ChunkManager::chunkData[vectorDem1][vectorDem2][blockPosition + 16] != 0) {
							sidesToRender[2] = false;
						}
					}
					else if (vectorDem1 + 1 != ChunkManager::renderDistance && ChunkManager::chunkData[vectorDem1 + 1][vectorDem2][blockPosition - x * 16] != 0) {
						sidesToRender[2] = false;
					}


					if (blockPosition % 16 != 0) {
						if (blockPosition != 0 && ChunkManager::chunkData[vectorDem1][vectorDem2][blockPosition - 1] != 0) {
							sidesToRender[3] = false;
						}
					}
					else if (vectorDem2 != 0 && ChunkManager::chunkData[vectorDem1][vectorDem2 - 1][blockPosition - z + 15] != 0) {
						sidesToRender[3] = false;
					}

					if (floor(blockPosition / 256) == floor((float)(blockPosition - 16) / 256)) {
						if (ChunkManager::chunkData[vectorDem1][vectorDem2][blockPosition - 16] != 0) {
							sidesToRender[4] = false;
						}
					}
					else if (vectorDem1 != 0 && ChunkManager::chunkData[vectorDem1 - 1][vectorDem2][blockPosition + 240] != 0) {
						sidesToRender[4] = false;
					}

					if ((blockPosition - 256) >= 0 && ChunkManager::chunkData[vectorDem1][vectorDem2][blockPosition - 256] != 0) { sidesToRender[5] = false; }


					int blockX = x + chunkOffsetX;
					int blockY = y;
					int blockZ = z + chunkOffsetZ;
					for (int side = 0; side < 6; side++) {
						if (sidesToRender[side]) {
							float blockSideTexture = (float)ChunkManager::CUBEID_TO_SIDES[block * 6 + side];
							float textureX, textureY;
							textureY = floor(blockSideTexture / ChunkManager::textureWidth);
							textureX = blockSideTexture - blockY * ChunkManager::textureWidth;
							for (int vertex = 0; vertex < 4; vertex++) {
								ChunkManager::chunkVertices[vectorDem1][vectorDem2].push_back(blockX + ChunkManager::CUBE_VERTICES_POSITIONS[side * 12 + vertex * 3]);
								ChunkManager::chunkVertices[vectorDem1][vectorDem2].push_back(blockY + ChunkManager::CUBE_VERTICES_POSITIONS[side * 12 + vertex * 3 + 1]);
								ChunkManager::chunkVertices[vectorDem1][vectorDem2].push_back(blockZ + ChunkManager::CUBE_VERTICES_POSITIONS[side * 12 + vertex * 3 + 2]);

								if (ChunkManager::TEXTURE_COORDS_X[vertex]) {
									ChunkManager::chunkVertices[vectorDem1][vectorDem2].push_back((textureX + 1) / ChunkManager::textureWidth);
								}
								else {
									ChunkManager::chunkVertices[vectorDem1][vectorDem2].push_back(textureX / ChunkManager::textureWidth);
								}

								if (ChunkManager::TEXTURE_COORDS_Y[vertex]) {
									ChunkManager::chunkVertices[vectorDem1][vectorDem2].push_back((textureY + 1) / ChunkManager::textureWidth);
								}
								else {
									ChunkManager::chunkVertices[vectorDem1][vectorDem2].push_back(textureY / ChunkManager::textureWidth);
								}
								ChunkManager::chunkVertices[vectorDem1][vectorDem2].push_back(normals[side][0]);
								ChunkManager::chunkVertices[vectorDem1][vectorDem2].push_back(normals[side][1]);
								ChunkManager::chunkVertices[vectorDem1][vectorDem2].push_back(normals[side][2]);
							}
						}
					}
				}
			}
		}
	}
	//std::cout << "Loaded chunk: " << vectorDem1 * renderDistance + vectorDem2 << std::endl;
}