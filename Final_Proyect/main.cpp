#include <glad.h>
#include "Sphere.h"
#include <glfw3.h>
#include <stdio.h>
#include <math.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
unsigned int  cargaTextura(const char* path);


const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;
int Gwidth = SCR_WIDTH;
int Gheight = SCR_HEIGHT;

enum partesGrua { BASEPRINCIPAL, BASEA1, BASE1, BASEA2, BASE2, PIRAD };

void openGlInit() {
	glClearDepth(1.0f); //Valor z-buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // valor limpieza buffer color
	glEnable(GL_DEPTH_TEST); // z-buffer
	glEnable(GL_CULL_FACE); //ocultacion caras back
	glCullFace(GL_BACK);
}

int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creo la ventana

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Grua", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Fallo Crear Ventana" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// glad: 
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glfwSetKeyCallback(window, keyCallback);
	openGlInit();
	
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		

		processInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		





	// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
			}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Gwidth = width;
	Gheight = height;
}
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	std::cout << key << std::endl;
	//Mov Primera art WASD
	if (key == 68) {


	}
	if (key == 65) {
	
	}
	
	if (key == 87) {

	}
	if (key == 83) {
	
	}
	//Mov. segunda art TFGH
	if (key == 84) {
	
	}
	if (key == 71) {

	}
	if (key == 72) {
	
	}
	if (key == 70) {
	
	}
	//K,L -> Movimiento cámara en cam libre
	if (key == 76) {

	}
	if (key == 75) {

	}
	//Flechas -> Movimiento de la grúa por el espacio
	if (key == 263) {

	}
	if (key == 262) {

	}
	if (key == 265) {

	}
	if (key == 264) {

	}
	//1,2,3 -> Cámara primera segunda o tercera
	if (key == 50) {

	}
	if (key == 49) {

	}
	if (key == 51) {
	
	}

	if (key == 93) {
	
	}
	if (key == 47) {

	}

	if (key == GLFW_KEY_SPACE && action == GLFW_REPEAT) {
		std::cout << "Space Key Pressed" << std::endl;
	}
}

unsigned int  cargaTextura(const char* path) {
	unsigned int ID;
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	//stbi_set_flip_vertically_on_load(true);
	stbi_image_free(data);

	return ID;
}