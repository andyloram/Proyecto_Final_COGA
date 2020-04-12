#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float VELOCIDAD = 2.5f;
const float SENSIBILIDADRATON = 0.1f;
const float ZOOM = 45.0f;

enum MOV_CAMARA {DELANTE, ATRAS, IZQUIERDA, DERECHA};

class Camara 
{
public:
	glm::vec3 Posicion; //Posicion de la c�mara
	glm::vec3 Frente; //Vector que determina hacia donde apunta la c�mara
	glm::vec3 Arriba; //�ltimo par�metro del lookAt, nos indica la 'orientaci�n' de la c�mara, asociado al eje de las Y en es espacio de la c�mara
	glm::vec3 Derecha; //vector asociado a eje de las X  en el espacio de la c�mara
	glm::vec3 MundoArriba; 
	//�ngulos de Euler
	float Yaw; //Eje de la Y, colatitud
	float Pitch; //Eje de la Z, azimutal
	//Opciones
	float VelocidadMov;
	float SensibilidadRaton;
	float Zoom;

	Camara(glm::vec3 posicion = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 arriba = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) 
		: Frente(glm::vec3(0.0f, 0.0f, -1.0f)), VelocidadMov(VELOCIDAD), SensibilidadRaton(SENSIBILIDADRATON), Zoom(ZOOM)
	{
		Posicion = posicion;
		MundoArriba = arriba;
		Yaw = yaw;
		Pitch = pitch;
		actualizarVectoresCam();
	}

	glm::mat4 getViewMat() 
	{
		return glm::lookAt(Posicion, Posicion + Frente, Arriba); 
	}

	void ProcesarTeclado(MOV_CAMARA direccion, float deltaTime)
	{
		float velocidad = VelocidadMov * deltaTime;
		if (direccion == DELANTE)
			Posicion += Frente * velocidad;
		if (direccion == ATRAS)
			Posicion -= Frente * velocidad;
		if (direccion == DERECHA)
			Posicion += Derecha * velocidad;
		if (direccion == IZQUIERDA)
			Posicion -= Derecha * velocidad;
	}
	void PocesarRaton(float xoffset, float yoffset, bool restriccionPitch = true)
	{
		xoffset *= SensibilidadRaton;
		yoffset *= SensibilidadRaton;

		Yaw += xoffset;
		Pitch += yoffset;

		//Si se indica, para evitar que la c�mara se voltee verticalmente, utilizamos la siguiente restricci�n
		if (restriccionPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}
		actualizarVectoresCam();
	}

	void procesarScroll(float yoffset) 
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
		if (Pitch <= 1.0f)
			Pitch = 1.0f;
	}

private:
	void actualizarVectoresCam()
	{
		glm::vec3 frente;
		frente.x = cos(glm::radians(Yaw)) * sin(glm::radians(Pitch));
		frente.y = sin(glm::radians(Pitch));
		frente.z = sin(glm::radians(Yaw)) * sin(glm::radians(Pitch));
		Frente = glm::normalize(frente);
		Derecha = glm::normalize(glm::cross(Frente, MundoArriba));
		Arriba = glm::normalize(glm::cross(Derecha, Frente));

	}

};