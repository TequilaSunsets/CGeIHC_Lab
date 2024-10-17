/*
Semestre 2024-1
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transforomación geométrica se ve modificada
Sesión 2
Compleja: Por medio de funciones y algoritmos.
Adicional.- ,Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <iostream>
#include <cstdlib>  // Para rand() y srand()
#include <ctime>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
double arrancar;

float lana;
float lanaOffSet;
float rot;
float rotOffSet;
float rotx;
float rotxOffSet;
float roty;
float rotyOffSet;
float elevar;
float elevarOffSet1;
float elevarOffSet2;

bool giro;


bool arab;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

Texture Dado10;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

Model Letrero_Arco;
Model Pilares_Arco;
Model Cuerpo_avion;
Model Helice_avion;
Model Tablero;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void dado10Caras() {
	unsigned int dado10CarasIndices[] = {
		0, 1, 2,
		3,4,5,
		6,7,8,
		9,10,11,
		12,13,14,
		15,16,17,
		18,19,20,
		21,22,23,
		24,25,26,
		27,28,29,
	};

	GLfloat dado10CarasVertices[] = {
		///////////////////////////////////
		// Parte Alta
		//////////////////////////////////
		//1
		//	x      y      z			u	  v			nx	  ny    nz
		-0.2938f, 0.0f, 0.4045f,	0.0f, 0.5f,		0.2938f, 0.0f, -0.4045f,
		0.2938f, 0.0f, 0.4045f,		0.33f, 0.5f,	-0.2938f, 0.0f, -0.4045f,
		0.0f, 0.5f, 0.0f,			0.16f, 1.0f,	0.0f, -0.5f, 0.0f,

		//5
		//	x      y      z			u	  v			nx	  ny    nz
		0.2938f, 0.0f, 0.4045f,		0.33f, 0.5f,	-0.2938f, 0.0f, -0.4045f,
		0.4755f, 0.0f, -0.1545f,	0.66f, 0.5f,	-0.4755f, 0.0f, 0.1545f,
		0.0f, 0.5f, 0.0f,			0.5f, 1.0f,		0.0f, -0.5f, 0.0f,

		//9
		//	x      y      z			u	  v			nx	  ny    nz
		0.4755f, 0.0f, -0.1545f,	0.67f, 0.5f,	-0.4755f, 0.0f, 0.1545f,
		0.0f, 0.0f, -0.5f,			0.99f, 0.5f,	0.0f, 0.0f, 0.5f,
		0.0f, 0.5f, 0.0f,			0.83f, 1.0f,	0.0f, -0.5f, 0.0f,

		//4
		//	x      y      z			u	  v			nx	  ny    nz
		0.0f, 0.0f, -0.5f,			0.17f, 0.0f,	0.0f, 0.0f, 0.5f,
		-0.4755f, 0.0f, -0.1545f,	0.5f, 0.0f,		0.4755f, 0.0f, 0.1545f,
		0.0f, 0.5f, 0.0f,			0.33f, 0.5f,	0.0f, -0.5f, 0.0f,

		//8
		//	x      y      z			u	  v			nx	  ny    nz
		-0.4755f, 0.0f, -0.1545f,	0.51f, 0.0f,	0.4755f, 0.0f, 0.1545f,
		-0.2938f, 0.0f, 0.4045f,	0.83f, 0.0f,	0.2938f, 0.0f, -0.4045f,
		0.0f, 0.5f, 0.0f,			0.66f, 0.5f,	0.0f, -0.5f, 0.0f,

		/////////////////////////////////////
		// Parte Baja
		/////////////////////////////////////
		//10
		//	x      y      z			u	  v			nx	  ny    nz
		0.2938f, 0.0f, 0.4045f,		1.0f, 0.5f,		-0.2938f, 0.0f, -0.4045f,
		-0.2938f, 0.0f, 0.4045f,	0.67f, 0.5f,	0.2938f, 0.0f, -0.4045f,
		0.0f, -0.5f, 0.0f,			0.83f, 0.0f,	0.0f, 0.5f, 0.0f,

		//6
		//	x      y      z			u	  v			nx	  ny    nz
		0.4755f, 0.0f, -0.1545f,	0.65f, 0.5f,	-0.4755f, 0.0f, 0.1545f,
		0.2938f, 0.0f, 0.4045f,		0.33f, 0.5f,	-0.2938f, 0.0f, -0.4045f,
		0.0f, -0.5f, 0.0f,			0.5f, 0.0f,		0.0f, 0.5f, 0.0f,

		//2
		//	x      y      z			u	  v			nx	  ny    nz
		0.0f, 0.0f, -0.5f,			0.33f, 0.49f,	0.0f, 0.0f, 0.5f,
		0.4755f, 0.0f, -0.1545f,	0.0f, 0.49f,	-0.4755f, 0.0f, 0.1545f,
		0.0f, -0.5f, 0.0f,			0.16f, 0.0f,	0.0f, 0.5f, 0.0f,

		//7
		//	x      y      z			u	  v			nx	  ny    nz
		-0.4755f, 0.0f, -0.1545f,	0.83f, 1.0f,	0.4755f, 0.0f, 0.1545f,
		0.0f, 0.0f, -0.5f,			0.5f, 1.0f,		0.0f, 0.0f, 0.5f,
		0.0f, -0.5f, 0.0f,			0.66f, 0.5f,	0.0f, 0.5f, 0.0f,

		//3
		//	x      y      z			u	  v			nx	  ny    nz
		-0.2938f, 0.0f, 0.4045f,	0.5f, 1.0f,		0.2938f, 0.0f, -0.4045f,
		-0.4755f, 0.0f, -0.1545f,	0.16f, 1.0f,	0.4755f, 0.0f, 0.1545f,
		0.0f, -0.5f, 0.0f,			0.33f, 0.5f,	0.0f, 0.5f, 0.0f,

	};

	Mesh* dado10Caras = new Mesh();
	dado10Caras->CreateMesh(dado10CarasVertices, dado10CarasIndices, 240, 30);
	meshList.push_back(dado10Caras);
}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	dado10Caras();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	Dado10 = Texture("Textures/TexDado.png");
	Dado10.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	Letrero_Arco = Model();
	Letrero_Arco.LoadModel("Models/Letrero.obj");
	Pilares_Arco = Model();
	Pilares_Arco.LoadModel("Models/Pilares.obj");
	Tablero = Model();
	Tablero.LoadModel("Models/tablero.obj");
	Cuerpo_avion = Model();
	Cuerpo_avion.LoadModel("Models/Cuerpo_avion.obj");
	Helice_avion = Model();
	Helice_avion.LoadModel("Models/Helice_avion.obj");



	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	movCoche = 0.0f;
	movOffset = 0.3f;
	rotllanta = 0.0f;
	rotllantaOffset = 5.0f;
	avanza = true;

	lana = 0.0f;
	lanaOffSet = 0.11f;
		
	arab = true;
	GLdouble now2 = 0.005f;
	GLdouble now3 = 0.0f;
	glfwSetTime(now2);
	////Loop mientras no se cierra la ventana

	//////////////////Variables para dado/////////////////////////
	float cara[][3] = { {-45.0f, 0.0f, 0.0f}, {-45.0f, 72.0f, 0.0f}, {-45.0f, 144.0f, 0.0f},
	{-45.0f, 216.0f, 0.0f}, {-45.0f, 288.0f, 0.0f}, {-125.0f, 0.0f, 0.0f}, {-125.0f, 72.0f, 0.0f},
	{-125.0f, 144.0f, 0.0f}, { -125.0f, 216.0f, 0.0f }, {-125.0f, 288.0f, 0.0f} };
	int min = 0;
	int max = 9;
	int randomInRange = 0;
	bool subir = false;
	bool bajar = false;
	bool girox = false;
	bool giroy = false;
	bool tirada = false;
	int cont = 10;
	rotx = 0.0f;
	rotxOffSet = 1.0f;
	roty = 0.0f;
	rotyOffSet = 1.0f;
	elevar = 0.0f;
	float elevaraux = 0.0f;
	elevarOffSet1 = 1.0f;
	elevarOffSet2 = 3.0f;
	srand(time(0));

	///////////////////////Variables recorrido///////////////////////////
	float movenx = 0.0f;
	float movenxOffSet = 0.3f;
	float moveny = 0.0f;
	float movenyOffSet = 0.07f;
	float movenz = 0.0f;
	float movenzOffSet = 0.3f;
	int recorrido = 1;
	int ele = 0;
	float front = 180.0f;
	float helice = 0.0f;
	float heliceOffSet = 10.0f;

	while (!mainWindow.getShouldClose())
	{


		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;


		/////////////////////////////ANIMACION DADO//////////////////////////////
		if (mainWindow.gettirada()) {
			subir = true;
			mainWindow.settirada(false);
		}

		if (subir) {
			if (elevaraux < 100.0f) {
				elevaraux += elevarOffSet1 * deltaTime;
				elevar = elevaraux;
			}
			else if (elevaraux < 150.0f) {
				elevaraux += elevarOffSet1 * deltaTime;
			}
			else {
				elevaraux = 0.0f;
				subir = false;
				bajar = true;
			}
		}

		if (bajar) {
			if (elevar > -4.0f) {
				elevar -= elevarOffSet2 * deltaTime;
			}
			else {
				bajar = false;
				randomInRange = rand() % (max - min + 1);
				tirada = girox = giroy = true;
				cont = 0;
			}
		}

		if (tirada && cont < 10) {
			printf("%d \n", randomInRange);
			if (girox || giroy) {
				if (cara[randomInRange][0] - 1.0f >= rotx) {
					rotx += rotxOffSet * deltaTime;
				}
				else if (cara[randomInRange][0] + 1.0f < rotx) {
					rotx -= rotxOffSet * deltaTime;
				}
				else {
					girox = false;
				}

				if (cara[randomInRange][1] - 1.0f >= roty) {
					roty += rotyOffSet * deltaTime;
				}
				else if (cara[randomInRange][1] + 1.0f < roty) {
					roty -= rotyOffSet * deltaTime;
				}
				else {
					giroy = false;
				}
			}
			else {
				tirada = false;
			}
		}
		else if (cont >= 10) {
			
		}
		else {
			randomInRange = rand() % (max - min + 1);
			cont += 1;
			tirada = girox = giroy = true;
		}


		/////////////////ANIMACION RECORRIDO////////////////////////////////
		if (helice < 360.0f) {
			helice += heliceOffSet * deltaTime;
		}
		else {
			helice = 0.0f;
		}
		
		switch (ele) {
			case 0:
				if (moveny < 1.0f) {
					moveny += movenyOffSet * deltaTime;
				}
				else {
					ele = 1;
				}
			break;
			
			case 1:
				if (moveny > -1.0f) {
					moveny -= movenyOffSet * deltaTime;
				}
				else {
					ele = 0;
				}
			break;
		}

		switch (recorrido) {
			case 1:
				if (movenx < 54.0f) {
					movenx += movenxOffSet * deltaTime;
				}
				else {
					front = 270.0f;
					recorrido = 2;
				}
			break;
				
			case 2:
				if (movenz > -54.0f) {
					movenz -= movenzOffSet * deltaTime;
				}
				else {
					front = 0.0f;
					recorrido = 3;
				}
			break;
				
			case 3:
				if (movenx > 0.0f) {
					movenx -= movenxOffSet * deltaTime;
				}
				else {
					front = 90.0f;
					recorrido = 4;
				}
			break;

			case 4:
				if (movenz < 0.0f) {
					movenz += movenzOffSet * deltaTime;
				}
				else {
					front = 180.0f;
					recorrido = 1;
				}
			break;
		}
		


		//printf("arrancar %f \n", arrancar);
		if (arrancar > 2) {
			if (avanza) {
				if (movCoche > -306.0f)
				{
					movCoche -= movOffset * deltaTime;
					//printf("avanza%f \n ",movCoche);
					rotllanta += rotllantaOffset * deltaTime;
				}
				else {
					avanza = !avanza;
				}
			}
			else {
				if (movCoche < 282.0f)
				{
					movCoche += movOffset * deltaTime;
					//printf("avanza%f \n ",movCoche);
					rotllanta += rotllantaOffset * deltaTime;
				}
				else {
					avanza = !avanza;
				}
			}
		}

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
			glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 4.5f + elevar, -40.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, glm::radians(rotx), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(roty), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Dado10.UseTexture();
		meshList[4]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 6.0f));
		model = glm::scale(model, glm::vec3(2.5f, 1.0f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tablero.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-27.0f + movenx, 4.0f + moveny, 33.0 + movenz));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, front * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cuerpo_avion.RenderModel();

		model = glm::translate(model, glm::vec3(-24.328, 5.2932f, 0.0));
		model = glm::rotate(model, helice * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helice_avion.RenderModel();


		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
