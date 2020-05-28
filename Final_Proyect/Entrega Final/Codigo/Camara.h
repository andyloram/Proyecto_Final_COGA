#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float VELOCIDAD = 12.0f;
const float SENSIBILIDADRATON = 0.1f;
const float ZOOM = 45.0f;

enum MOV_CAMARA {DELANTE, ATRAS, IZQUIERDA, DERECHA};

class Camara 
{
public:
	glm::vec3 Posicion; //Posicion de la cámara
	glm::vec3 Frente; //Vector que determina hacia donde apunta la cámara
	glm::vec3 Arriba; //Último parámetro del lookAt, nos indica la 'orientación' de la cámara, asociado al eje de las Y en es espacio de la cámara
	glm::vec3 Derecha; //vector asociado a eje de las X  en el espacio de la cámara
	glm::vec3 MundoArriba; 
	
	//Ángulos de Euler
	float Yaw; //Eje de la Y, colatitud
	float Pitch; //Eje de la Z, azimutal
	
	//Opciones
	float VelocidadMov;
	float SensibilidadRaton;
	float Zoom;

	Camara(glm::vec3 posicion , glm::vec3 arriba = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) 
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

	bool ProcesarTeclado(MOV_CAMARA direccion, float deltaTime)
	{
		bool a=true;
		float posY = Posicion.y;
		glm::vec3 posicionAnt = Posicion;

		float velocidad = VelocidadMov * deltaTime;
		if (direccion == DELANTE)
			Posicion += Frente * velocidad;
		else if (direccion == ATRAS)
			Posicion -= Frente * velocidad;
		else if (direccion == DERECHA)
			Posicion += Derecha * velocidad;
		else if (direccion == IZQUIERDA)
			Posicion -= Derecha * velocidad;

		//Condición para no salir del cuadrado
		if (glm::abs(Posicion.x) > 16.5f || glm::abs(Posicion.z) > 15)
		{
			Posicion = posicionAnt;
			a = false;
		}

		Posicion.y = posY;
		return a;
	}
	void ProcesarRaton(float xoffset, float yoffset, bool restriccionPitch = true)
	{
		xoffset *= SensibilidadRaton;
		yoffset *= SensibilidadRaton;

		Yaw = glm::mod(Yaw + xoffset, 360.0f);
		Pitch += yoffset;

		//Si se indica, para evitar que la cámara se voltee verticalmente, utilizamos la siguiente restricción
		if (restriccionPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}
		actualizarVectoresCam();
	}

private:
	void actualizarVectoresCam()
	{
		glm::vec3 frente;
		frente.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		frente.y = sin(glm::radians(Pitch));
		frente.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Frente = glm::normalize(frente);
		Derecha = glm::normalize(glm::cross(Frente, MundoArriba));
		Arriba = glm::normalize(glm::cross(Derecha, Frente));

	}

};