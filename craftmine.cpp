#include "Craftmine.h"
#include <iostream>


Craftmine::Craftmine(unsigned short int renderDistance, unsigned int seed, GLFWwindow* window) {
	Craftmine::window = window;

	chunks.init(renderDistance, seed);
}

void Craftmine::changeRenderDistance(unsigned short int newRenderDistance, glm::vec3 camPos) {
	//Craftmine::renderDistance = newRenderDistance;
	//reloadChunks(camPos);
}

void Craftmine::reloadChunks(glm::vec3 camPos) {
	chunks.chunkData.clear();
	chunks.chunkVertices.clear();
	chunks.chunkData.reserve(chunks.renderDistance);
	chunks.chunkVertices.reserve(chunks.renderDistance);
	for (int i = 0; i < chunks.renderDistance; i++) {
		chunks.chunkData.push_back(std::vector<std::vector<char>>(chunks.renderDistance, std::vector<char>(65536)));
		chunks.chunkVertices.push_back(std::vector<std::vector<float>>(chunks.renderDistance, std::vector<float>()));
	}

	chunks.bottomLeftChunkX = round(camPos[0] / 16) - chunks.renderDistance / 2;
	chunks.bottomLeftChunkZ = round(camPos[2] / 16) - chunks.renderDistance / 2;

	for (int i = 0; i < chunks.renderDistance; i++) {
		for (int j = 0; j < chunks.renderDistance; j++) {
			chunks.generateChunkData(i + chunks.bottomLeftChunkX, j + chunks.bottomLeftChunkZ, i, j);
		}
	}
	for (int i = 0; i < chunks.renderDistance; i++) {
		for (int j = 0; j < chunks.renderDistance; j++) {
			chunks.generateChunkVertices(i, j);
		}
	}
}

void Craftmine::generateStartChunks() {
	for (int i = 0; i < chunks.renderDistance; i++) {
		for (int j = 0; j < chunks.renderDistance; j++) {
			chunks.pregenerateChunkData(i + chunks.bottomLeftChunkX, j + chunks.bottomLeftChunkZ, i, j);
		}
	}
	for (int i = 0; i < chunks.renderDistance; i++) {
		for (int j = 0; j < chunks.renderDistance; j++) {
			chunks.generateChunkData(i + chunks.bottomLeftChunkX, j + chunks.bottomLeftChunkZ, i, j);
		}
	}
	for (int i = 0; i < chunks.renderDistance; i++) {
		for (int j = 0; j < chunks.renderDistance; j++) {
			chunks.generateChunkVertices(i, j);
		}
	}
}

void Craftmine::generateVertices() {
	allChunkVertices.clear();
	allChunkIndices.clear();
	std::cout << "generating vertices..." << std::endl;
	for (int i = 0; i < chunks.renderDistance; i++) {
		for (int j = 0; j < chunks.renderDistance; j++) {
			Craftmine::allChunkVertices.insert(Craftmine::allChunkVertices.end(), chunks.chunkVertices[j][i].begin(), chunks.chunkVertices[j][i].end());
		}
	}
	Craftmine::allChunkVertices.shrink_to_fit();
	int indicesSize = Craftmine::allChunkVertices.size() / 5 / 4;
	Craftmine::allChunkIndices.reserve(indicesSize * 6);
	//std::cout << "indicesSize: " << indicesSize * 6 << std::endl;
	for (int i = 0; i < indicesSize; i++) {
		for (int j = 0; j < 6; j++) {
			Craftmine::allChunkIndices.push_back(Craftmine::CUBE_INDICES[j] + i * 4);
		}
	}
}





bool Craftmine::loadSideChunks(glm::vec3 camPos) {
	if (camPos[2] > (chunks.bottomLeftChunkZ + chunks.renderDistance / 2) * 16 + 8) {
		Craftmine::loadTopChunks();
		bool response = true;
		while (response) {
			response = Craftmine::loadSideChunks(camPos);
		}
		return true;
	}
	else if (camPos[2] < (chunks.bottomLeftChunkZ + chunks.renderDistance / 2) * 16 - 8) {
		Craftmine::loadBottomChunks();
		bool response = true;
		while (response) {
			response = Craftmine::loadSideChunks(camPos);
		}
		return true;
	}
	else if (camPos[0] > (chunks.bottomLeftChunkX + chunks.renderDistance / 2) * 16 + 8) {
		Craftmine::loadRightChunks();
		bool response = true;
		while (response) {
			response = Craftmine::loadSideChunks(camPos);
		}
		return true;
	}
	else if (camPos[0] < (chunks.bottomLeftChunkX + chunks.renderDistance / 2) * 16 - 8) {
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
	for (int i = 0; i < chunks.renderDistance; i++) {
		chunks.chunkData[i].erase(chunks.chunkData[i].begin());
		chunks.chunkVertices[i].erase(chunks.chunkVertices[i].begin());
		chunks.chunkData[i].push_back(std::vector<char>(65536));
		chunks.chunkVertices[i].push_back(std::vector<float>());
	}
	chunks.bottomLeftChunkZ++;
	for (int i = 0; i < chunks.renderDistance; i++) {
		chunks.generateChunkData(chunks.bottomLeftChunkX + i, chunks.bottomLeftChunkZ + chunks.renderDistance - 1, i, chunks.renderDistance - 1);
	}
	for (int i = 0; i < chunks.renderDistance; i++) {
		chunks.generateChunkVertices(i, chunks.renderDistance - 1);
		chunks.generateChunkVertices(i, chunks.renderDistance - 2);
	}
}

void Craftmine::loadRightChunks() {
	chunks.chunkData.erase(chunks.chunkData.begin());
	chunks.chunkVertices.erase(chunks.chunkVertices.begin());
	chunks.chunkData.push_back(std::vector<std::vector<char>>(chunks.renderDistance, std::vector<char>(65536)));
	chunks.chunkVertices.push_back(std::vector<std::vector<float>>(chunks.renderDistance, std::vector<float>()));
	chunks.bottomLeftChunkX++;
	for (int i = 0; i < chunks.renderDistance; i++) {
		chunks.generateChunkData(chunks.bottomLeftChunkX + chunks.renderDistance - 1, chunks.bottomLeftChunkZ + i, chunks.renderDistance - 1, i);
	}
	for (int i = 0; i < chunks.renderDistance; i++) {
		chunks.generateChunkVertices(chunks.renderDistance - 1, i);
		chunks.generateChunkVertices(chunks.renderDistance - 2, i);
	}
}

void Craftmine::loadBottomChunks() {
	for (int i = 0; i < chunks.renderDistance; i++) {
		chunks.chunkData[i].erase(chunks.chunkData[i].begin() + chunks.renderDistance - 1);
		chunks.chunkVertices[i].erase(chunks.chunkVertices[i].begin() + chunks.renderDistance - 1);
		chunks.chunkData[i].insert(chunks.chunkData[i].begin(), std::vector<char>(65536));
		chunks.chunkVertices[i].insert(chunks.chunkVertices[i].begin(), std::vector<float>());
	}
	chunks.bottomLeftChunkZ--;
	for (int i = 0; i < chunks.renderDistance; i++) {
		chunks.generateChunkData(chunks.bottomLeftChunkX + i, chunks.bottomLeftChunkZ, i, 0);
	}
	for (int i = 0; i < chunks.renderDistance; i++) {
		chunks.generateChunkVertices(i, 0);
		chunks.generateChunkVertices(i, 1);
	}
}

void Craftmine::loadLeftChunks() {
	chunks.chunkData.pop_back();
	chunks.chunkVertices.pop_back();
	chunks.chunkData.insert(chunks.chunkData.begin(), std::vector<std::vector<char>>(chunks.renderDistance, std::vector<char>(65536)));
	chunks.chunkVertices.insert(chunks.chunkVertices.begin(), std::vector<std::vector<float>>(chunks.renderDistance, std::vector<float>()));
	chunks.bottomLeftChunkX--;
	for (int i = 0; i < chunks.renderDistance; i++) {
		chunks.generateChunkData(chunks.bottomLeftChunkX, chunks.bottomLeftChunkZ + i, 0, i);
	}
	for (int i = 0; i < chunks.renderDistance; i++) {
		chunks.generateChunkVertices(0, i);
		chunks.generateChunkVertices(1, i);
	}
}

void Craftmine::tp(float x, float y, float z, Camera* camera) {
	glm::vec3 newCamPos = glm::vec3(x, y, z);
	camera->Position = newCamPos;
	Craftmine::reloadChunks(newCamPos);
}