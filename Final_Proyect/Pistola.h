#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

enum partesPistola{CUERPO, CARGADOR, GATILLO_TRASERO, GATILLO};
enum modos {TERCERA, APUNTADO};
class Pistola
{
public:
	glm::vec3 escalado;
	glm::vec3 posicion1;
	glm::vec3 posicion2;
	glm::vec3 diferencia;
	Model modeloPistola;
	float radio;
	int modo;

	//Variables cambiar apuntado
	float escala = 0.0f;
	float velEscala = 0.8f;

	//variables animarCargador
	float anguloCargador = 0.0f;
	float contadorCargador = 0.0f;


	float anguloGatillo;
	
	float anguloPistola;
	float random = 0;

	Pistola(glm::vec3 posiciona, glm::vec3 posicionb, glm::vec3 scale, Model model, float r=20)
		:posicion1(posiciona), posicion2(posicionb), escalado(scale), modeloPistola(model), modo(TERCERA), radio(r)
	{
		radio = r * scale.x;
		diferencia = posicion2 - posicion1;
	}

	void cambiarApuntado(float deltaTime)
	{
		float velescala = 5;
		if (modo == APUNTADO && escala < 1)
		{
			escala += velescala * deltaTime;
			if (escala > 1)
				escala = 1;
		}
		if (modo == TERCERA && escala > 0)
		{
			velescala = -velescala;
			escala += velescala * deltaTime;
			if (escala < 0)
				escala = 0;
		}
	}

	glm::mat4 animarCargador(bool& anim_Cargador,float deltaTime, glm::mat4 model)
	{
		float velocidadAnimacion = 45;
		float maxAngulo = 60;
		if (anim_Cargador == true)
		{
			contadorCargador += velocidadAnimacion * deltaTime;
			if (contadorCargador >= maxAngulo)
			{
				
				anim_Cargador = false;
				contadorCargador = 0.0f;
			}
		}
		if (anim_Cargador == true)
		{
			anguloCargador += velocidadAnimacion * deltaTime;
			anguloCargador = glm::mod(anguloCargador, 360.0f);
		}
			model = glm::translate(model, glm::vec3(0, 4.59, 0));
			model = glm::rotate(model, glm::radians(anguloCargador), glm::vec3(0, 0, 1));
			model = glm::translate(model, glm::vec3(0, -4.59, 0));
			
		
			
		return model;
	}

	glm::mat4 animarGatillo(bool& anim_Gatillo, float deltaTime, glm::mat4 model)
	{
		static bool vuelta;
		float velocidadAnimacion = 55;
		float anguloMaximo=30;
		if(vuelta)
			velocidadAnimacion = -velocidadAnimacion;

		if (anim_Gatillo == true)
		{

			if (anguloGatillo > anguloMaximo)
			{
				anguloGatillo = anguloMaximo;
				velocidadAnimacion = -velocidadAnimacion;
				vuelta = true;
			}
			if (anguloGatillo < 0)
			{
				anguloGatillo = 0;
				anim_Gatillo = false;
				vuelta = false;
			}

		}
		if (anim_Gatillo == true)
		{	
			anguloGatillo += velocidadAnimacion * deltaTime;
			model = glm::translate(model, glm::vec3(0, 6.04, 11.1));
			model = glm::rotate(model, glm::radians(anguloGatillo), glm::vec3(1, 0, 0));
			model = glm::translate(model, glm::vec3(0, -6.04, -11.1));
		}

		return model;
	}
	glm::mat4 animarRetroceso(bool& anim_Cuerpo, float deltaTime, glm::mat4 model)
	{
		static bool vuelta;
		float velocidadAnimacion = 80;
		float anguloMaximo = 15;
		if (vuelta)
			velocidadAnimacion = -velocidadAnimacion;

		if (anim_Cuerpo == true)
		{

			if (anguloPistola > anguloMaximo)
			{
				anguloPistola = anguloMaximo;
				velocidadAnimacion = -velocidadAnimacion;
				vuelta = true;
			}
			if (anguloPistola < 0)
			{
				anguloPistola = 0;
				anim_Cuerpo = false;
				vuelta = false;
				random = static_cast <float> (rand() / (static_cast <float> (RAND_MAX / 0.2f))) - 0.1f;
			}

		}
		if (anim_Cuerpo == true)
		{
			anguloPistola += velocidadAnimacion * deltaTime;
			model = glm::translate(model, glm::vec3(0, 0.604, 1.11));
			model = glm::rotate(model, glm::radians(anguloPistola), glm::vec3(1, random, 0));
			model = glm::translate(model, glm::vec3(0, -0.604, -1.11));
		}

		return model;
	}
		
	void dibujarParte(enum partesPistola a, Shader s)
	{
		modeloPistola.meshes[a].Draw(s);
	}
	


	glm::vec3 obtenerPosicionReal() 
	{
			return  posicion1 + escala * diferencia;
	}

};