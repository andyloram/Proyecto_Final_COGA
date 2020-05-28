#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
class Bala
{
public:
	glm::vec3 posicion;
	glm::vec3 posicionReal;
	glm::vec3 direccion;
	glm::mat4 orientacion;
	float escala;
	float velocidad;
	float radio;
	Bala(glm::vec3 posInicial, glm::vec3 dir, glm::mat4 orient,float desplazamiento,float vel = 55.0f, float r = 1.50, float scal=0.020) :
		direccion{dir}, velocidad{vel}, orientacion{orient},escala{scal}
	{
		radio = r * escala;
		posicion= posInicial + (desplazamiento) * direccion;
		posicionReal = posInicial;
	}
	glm::mat4 Colocar(float deltaTime)
	{
		glm::mat4 model = glm::mat4(1.0f);
		
		model = glm::translate(model, posicion);

		model = model * glm::transpose(orientacion);

		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));

		model = glm::scale(model, glm::vec3(escala));

		if (deltaTime >= 0)
		{

				posicion += velocidad * deltaTime * direccion;
				posicionReal += velocidad * deltaTime * direccion;

		}
		

		return model;
	}


};