#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <math.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Shader.h"
#include "Camara.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Model.h"
#include "Pistola.h"
#include "Bala.h"
#include "CoronaVirus.h"
#include "Skybox.h"
#include "Figuras.h"
#include <ft2build.h>
#include FT_FREETYPE_H  
#include "Glifos.h"
#include <includeSound/irrKlang.h>
using namespace irrklang;
ISoundEngine* SoundEngine = createIrrKlangDevice();
ISoundSource* ambiente = SoundEngine->addSoundSourceFromFile("./Sounds/ambiente.mp3");
ISound* pisada=SoundEngine->play2D("./Sounds/footsteps.mp3", true, true, true);
ISound* inicio = SoundEngine->play2D("./Sounds/inicio.mp3", true, true, true);
ISound* sonidoAmbiente;
const unsigned int MAX_CORONA = 7;

const float ZOOM_TERCERA = 45.0f;
const float ZOOM_PRIMERA = 20.0f;

const unsigned int SCR_WIDTH = 2000;
const unsigned int SCR_HEIGHT = 2000;

enum estadoJuego{INICIO, JUGANDO, VICTORIA, DERROTA};
enum estadoRonda{START, EN_CURSO};

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
bool esferaesfera(float radioA, glm::vec3 centroA, float radioB, glm::vec3 centroB);
bool esferaesfera2(float radioA, glm::vec3 centroA, float radioB, glm::vec3 centroB, glm::vec3 direccion, float velocidad, float deltaTime);


float reproducirPisada = true;
float desplazamientoInicial = 3.5f;
float desplazamientoBala = desplazamientoInicial;
Camara camara(glm::vec3(0.0f, 0.0f, desplazamientoInicial));

//Variables Camara
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool primerRaton = true;
int camMode = TERCERA;

//Vector de las balas
std::vector<Bala*> municion;

//Timing 
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

//Variables del estado del juego
int estado = INICIO;
bool inicializar = false;
float tiempoRonda = glfwGetTime();
int estadoRonda = START;
bool ANIMACIONES[3] = { false,false,false };
bool noche = false;
bool playNextRound = true;

void openGlInit() {
	glClearDepth(1.0f); //Valor z-buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // valor limpieza buffer color
	glEnable(GL_DEPTH_TEST); // z-buffer
	glEnable(GL_CULL_FACE); //ocultacion caras back
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void resetear(vector<Virus>& virus)
{
	for (int i = 0; i < virus.size(); i++)
	{
		virus[i].estado = STANDBY;
		virus[i].recalcularPosiciones();
	}
}

int main()
{
	
	srand(static_cast <unsigned> (time(0)));
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creo la ventana

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Simon Scape", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Fallo Crear Ventana" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// glad: 
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	openGlInit();
	glEnable(GL_PROGRAM_POINT_SIZE);

	//---------------------------------------------
	//	 CREAMOS FRAMEBUFFER PARA EL MINIMAPA
	//	Solo nos interesa guardar la textura de color
	//---------------------------------------------

	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); 																					 
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	unsigned int textVAO;
	unsigned int textVBO;
	cargarFuente();
	cargarVAOs(textVAO, textVBO);

	//---------------------------------------------
	//			CARGAMOS TEXTURAS PARA EL GUI
	//---------------------------------------------

	unsigned int texturaInicio = TextureFromFile("texturaInicio.png","./Resources/TexturasGUI", true);
	unsigned int texturaVictoria = TextureFromFile("texturaVictoria.png", "./Resources/TexturasGUI", true);
	unsigned int texturaDerrota = TextureFromFile("texturaDerrota.png", "./Resources/TexturasGUI",true);
	unsigned int texturaInicioRonda = TextureFromFile("inicioDeRonda.png", "./Resources/TexturasGUI", true);

	//Caras del Skybox
	std::vector<std::string> caras
	{
		"./images/skybox/right.jpg",
			"./images/skybox/left.jpg",
			"./images/skybox/top.jpg",
			"./images/skybox/bottom.jpg",
			"./images/skybox/front.jpg",
			"./images/skybox/back.jpg"
	};

	//Diferentes VAOs para el punto de la mira, el mini mapa, cuadrado para las texturas del gui y avisos y el cubo del skybox

	unsigned int puntoVAO = cargarPunto();
	
	unsigned int minimapaVAO = cargarMiniMapaRect();

	unsigned int cuadradoVAO = cargarCuadrado();

	unsigned int texturaSkyBox = cargarCubeMap(caras);
	unsigned int skyboxVAO = cargarSkyBox();

	//Modos de apuntado en apuntado y sin apuntar
	glm::vec3 tercera = glm::vec3(.5f, -.7f, .2f);
	glm::vec3 primera = glm::vec3(.0f, -.82f, 1.0f);


	//Modelos de la escena, la isla y los añadidos de la misma se cargan en el stack
	Model modelPistola("Resources/Pistola/COLT.obj");
	Model modelEsfera("Resources/Esfera/Flecha2.obj");
	Model modelBala("Resources/Bala/9mm Regular.obj");
	Model modelCorona("Resources/Corona/corona.obj");
	Model modelLinterna("Resources/Linterna/linterna.obj");
	Model* modelCasa =new Model("Resources/Isla/isla.obj");
	Model* modelCasaAdd = new Model("Resources/Props/props.obj");

	//Creamos la pistola y el vector de virus
	Pistola pistol(tercera, primera, glm::vec3(.09f), modelPistola);
	std::vector<Virus> corona(MAX_CORONA, Virus(modelCorona));

	//Diferentes shaders de la escena
	Shader shaderSkybox("./shaders/shaderSkybox.vert", "./shaders/shaderSkybox.frag");
	Shader shaderGUI("./shaders/shaderGUI.vert", "./shaders/shaderGUI.frag");
	Shader shaderPunto("./shaders/shaderPunto.vert", "./shaders/shaderPunto.frag");
	Shader normalShader("./shaders/shaderNormal.vert", "./shaders/shaderNormal.frag");
	Shader esferaShader("./shaders/shaderEsferas.vert", "./shaders/shaderEsferas.frag");
	Shader minimapShader("./shaders/ShaderMinimap.vert", "./shaders/ShaderMinimap.frag");
	Shader shaderPeligro("./shaders/shaderAlerta.vert", "./shaders/shaderAlerta.frag");
	Shader shaderTexto("./shaders/shaderTexto.vert", "./shaders/shaderTexto.frag");

	glm::mat4 projectionText = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
	shaderTexto.use();
	shaderTexto.setMat4("projection", projectionText);

	//Inicializamos las variables de textura en el minimapa y el skybox
	minimapShader.use();
	minimapShader.setInt("textura_minimap", 0);
	shaderSkybox.use();
	shaderSkybox.setInt("skybox", 0);

	//Matrices
	glm::mat4 view;
	glm::mat4 model;
	glm::mat4 projection;
	glm::mat2 textura;

	glm::mat4 projectionPistola = glm::perspective(glm::radians(ZOOM_TERCERA), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	//Variables de la luz linterna y sol
	normalShader.use();
	normalShader.setVec3("linterna.ambiente", glm::vec3(0.1f));
	normalShader.setVec3("linterna.difusa", glm::vec3(0.8f));
	normalShader.setVec3("linterna.especular", glm::vec3(1.0f));
	normalShader.setFloat("linterna.constant", .5f);
	normalShader.setFloat("linterna.linear", 0.009f);
	normalShader.setFloat("linterna.quadratic", 0.0032f);
	normalShader.setFloat("linterna.cutOff", glm::cos(glm::radians(15.0f)));
	normalShader.setFloat("linterna.outerCutOff", glm::cos(glm::radians(20.0f)));
	normalShader.setVec3("sol.direccion", -0.2f, -2.0f, 1.0f);
	normalShader.setVec3("sol.ambiente", glm::vec3(0.2f));
	normalShader.setVec3("sol.difusa", glm::vec3(0.8f));
	normalShader.setVec3("sol.especular", glm::vec3(0.6f));

	
	bool peligro=false;
	float transparencia = 0;
	float tiempoAlerta = 0;
	int ronda = 0;
	int max_distancia_bala = 60;
	desplazamientoBala += pistol.radio;
	int resultadoRonda;	
	ISoundSource* nextRoundS = SoundEngine->addSoundSourceFromFile("./Sounds/NextRound.mp3");
	ISoundSource* alarmS = SoundEngine->addSoundSourceFromFile("./Sounds/alarma.mp3");
	ambiente->setDefaultVolume(0.4f);
	alarmS->setDefaultVolume(0.3f);
	nextRoundS->setDefaultVolume(0.2f);
	ISound* sonidoAlarma = SoundEngine->play2D(alarmS, true, true, true);
	inicio->setVolume(0.05f);
	inicio->setIsPaused(false);
	while (!glfwWindowShouldClose(window))
	{

		processInput(window);
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (inicializar)
		{
			inicializar = false;
			corona[0].estado = VIVO;
		}

		//Dibujamos la escena en el FrameBuffer que creamos anteriormente para guardarlo en una textura y usarla como minimapa
		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		//look at para mirar desde arriba siguiendo la posicion del jugador
		view = glm::lookAt(glm::vec3(camara.Posicion.x, camara.Posicion.y + 80, camara.Posicion.z), camara.Posicion, glm::vec3(0.0, 0.0, -1.0));
		projection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 0.1f, 100.0f);

		//Varables de la camara y modo
		normalShader.use();
		normalShader.setMat4("view", view);
		normalShader.setBool("noche", false);
		normalShader.setMat4("projection", projection);
		normalShader.setFloat("shininess", 32.0f);

		//Isla
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0, -6.0, 0.0));
		normalShader.setMat4("model", model);

		textura = glm::mat2(10.0f);
		normalShader.setMat2("textureTrans", textura);
		modelCasa->Draw(normalShader);

		//Anadidos de la isla
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0, -6.0, 0.0));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		normalShader.setMat4("model", model);

		textura = glm::mat2(1.0f);
		normalShader.setMat2("textureTrans", textura);
		modelCasaAdd->Draw(normalShader);

		//Coronavirus como puntos rojos
		model = glm::mat4(1.0f);
		esferaShader.use();
		esferaShader.setMat4("view", view);
		esferaShader.setMat4("projection", projection);
		esferaShader.setFloat("color", 0);
		for (int i = 0; i < corona.size(); i++)
		{
			if (corona[i].estado == VIVO)
			{
				model = glm::mat3(1.0f);
				model = glm::translate(model, corona[i].posicion);
				model = glm::scale(model, glm::vec3(corona[i].radio));
				esferaShader.setMat4("model", model);
				modelEsfera.Draw(esferaShader);
			}
		}
		//Jugador
		model = glm::mat4(1.0f);
		model = glm::translate(model, camara.Posicion);
		model = glm::rotate(model, glm::radians(-camara.Yaw-90.0f), glm::vec3(0.0, 1, 0.0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0, 0, 0.0));
		model = glm::scale(model, glm::vec3(pistol.radio + glm::length(tercera)));
		esferaShader.setMat4("model", model);
		esferaShader.setFloat("color", 1);
		modelEsfera.Draw(esferaShader);
		
		
		//Escena real
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Dependiendo si apuntamos o no pues la perspectiva es diferente (zoom o no)
		view = camara.getViewMat();
		if (camMode == TERCERA)
			projection = glm::perspective(glm::radians(ZOOM_TERCERA), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		if (camMode == APUNTADO)
			projection = glm::perspective(glm::radians(ZOOM_PRIMERA), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		//Variables de la camara y de la luz de la linterna
		normalShader.use();
		normalShader.setMat4("view", view);
		normalShader.setMat4("projection", projection);
		normalShader.setVec3("viewPos", camara.Posicion);
		normalShader.setBool("noche", noche);
		normalShader.setVec3("linterna.direccion",camara.Frente);
		normalShader.setVec3("linterna.posicion", camara.Posicion + camara.Frente * (desplazamientoInicial - 1.5f));

	
		textura = glm::mat2(1.0f);
		pistol.modo = camMode;
		//Mismos giros de la camara para alinear balas y pistola 
		glm::mat4 alinearCam = glm::mat4(glm::mat3(camara.getViewMat()));
		normalShader.setMat2("textureTrans", textura);


	//---------------------------------------------
	//					Cesped
	//---------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0, -6.0, 0.0));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		normalShader.setMat4("model", model);

		textura = glm::mat2(10.0f);
		normalShader.setMat2("textureTrans", textura);
		modelCasa->Draw(normalShader);

	//---------------------------------------------
	//					CASA etc.
	//---------------------------------------------

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0, -6.0, 0.0));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		normalShader.setMat4("model", model);

		textura = glm::mat2(1.0f);
		normalShader.setMat2("textureTrans", textura);
		modelCasaAdd->Draw(normalShader);
		
		//---------------------------------------------
		//					BALAS
		//---------------------------------------------
			
		model = glm::mat4(1.0f);
		int distanciaCubo = 60;
		for (int i = 0; i < municion.size(); i++)
		{
			if (glm::abs(municion[i]->posicion.x) < distanciaCubo && glm::abs(municion[i]->posicion.y) < distanciaCubo && glm::abs(municion[i]->posicion.z) < distanciaCubo)
			{
				model = municion[i]->Colocar(deltaTime);
				normalShader.setMat4("model", model);
				modelBala.Draw(normalShader);
			}
			else
			{
				delete municion[i];
				municion.erase(municion.begin() + i);
				i--;
			}
		}
		
		
		//---------------------------------------------
		//				Corona
		//---------------------------------------------
		model = glm::mat4(1.0f);
		int cont=0;
		//Chekeo si las balas chocan con los virus
		for (int i = 0; i < corona.size(); i++)
		{
			if (corona[i].estado == VIVO)
			{
				for (int j = 0; j < municion.size(); j++)
				{
					if (esferaesfera2(municion[j]->radio, municion[j]->posicionReal, corona[i].radio, corona[i].posicion, municion[j]->direccion, municion[j]->velocidad, deltaTime))
					{
						SoundEngine->play2D("./Sounds/hitmarker.mp3", false);
						corona[i].estado = DESCENSO;
					}
				}
			}
			//Comprobamos si los virus chocan con el jugador
			if (esferaesfera(pistol.radio+desplazamientoInicial, camara.Posicion, corona[i].radio, corona[i].posicion))
			{
				peligro = true;
				sonidoAlarma->setIsPaused(false);
				if (esferaesfera(pistol.radio, camara.Posicion, corona[i].radio, corona[i].posicion))
				{
					estado = DERROTA;
					if (sonidoAmbiente)
					{
						sonidoAmbiente->stop();
						sonidoAmbiente->drop();
						sonidoAmbiente = 0;
					}
					if (sonidoAlarma)
					{
						sonidoAlarma->stop();
						sonidoAlarma->drop();
						sonidoAlarma = SoundEngine->play2D(alarmS, true, true, true);
	
					}
					SoundEngine->play2D("./Sounds/death.mp3", false, false, true);
					resetear(corona);
					resultadoRonda = ronda;
					ronda = 0;
				}
			}
			else
			{
				cont++;
			}
			model = corona[i].Colocar(deltaTime, camara.Posicion);
			normalShader.setMat4("model", model);
			corona[i].dibujar(normalShader);
		}
		if (cont == corona.size())
		{
			peligro = false;
			sonidoAlarma->setIsPaused(true);
		}
		int contador = 0;
		for (int i = 0; i < corona.size(); i++)
		{
			if (corona[i].estado == MUERTO) 
			{
				contador++;
			}
		}
		if (contador == ronda+1) 
		{
			ronda++;
			for (int i = 0; i < corona.size(); i++) 
			{
				if(corona[i].estado == MUERTO)
				{
					corona[i].estado = VIVO;
					corona[i].recalcularPosiciones();
					estadoRonda = START;
					tiempoRonda = glfwGetTime();
					playNextRound = true;
				}
				if (ronda < corona.size())
					corona[ronda].estado = VIVO;
				else
				{
					estado = VICTORIA;
					SoundEngine->play2D("./Sounds/Victory.mp3", false);
					resetear(corona);
					resultadoRonda = ronda-1;
					ronda = 0;
				}

			}
		}
		

		//PUNTO
		//Simpre dibujamos el punto
		shaderPunto.use();
		glDepthFunc(GL_ALWAYS);
		glBindVertexArray(puntoVAO);
		float puntoTam = 3.0f;
		model = glm::mat4(1.0);
		model = glm::translate(model, camara.Posicion + camara.Frente * desplazamientoInicial);
		shaderPunto.setMat4("model", model);
		view = camara.getViewMat();
		shaderPunto.setMat4("view", view);
		shaderPunto.setMat4("projection", projection);
		shaderPunto.setFloat("puntoTam", puntoTam);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		//---------------------------------------------
		//					SkyBox
		//---------------------------------------------
		//Si es de dia pinto el skybox si es de noche queda negro todo, no hubo otras imágenes de cielo norcturno que crearan el mismo efecto como de horror 
		//por lo que decidí que quedaba mejor así
		if (!noche)
		{
			glDepthFunc(GL_LEQUAL);
			shaderSkybox.use();
			view = glm::mat4(glm::mat3(camara.getViewMat()));
			shaderSkybox.setMat4("view", view);
			shaderSkybox.setMat4("projection", projection);
			glBindVertexArray(skyboxVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, texturaSkyBox);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glDepthFunc(GL_LESS); 
		}

		//---------------------------------------------
		//		Pistola y Linterna si es de noche
		//---------------------------------------------
		//Para la pistola como quiero quese pinten todas sus partes en orden y delante de todo lo que haya limpio el buffer de profundidad para que haya lo que haya lo pinte

		glClear(GL_DEPTH_BUFFER_BIT);
		normalShader.setFloat("shininess", 62.0f);

		normalShader.use();
		normalShader.setMat4("projection", projectionPistola);

		model = glm::mat4(1.0f);
		pistol.cambiarApuntado(deltaTime);
		model = glm::translate(model, camara.Posicion + camara.Frente * desplazamientoInicial);
		model = model * glm::transpose(alinearCam);
		model = pistol.animarRetroceso(ANIMACIONES[CUERPO], deltaTime, model);
		model = glm::translate(model, pistol.obtenerPosicionReal());

		glm::mat4 transformaux = model;

		model = glm::scale(model, pistol.escalado);

		normalShader.setMat4("model", model);
		pistol.dibujarParte(CUERPO, normalShader);


		model = transformaux;
		model = glm::scale(model, pistol.escalado);
		model = pistol.animarCargador(ANIMACIONES[CARGADOR], deltaTime, model);



		normalShader.setMat4("model", model);
		pistol.dibujarParte(CARGADOR, normalShader);

		model = transformaux;
		model = glm::scale(model, pistol.escalado);
		model = pistol.animarGatillo(ANIMACIONES[GATILLO_TRASERO], deltaTime, model);


		normalShader.setMat4("model", model);
		pistol.dibujarParte(GATILLO_TRASERO, normalShader);

		//Si es de noche dibujamos la linterna
		if (noche)
		{
			if (pistol.escala < 0.5f)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, camara.Posicion + camara.Frente * desplazamientoInicial);
				model = model * glm::transpose(alinearCam);
				model = glm::translate(model, glm::vec3(-0.3, -0.4, 2));
				model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0, 1, 0));

				model = glm::scale(model, glm::vec3(0.7));
				normalShader.setMat4("model", model);
				modelLinterna.Draw(normalShader);
			}
		}
		//---------------------------------------------
		//	Si el coronavirus está cerca empezamos pintndo de rojo la pantalla
		//---------------------------------------------
		if (peligro == true)
		{
			if (glm::abs(currentFrame - tiempoAlerta) > 0.25f)
			{
				transparencia += 0.02f;
				if (transparencia > 0.7f)
				{
					transparencia = 0;
				}

				tiempoAlerta = glfwGetTime();
			}
			shaderPeligro.use();
			shaderPeligro.setFloat("transparencia", transparencia);
			glBindVertexArray(cuadradoVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}


		//---------------------------------------------
		//					Minimapa
		//---------------------------------------------
		glDisable(GL_DEPTH_TEST);
		minimapShader.use();
		glBindVertexArray(minimapaVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		if (estado != JUGANDO)
		{
			shaderGUI.use();
			shaderGUI.setBool("derrota", false);
			glBindVertexArray(cuadradoVAO);
			glActiveTexture(GL_TEXTURE0);
			if (estado == INICIO)
				glBindTexture(GL_TEXTURE_2D, texturaInicio);
			if (estado == VICTORIA)
				glBindTexture(GL_TEXTURE_2D, texturaVictoria);
			if (estado == DERROTA)
			{
				glBindTexture(GL_TEXTURE_2D, texturaDerrota);
				shaderGUI.setBool("derrota", true);
			}
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		if (estado == JUGANDO && estadoRonda==START && glm::abs(currentFrame - tiempoRonda)<2.5f)
		{
			if (playNextRound)
			{
				SoundEngine->play2D(nextRoundS, false);
				playNextRound = false;
			}
			shaderGUI.use();
			shaderGUI.setBool("derrota", false);
			glBindVertexArray(cuadradoVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texturaInicioRonda);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}
		else
		{
			estadoRonda = EN_CURSO;
		}
		glEnable(GL_DEPTH_TEST);
		std::string outPutFinal;
		if (estado == JUGANDO || estado==INICIO)
		{
			std::string OutpuRonda = "   Ronda: " + std::to_string(ronda+1) + "/" + std::to_string(MAX_CORONA);
			std::string OutputCororna = "   Coronavirus: " + std::to_string(ronda + 1 - contador) + "/" + std::to_string(ronda + 1);
			outPutFinal = OutpuRonda + " " + OutputCororna;
		}
		if (estado == VICTORIA || estado == DERROTA) 
		{
			outPutFinal = "   Has sobrevivido " + std::to_string(resultadoRonda+1) + " rondas" ;
		}
		glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
		if(noche)
			color = glm::vec3(1.0f, 1.0f, 1.0f);
		RenderText(shaderTexto, outPutFinal, 1.0f, 29*SCR_HEIGHT/30, 1.5f, color, textVAO, textVBO);



	// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete modelCasa;
	delete modelCasaAdd;
	glDeleteBuffers(1, &puntoVAO);
	glDeleteBuffers(1, &minimapaVAO);
	glDeleteBuffers(1, &cuadradoVAO);
	glDeleteBuffers(1, &skyboxVAO);
	if (pisada)
		pisada->drop();
	if (inicio)
		inicio->drop();

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{	

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		if (estado == JUGANDO)
		{
			bool sonidoPisada = false;
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				sonidoPisada=camara.ProcesarTeclado(DELANTE, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				sonidoPisada = camara.ProcesarTeclado(ATRAS, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				sonidoPisada = camara.ProcesarTeclado(IZQUIERDA, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				sonidoPisada = camara.ProcesarTeclado(DERECHA, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
				noche = false;
			if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
				noche = true;
			if (sonidoPisada)
			{
				pisada->setIsPaused(false);
			}
			else
			{
				pisada->stop();
				pisada->drop();
				pisada = SoundEngine->play2D("./Sounds/footsteps.mp3", true, true, true);
			}
			

		}
		if (estado != JUGANDO)
		{
			pisada->stop();
			pisada->drop();
			pisada = SoundEngine->play2D("./Sounds/footsteps.mp3", true, true, true);
		}
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (estado == JUGANDO)
	{
		if (primerRaton)
		{
			lastX = xpos;
			lastY = ypos;
			primerRaton = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top


		lastX = xpos;
		lastY = ypos;

		camara.ProcesarRaton(xoffset, yoffset);
	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if(estado==JUGANDO)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			camMode = camMode == TERCERA ? APUNTADO : TERCERA;
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			if (ANIMACIONES[CARGADOR] == false && ANIMACIONES[GATILLO_TRASERO] == false && ANIMACIONES[CUERPO] == false)
			{
				ANIMACIONES[CUERPO] = true;
				ANIMACIONES[CARGADOR] = true;
				ANIMACIONES[GATILLO_TRASERO] = true;

				glm::mat4 alinearCam = glm::mat4(glm::mat3(camara.getViewMat()));

				Bala* bala = new Bala(camara.Posicion, camara.Frente, alinearCam, desplazamientoBala);
				municion.push_back(bala);
				SoundEngine->play2D("./Sounds/disparo.mp3", false);
				SoundEngine->play2D("./Sounds/nextBullet.mp3", false);
			}
		}
	}
	else 
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			estado = JUGANDO;
			inicializar = true;
			estadoRonda = START;
			tiempoRonda = glfwGetTime();
			playNextRound = true;
			if (inicio)
			{
				inicio->stop();
				inicio->drop();
				inicio = 0;
			}
			sonidoAmbiente = SoundEngine->play2D(ambiente, true, false, true);
			
		}
	}

}
	float distanciaPuntos(glm::vec3 puntoA, glm::vec3 puntoB)
	{
		return glm::sqrt((puntoA.x - puntoB.x) * (puntoA.x - puntoB.x) + (puntoA.y - puntoB.y) * (puntoA.y - puntoB.y) + (puntoA.z - puntoB.z) * (puntoA.z - puntoB.z));
	}

	bool esferaesfera(float radioA, glm::vec3 centroA, float radioB, glm::vec3 centroB)
	{
		if (distanciaPuntos(centroA, centroB) <= radioA + radioB)
			return true;
		else
			return false;
	}

	bool esferaesfera2(float radioA, glm::vec3 centroA, float radioB, glm::vec3 centroB, glm::vec3 direccion, float velocidad, float deltaTime)
	{
		float suma = 0;
		glm::vec3 centroAaux = centroA;
		while (suma <= velocidad*deltaTime)
		{
			if (distanciaPuntos(centroA, centroB) <= radioA + radioB)
				return true;
			centroA += (radioA + radioB) * direccion;
			suma += radioA + radioB;
		}
		if (esferaesfera(radioA, centroAaux, radioB, centroB))
			return true;
		return false;
	}
		

