enum tipo {VIVO, DESCENSO, MUERTO, STANDBY};
class Virus {
public:
	glm::vec3 posicion;
	Model modelo;
	float size;
	float radio;
	float velocidad;
	float velocidadCaida;
	int estado = VIVO;
	Virus(Model& model, float vel = 2.5f, float s = 0.071, float r = 6.0f, float v = 5)
		: modelo(model), size(s), velocidad(vel), estado(STANDBY), velocidadCaida(v)
	{
		radio = r * size;
		recalcularPosiciones();
	}
	glm::mat4 Colocar(float deltaTime, glm::vec3 posTarget)
	{
		glm::mat4 model = glm::mat4(1.0f);
		if (estado == VIVO) {
			
			model = glm::translate(model, posicion);
			model = glm::scale(model, glm::vec3(size));
			if (deltaTime >= 0)
				posicion = posicion + velocidad * deltaTime * glm::normalize(posTarget - posicion);
		}
		if (estado == DESCENSO)
		{
			
			model = glm::translate(model, posicion);
			model = glm::scale(model, glm::vec3(size));
			if (posicion.y <= -2.0f)
			{
				velocidadCaida = 5;
				estado = MUERTO;
			}
			if (deltaTime >= 0)
			{
				posicion = posicion + velocidadCaida * deltaTime * glm::vec3(0.0f, -1.0f, 0.0f);
				velocidadCaida *= 1.02;
			}
		}
		return model;
	}
	void dibujar(Shader shader)
	{
		if (estado!=MUERTO && estado!=STANDBY)
			modelo.Draw(shader);
	}
	void recalcularPosiciones()
	{
		posicion.x = -50 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (50 + 50)));
		while (glm::abs(posicion.x) < 20)
		{
			posicion.x = -50 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (50 + 50)));
		}
		posicion.y = 10 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (40 - 10)));
		posicion.z = -50 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (50 + 50)));
		while (glm::abs(posicion.z) < 10)
		{
			posicion.z = -50 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (50 + 50)));
		}
	}
		 


};
