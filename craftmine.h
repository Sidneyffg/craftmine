#ifndef CRAFTMINE_CLASS_H
#define CRAFTMINE_CLASS_H

#include "ChunkManager.h"
#include <vector>
//#include <glm/glm.hpp> included by Camera.h
//#include <GLFW/glfw3.h> included by Camera.h
#include "Camera.h"
#include <ctime>
#include <cmath>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


class Craftmine
{
public:


	Craftmine(unsigned short int renderDistance, unsigned int seed, GLFWwindow* window);

	void generateStartChunks();

	void generateVertices();

	bool loadSideChunks(glm::vec3 camPos);

	void changeRenderDistance(unsigned short int newRenderDistance, glm::vec3 camPos);

	void tp(float x, float y, float z, Camera* camera);

	std::vector<float> allChunkVertices;
	std::vector<float> newAllChunkVertices;
	std::vector<unsigned int> allChunkIndices;
	std::vector<unsigned int> newAllChunkIndices;

	void checkForChunksToLoad(glm::vec3 camPos);

private:
	ChunkManager chunkManager;
	void reloadChunks(glm::vec3 camPos);

	void loadTopChunks();
	void loadRightChunks();
	void loadBottomChunks();
	void loadLeftChunks();


	GLFWwindow* window;

	const unsigned short int CUBE_INDICES[6] = {
		0,1,2,
		0,2,3
	};
};

#endif