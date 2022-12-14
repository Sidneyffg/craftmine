#include "Craftmine.h"
#include <iostream>


Craftmine::Craftmine(unsigned short int renderDistance, unsigned int seed, GLFWwindow* window) {
	Craftmine::window = window;

	chunkManager.init(renderDistance, seed);
}

void Craftmine::changeRenderDistance(unsigned short int newRenderDistance, glm::vec3 camPos) {
	//Craftmine::renderDistance = newRenderDistance;
	//reloadChunks(camPos);
}

void Craftmine::reloadChunks(glm::vec3 camPos) {
	chunkManager.chunkData.clear();
	chunkManager.chunkVertices.clear();
	chunkManager.chunkData.reserve(chunkManager.renderDistance);
	chunkManager.chunkVertices.reserve(chunkManager.renderDistance);
	for (int i = 0; i < chunkManager.renderDistance; i++) {
		chunkManager.chunkData.push_back(std::vector<std::vector<char>>(chunkManager.renderDistance, std::vector<char>(65536)));
		chunkManager.chunkVertices.push_back(std::vector<std::vector<float>>(chunkManager.renderDistance, std::vector<float>()));
	}

	chunkManager.bottomLeftChunkX = round(camPos[0] / 16) - chunkManager.renderDistance / 2;
	chunkManager.bottomLeftChunkZ = round(camPos[2] / 16) - chunkManager.renderDistance / 2;

	for (int i = 0; i < chunkManager.renderDistance; i++) {
		for (int j = 0; j < chunkManager.renderDistance; j++) {
			chunkManager.generateChunkData(i + chunkManager.bottomLeftChunkX, j + chunkManager.bottomLeftChunkZ, i, j);
		}
	}
	for (int i = 0; i < chunkManager.renderDistance; i++) {
		for (int j = 0; j < chunkManager.renderDistance; j++) {
			chunkManager.generateChunkVertices(i, j);
		}
	}
}

void Craftmine::generateStartChunks() {
	for (int i = 0; i < chunkManager.renderDistance + chunkManager.preGenNumber * 2; i++) {
		for (int j = 0; j < chunkManager.renderDistance + chunkManager.preGenNumber * 2; j++) {
			chunkManager.pregenerateChunkData(i + chunkManager.precalculatedBottomLeftChunkX, j + chunkManager.precalculatedBottomLeftChunkZ);
		}
	}
	for (int i = 0; i < chunkManager.renderDistance; i++) {
		for (int j = 0; j < chunkManager.renderDistance; j++) {
			chunkManager.generateChunkData(i + chunkManager.bottomLeftChunkX, j + chunkManager.bottomLeftChunkZ, i, j);
		}
	}
	for (int i = 0; i < chunkManager.renderDistance; i++) {
		for (int j = 0; j < chunkManager.renderDistance; j++) {
			chunkManager.generateChunkVertices(i, j);
		}
	}
}

void Craftmine::generateVertices() {
	newAllChunkVertices.clear();
	newAllChunkIndices.clear();
	std::cout << "generating vertices..." << std::endl;
	for (int i = 0; i < chunkManager.renderDistance; i++) {
		for (int j = 0; j < chunkManager.renderDistance; j++) {
			Craftmine::newAllChunkVertices.insert(Craftmine::newAllChunkVertices.end(), chunkManager.chunkVertices[j][i].begin(), chunkManager.chunkVertices[j][i].end());
		}
	}
	Craftmine::newAllChunkVertices.shrink_to_fit();
	int indicesSize = Craftmine::newAllChunkVertices.size() / 5 / 4;
	Craftmine::newAllChunkIndices.reserve(indicesSize * 6);
	//std::cout << "indicesSize: " << indicesSize * 6 << std::endl;
	for (int i = 0; i < indicesSize; i++) {
		for (int j = 0; j < 6; j++) {
			Craftmine::newAllChunkIndices.push_back(Craftmine::CUBE_INDICES[j] + i * 4);
		}
	}
	allChunkIndices = newAllChunkIndices;
	allChunkVertices = newAllChunkVertices;
}





bool Craftmine::loadSideChunks(glm::vec3 camPos) {
	if (camPos[2] > (chunkManager.bottomLeftChunkZ + chunkManager.renderDistance / 2) * 16 + 8) {
		Craftmine::loadTopChunks();
		bool response = true;
		while (response) {
			response = Craftmine::loadSideChunks(camPos);
		}
		return true;
	}
	else if (camPos[2] < (chunkManager.bottomLeftChunkZ + chunkManager.renderDistance / 2) * 16 - 8) {
		Craftmine::loadBottomChunks();
		bool response = true;
		while (response) {
			response = Craftmine::loadSideChunks(camPos);
		}
		return true;
	}
	else if (camPos[0] > (chunkManager.bottomLeftChunkX + chunkManager.renderDistance / 2) * 16 + 8) {
		Craftmine::loadRightChunks();
		bool response = true;
		while (response) {
			response = Craftmine::loadSideChunks(camPos);
		}
		return true;
	}
	else if (camPos[0] < (chunkManager.bottomLeftChunkX + chunkManager.renderDistance / 2) * 16 - 8) {
		Craftmine::loadLeftChunks();
		bool response = true;
		while (response) {
			response = Craftmine::loadSideChunks(camPos);
		}
		return true;
	}
	return false;
}

void Craftmine::loadTopChunks() {
	for (int i = 0; i < chunkManager.renderDistance; i++) {
		chunkManager.chunkData[i].erase(chunkManager.chunkData[i].begin());
		chunkManager.chunkVertices[i].erase(chunkManager.chunkVertices[i].begin());
		chunkManager.chunkData[i].push_back(std::vector<char>(65536));
		chunkManager.chunkVertices[i].push_back(std::vector<float>());
	}
	chunkManager.bottomLeftChunkZ++;
	for (int i = 0; i < chunkManager.renderDistance; i++) {
		chunkManager.generateChunkData(chunkManager.bottomLeftChunkX + i, chunkManager.bottomLeftChunkZ + chunkManager.renderDistance - 1, i, chunkManager.renderDistance - 1);
	}
	for (int i = 0; i < chunkManager.renderDistance; i++) {
		chunkManager.generateChunkVertices(i, chunkManager.renderDistance - 1);
		chunkManager.generateChunkVertices(i, chunkManager.renderDistance - 2);
	}
}

void Craftmine::loadRightChunks() {
	chunkManager.chunkData.erase(chunkManager.chunkData.begin());
	chunkManager.chunkVertices.erase(chunkManager.chunkVertices.begin());
	chunkManager.chunkData.push_back(std::vector<std::vector<char>>(chunkManager.renderDistance, std::vector<char>(65536)));
	chunkManager.chunkVertices.push_back(std::vector<std::vector<float>>(chunkManager.renderDistance, std::vector<float>()));
	chunkManager.bottomLeftChunkX++;
	for (int i = 0; i < chunkManager.renderDistance; i++) {
		chunkManager.generateChunkData(chunkManager.bottomLeftChunkX + chunkManager.renderDistance - 1, chunkManager.bottomLeftChunkZ + i, chunkManager.renderDistance - 1, i);
	}
	for (int i = 0; i < chunkManager.renderDistance; i++) {
		chunkManager.generateChunkVertices(chunkManager.renderDistance - 1, i);
		chunkManager.generateChunkVertices(chunkManager.renderDistance - 2, i);
	}
}

void Craftmine::loadBottomChunks() {
	for (int i = 0; i < chunkManager.renderDistance; i++) {
		chunkManager.chunkData[i].erase(chunkManager.chunkData[i].begin() + chunkManager.renderDistance - 1);
		chunkManager.chunkVertices[i].erase(chunkManager.chunkVertices[i].begin() + chunkManager.renderDistance - 1);
		chunkManager.chunkData[i].insert(chunkManager.chunkData[i].begin(), std::vector<char>(65536));
		chunkManager.chunkVertices[i].insert(chunkManager.chunkVertices[i].begin(), std::vector<float>());
	}
	chunkManager.bottomLeftChunkZ--;
	for (int i = 0; i < chunkManager.renderDistance; i++) {
		chunkManager.generateChunkData(chunkManager.bottomLeftChunkX + i, chunkManager.bottomLeftChunkZ, i, 0);
	}
	for (int i = 0; i < chunkManager.renderDistance; i++) {
		chunkManager.generateChunkVertices(i, 0);
		chunkManager.generateChunkVertices(i, 1);
	}
}

void Craftmine::loadLeftChunks() {
	chunkManager.chunkData.pop_back();
	chunkManager.chunkVertices.pop_back();
	chunkManager.chunkData.insert(chunkManager.chunkData.begin(), std::vector<std::vector<char>>(chunkManager.renderDistance, std::vector<char>(65536)));
	chunkManager.chunkVertices.insert(chunkManager.chunkVertices.begin(), std::vector<std::vector<float>>(chunkManager.renderDistance, std::vector<float>()));
	chunkManager.bottomLeftChunkX--;
	for (int i = 0; i < chunkManager.renderDistance; i++) {
		chunkManager.generateChunkData(chunkManager.bottomLeftChunkX, chunkManager.bottomLeftChunkZ + i, 0, i);
	}
	for (int i = 0; i < chunkManager.renderDistance; i++) {
		chunkManager.generateChunkVertices(0, i);
		chunkManager.generateChunkVertices(1, i);
	}
}

void Craftmine::tp(float x, float y, float z, Camera* camera) {
	glm::vec3 newCamPos = glm::vec3(x, y, z);
	camera->Position = newCamPos;
	Craftmine::reloadChunks(newCamPos);
}

void Craftmine::checkForChunksToLoad(glm::vec3 camPos) {
	if (camPos[2] > (chunkManager.precalculatedBottomLeftChunkZ + chunkManager.preGenNumber + chunkManager.renderDistance / 2) * 16 + 8) {
		for (int i = 0; i < chunkManager.renderDistance + chunkManager.preGenNumber * 2; i++) {
			chunkManager.pregenerateChunkData(chunkManager.precalculatedBottomLeftChunkX + i, chunkManager.precalculatedBottomLeftChunkZ + chunkManager.renderDistance + chunkManager.preGenNumber * 2 + 1);
		}
		chunkManager.precalculatedBottomLeftChunkZ++;
	}
	else if (camPos[2] < (chunkManager.precalculatedBottomLeftChunkZ + chunkManager.preGenNumber + chunkManager.renderDistance / 2) * 16 - 8) {
		for (int i = 0; i < chunkManager.renderDistance + chunkManager.preGenNumber * 2; i++) {
			chunkManager.pregenerateChunkData(chunkManager.precalculatedBottomLeftChunkX + i, chunkManager.precalculatedBottomLeftChunkZ - 1);
		}
		chunkManager.precalculatedBottomLeftChunkZ--;
	}
	else if (camPos[0] > (chunkManager.precalculatedBottomLeftChunkX + chunkManager.preGenNumber + chunkManager.renderDistance / 2) * 16 + 8) {
		for (int i = 0; i < chunkManager.renderDistance + chunkManager.preGenNumber * 2; i++) {
			chunkManager.pregenerateChunkData(chunkManager.bottomLeftChunkX + chunkManager.renderDistance + 1, chunkManager.precalculatedBottomLeftChunkZ + i);
		}
		chunkManager.precalculatedBottomLeftChunkX++;
	}
	else if (camPos[0] < (chunkManager.precalculatedBottomLeftChunkX + chunkManager.preGenNumber + chunkManager.renderDistance / 2) * 16 - 8) {
		for (int i = 0; i < chunkManager.renderDistance + chunkManager.preGenNumber * 2; i++) {
			chunkManager.pregenerateChunkData(chunkManager.precalculatedBottomLeftChunkX - 1, chunkManager.precalculatedBottomLeftChunkZ + i);
		}
		chunkManager.precalculatedBottomLeftChunkX--;
	}
}