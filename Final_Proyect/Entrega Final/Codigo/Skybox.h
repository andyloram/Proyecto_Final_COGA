#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


unsigned int cargarSkyBox();
unsigned int cargarCubeMap(std::vector<std::string> faces);
