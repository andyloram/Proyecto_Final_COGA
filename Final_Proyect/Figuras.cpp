#include "Figuras.h"
unsigned int cargarPunto()
{
	unsigned int puntoVAO, puntoVBO;
	float punto[] = { 0.0f,0.0f,-3.0f };
	glGenVertexArrays(1, &puntoVAO);
	glGenBuffers(1, &puntoVBO);
	glBindVertexArray(puntoVAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(punto), &punto, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	return puntoVAO;
}
unsigned int cargarMiniMapaRect()
{
	float quadVertices[] = {
		0.7f,  1.0f,  0.0f, 0.9f,
		0.7f,  0.7f,  0.0f, 0.0f,
		 1.0f,  0.7f,  1.0f, 0.0f,

		0.7f,  1.0f,  0.0f, 0.9f,
		 1.0f,  0.7f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 0.9f
	};
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	return quadVAO;
}
unsigned int cargarCuadrado()
{
	float cuadrado[] = {
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f,  -1.0f,  0.0f, 0.0f,
	 1.0f,  -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f,  -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};
	unsigned int cuadradoVAO, cuadradoVBO;
	glGenVertexArrays(1, &cuadradoVAO);
	glGenBuffers(1, &cuadradoVBO);
	glBindVertexArray(cuadradoVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cuadradoVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cuadrado), &cuadrado, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	return cuadradoVAO;
}