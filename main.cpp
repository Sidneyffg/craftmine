#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <omp.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include "Texture.h"
#include "Camera.h"
#include "Craftmine.h"
#include "Time.h"
#include <map>

int width = 1920;
int height = 1080;

void makeTreeSideways() {
	int test[563] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 0, 0, 0, 0, 0, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 16, 16, 16, 0, 0, 0, 16, 16, 16, 16, 16, 0, 0, 16, 16, 99, 16, 16, 0, 0, 16, 16, 16, 16, 16, 0, 0, 0, 16, 16, 16, 0, 0, 0, 0, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 16, 16, 0, 0, 0, 0, 16, 16, 16, 0, 0, 0, 0, 16, 16, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 16, 16, 16, 16, 0, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 99, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 16, 16, 16, 16, 16, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 16, 16, 16, 0, 0, 0, 16, 16, 16, 16, 16, 0, 0, 0, 16, 16, 16, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned short int treeX = 10;
	unsigned short int treeY = 8;
	unsigned short int treeZ = 7;

	//stores blocks of the tree and chances for the leaves to spawn
	std::vector<unsigned short int> treePreset{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 0, 0, 0, 0, 0, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 16, 16, 16, 0, 0, 0, 16, 16, 16, 16, 16, 0, 0, 16, 16, 99, 16, 16, 0, 0, 16, 16, 16, 16, 16, 0, 0, 0, 16, 16, 16, 0, 0, 0, 0, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 16, 16, 0, 0, 0, 0, 16, 16, 16, 0, 0, 0, 0, 16, 16, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 16, 16, 16, 16, 0, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 99, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 16, 16, 16, 16, 16, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 16, 16, 16, 0, 0, 0, 16, 16, 16, 16, 16, 0, 0, 0, 16, 16, 16, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	std::vector<unsigned short int> spawnChance{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,100,100,100,0,0,0,100,100,100,100,100,0,0,100,100,100,100,100,0,0,100,100,100,100,100,0,0,0,100,100,100,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,100,100,100,0,0,0,0,100,100,100,0,0,0,0,100,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100,100,100,100,0,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,0,100,100,100,100,100,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,100,100,100,0,0,0,100,100,100,100,100,0,0,0,100,100,100,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	treePreset.shrink_to_fit();
	std::vector<unsigned short int> treePreset2;
	treePreset2.reserve(treePreset.size());
	std::vector<unsigned short int> spawnChance2;
	spawnChance2.reserve(spawnChance2.size());
	for (int q = 0; q < treeY; q++) {
		for (int i = 0; i < treeZ; i++) {
			for (int j = 0; j < treeX; j++) {
				treePreset2.push_back(treePreset[j * treeZ + q * treeX * treeZ + i]);
				spawnChance2.push_back(spawnChance[j * treeZ + q * treeX * treeZ + i]);
			}
		}
	}
	
	std::cout << treePreset2.size() << std::endl;
	std::cout << treePreset.size() << std::endl;

	std::ostringstream oss;

		// Convert all but the last element to avoid a trailing ","
		std::copy(treePreset2.begin(), treePreset2.end() - 1,
			std::ostream_iterator<int>(oss, ","));

		// Now add the last element with no delimiter
		oss << treePreset2.back();

	std::cout << oss.str() << std::endl;
	std::ostringstream osss;

	// Convert all but the last element to avoid a trailing ","
	std::copy(spawnChance2.begin(), spawnChance2.end() - 1,
		std::ostream_iterator<int>(osss, ","));

	// Now add the last element with no delimiter
	osss << spawnChance2.back();

	std::cout << osss.str() << std::endl;
};

int main()
{
	//makeTreeSideways();
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "CraftMine", NULL, NULL);

	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();

	glViewport(0, 0, width, height);

	Craftmine craftmine(32, 69, window);
	std::cout << "Generating chunks..." << std::endl;
	craftmine.generateStartChunks();
	std::cout << "Finished generating chunks" << std::endl;
	craftmine.generateVertices();

	Time time(true);

	GLfloat vertices2[8 * 3] = {
		-256.0f, 256.0f, 0.0f, 0.0f,0.0f, 0,1,0,
		256.0f, 256.0f, 0.0f, 0.0f,0.0f, 0,1,0,
		-256.0f, 0.0f, 0.0f, 0.0f,0.0f, 0,1,0,
	};

	GLuint indices[3] = {
		0,1,2
	};

	
	Shader shaderProgram("default.vert", "default.frag");

	Texture texture("textures/block.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	texture.texUnit(shaderProgram, "tex0", 0);

	
	// Generates Vertex Array Object
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(craftmine.allChunkVertices.data(), craftmine.allChunkVertices.size() * sizeof(float));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(craftmine.allChunkIndices.data(), craftmine.allChunkIndices.size() * sizeof(unsigned int));

	// Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);


	Camera camera(width, height, glm::vec3(0.0f, 250.0f, 0.0f));

	bool keyPressed[3] = { false, false, false };

	// Main while loop
	bool reloadVAO = false;
	omp_set_num_threads(3);
	int id;
#pragma omp parallel
	{
		id = omp_get_thread_num();
		if (id == 0) {
			while (!glfwWindowShouldClose(window)) {
				time.tick(camera.Position);
				if (reloadVAO) {
					std::cout << "reloading vao..." << std::endl;
					
					// Generates Vertex Buffer Object and links it to vertices
					VBO1.Update(craftmine.allChunkVertices.data(), craftmine.allChunkVertices.size() * sizeof(float));
					// Generates Element Buffer Object and links it to indices
					EBO1.Update(craftmine.allChunkIndices.data(), craftmine.allChunkIndices.size() * sizeof(unsigned int));
					reloadVAO = false;
				}

				//background color
				glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
				// Clean the back buffer and assign the new color to it
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				// Tell OpenGL which Shader Program we want to use
				shaderProgram.Activate();

				camera.Inputs(window, time.deltaTime());
				camera.Matrix(45.0f, 0.1f, 10000.0f, shaderProgram, "camMatrix");
				texture.Bind();
				// Bind the VAO so OpenGL knows to use it
				VAO1.Bind();
				// Draw primitives, number of indices, datatype of indices, index of indices
				glDrawElements(GL_TRIANGLES, craftmine.allChunkIndices.size(), GL_UNSIGNED_INT, 0);
				VAO1.Unbind();


				// Swap the back buffer with the front buffer
				glfwSwapBuffers(window);
				// Take care of all GLFW events
				glfwPollEvents();

			}
		}
		else if (id == 1) {
			while (!glfwWindowShouldClose(window)) {
				if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) { //min knop
					if (!keyPressed[0]) {
						keyPressed[0] = true;
						//craftmine.changeRenderDistance(craftmine.renderDistance - 1, camera.Position);
						craftmine.generateVertices();
						reloadVAO = true;
					}
				}
				else if (keyPressed[0]) {
					keyPressed[0] = false;
				}
				if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) { //plus knop
					if (!keyPressed[1]) {
						keyPressed[1] = true;
						//craftmine.changeRenderDistance(craftmine.renderDistance + 1, camera.Position);
						craftmine.generateVertices();
						reloadVAO = true;
					}
				}
				else if (keyPressed[1]) {
					keyPressed[1] = false;
				}
				if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) { //tp naar 0,0
					if (!keyPressed[2]) {
						keyPressed[2] = true;
						//craftmine.tp(0.0f, 250.0f, 0.0f, &camera);
						craftmine.generateVertices();
						reloadVAO = true;
					}
				}
				else if (keyPressed[2]) {
					keyPressed[2] = false;
				}
				if (!reloadVAO && craftmine.loadSideChunks(camera.Position)) {
					craftmine.generateVertices();
					reloadVAO = true;
				}
			}
		}
		else if (id == 2) {
			while (!glfwWindowShouldClose(window)) {
				craftmine.checkForChunksToLoad(camera.Position);
			}
		}
	}


	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	texture.Delete();

	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}