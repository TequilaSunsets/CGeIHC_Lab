/*
Animación por keyframes
La textura del skybox fue conseguida desde la página https ://opengameart.org/content/elyvisions-skyboxes?page=1
y edité en Gimp rotando 90 grados en sentido antihorario la imagen  sp2_up.png para poder ver continuidad.
Fuentes :
	https ://www.khronos.org/opengl/wiki/Keyframe_Animation
	http ://what-when-how.com/wp-content/uploads/2012/07/tmpcd0074_thumb.png
	*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

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

using namespace std;

int FrameIndex;

// Definición de la clase KeyFrame
class KeyFrameD {
public:
	float mov_x, mov_y, mov_z;
	float giro_x, giro_y, giro_z;

	// Constructor por defecto
	KeyFrameD() : mov_x(0.0f), mov_y(0.0f), mov_z(0.0f),
		giro_x(0.0f), giro_y(0.0f), giro_z(0.0f) {}

	// Constructor con movimiento
	void setKeyFrameD(float mx, float my, float mz) {
		mov_x = mx;
		mov_y = my;
		mov_z = mz;
		giro_x = 0.0f; // O puedes permitir que se configuren también
		giro_y = 0.0f; // lo que necesites
		giro_z = 0.0f;
	}

	// Constructor con movimiento y rotación
	void setKeyFrameD(float mx, float my, float mz, float gx, float gy, float gz) {
		mov_x = mx;
		mov_y = my;
		mov_z = mz;
		giro_x = gx; // O puedes permitir que se configuren también
		giro_y = gy; // lo que necesites
		giro_z = gz;
	}
};

// Arreglo global para almacenar los KeyFrames
KeyFrameD KeyFrames[100];


// Función para leer un archivo y llenar los KeyFrames
void leerArchivo(string nombreArchivo) {
	ifstream archivo(nombreArchivo);

	if (!archivo.is_open()) {
		cout << "El archivo no existe. Se creará uno nuevo." << endl;
		ofstream nuevoArchivo(nombreArchivo);  // Crear archivo si no existe
		nuevoArchivo.close();
		return;
	}

	string linea;
	while (getline(archivo, linea)) {
		stringstream os;
		os << linea;

		string basura, nombre;
		float valor;
		int indice;

		//KeyFrame[0].mov_x = 0
		//os >> "KeyFrame[" >> indice >> "]." >> nombre >> " = " >> valor;

		os.ignore(9); // "KeyFrame["
		os >> indice; // Indice del KeyFrame
		os.ignore(2); // "]."
		os >> nombre; // nombre de variable sea de giro o movimiento y en que eje lo aplica.
		os >> basura; // " = "
		os >> valor; // Valor variable


		if (indice < 0 || indice >= 100) {
			cerr << "Error: Índice fuera de rango.\n";
			continue;
		}


		if (nombre.compare("mov_x") == 0) {
			KeyFrames[indice].mov_x = valor;
		}
		else if (nombre.compare("mov_y") == 0) {
			KeyFrames[indice].mov_y = valor;
		}
		else if (nombre.compare("mov_z") == 0) {
			KeyFrames[indice].mov_z = valor;
		}
		else if (nombre.compare("giro_x") == 0) {
			KeyFrames[indice].giro_x = valor;
		}
		else if (nombre.compare("giro_y") == 0) {
			KeyFrames[indice].giro_y = valor;
		}
		else if (nombre.compare("giro_z") == 0) {
			KeyFrames[indice].giro_z = valor;
		}
		FrameIndex = indice+1;
		cout << "Indice Frame: " << FrameIndex << endl;
	}

	archivo.close();
}

// Función para escribir un KeyFrame en el archivo
void escribirArchivo(string nombreArchivo, KeyFrameD frame, int index) {
	ofstream archivo(nombreArchivo, ios::app);  // Abrir en modo "append"

	if (!archivo.is_open()) {
		cerr << "Error: No se pudo abrir el archivo para escritura.\n";
		return;
	}

	archivo << "KeyFrame[" << index << "].mov_x = " << frame.mov_x << endl;
	archivo << "KeyFrame[" << index << "].mov_y = " << frame.mov_y << endl;
	archivo << "KeyFrame[" << index << "].mov_z = " << frame.mov_z << endl;
	archivo << "KeyFrame[" << index << "].giro_x = " << frame.giro_x << endl;
	archivo << "KeyFrame[" << index << "].giro_y = " << frame.giro_y << endl;
	archivo << "KeyFrame[" << index << "].giro_z = " << frame.giro_z << endl;

	archivo.close();
}

void limpiarArchivo(const string nombreArchivo) {
	std::ofstream archivo(nombreArchivo, std::ios::trunc);

	if (!archivo) {
		std::cerr << "Error: No se pudo abrir el archivo para limpiar.\n";
		return;
	}

	archivo.close();
	std::cout << "Archivo '" << nombreArchivo << "' limpiado exitosamente.\n";
}


const float toRadians = 3.14159265f / 180.0f;
string nombreArchivo = "KeyFramesFW.txt";
KeyFrameD fm;

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;

//variables para keyframes
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;

///////////////////Modelo Kim///////////////////////////
Model cuerpo_kim;
Model chamorro_kim;
Model bota_kim;

///////////////////Modelo Chamarra///////////////////////////
Model base_FW;
Model hombroD_FW;
Model brazoD_FW;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

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

//función para teclado de keyframes 
void inputKeyframes(bool* keys);

//cálculo del promedio de las normales para sombreado de Phong
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
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

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

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


///////////////////////////////KEYFRAMES/////////////////////


bool animacion = false;

//NEW// Keyframes
float girox_hombro, girox_brazo;
float girox_hombro_INC, girox_brazo_INC;

#define MAX_FRAMES 100
int i_max_steps = 90;
int i_curr_steps = 8;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float girox_hombro;
	float girox_brazo;
	float girox_hombro_INC; 
	float girox_brazo_INC;


}FRAME;

FRAME KeyFrame[MAX_FRAMES];			//introducir datos
bool play = false;
int playIndex = 0;

////NEW// Keyframes
//float posXavion = 2.0, posYavion = 5.0, posZavion = -3.0;
//float	movAvion_x = 0.0f, movAvion_y = 0.0f;
//float giroAvion = 0;
//
//#define MAX_FRAMES 100
//int i_max_steps = 90;
//int i_curr_steps = 8;
//typedef struct _frame
//{
//	//Variables para GUARDAR Key Frames
//	float movAvion_x;		//Variable para PosicionX
//	float movAvion_y;		//Variable para PosicionY
//	float movAvion_xInc;		//Variable para IncrementoX
//	float movAvion_yInc;		//Variable para IncrementoY
//	float giroAvion;
//	float giroAvionInc;
//}FRAME;
//
//FRAME KeyFrame[MAX_FRAMES];			//introducir datos
//bool play = false;
//int playIndex = 0;

void almacenarKeyFrames() {
	leerArchivo(nombreArchivo);
	for (int i = 0; i < FrameIndex; i++) {
		KeyFrame[i].girox_hombro = KeyFrames[i].mov_x;
		KeyFrame[i].girox_brazo = KeyFrames[i].mov_y;
	}
}

void saveFrame(void) //tecla L
{
	printf("frameindex %d\n", FrameIndex);
	fm.setKeyFrameD(girox_hombro, girox_brazo, 0.0f);
	escribirArchivo(nombreArchivo, fm, FrameIndex);
	almacenarKeyFrames();
	//Ejercicio 1:
	//agregar una forma de escribir a un archivo para guardar los frames
}



void resetElements(void) //Tecla 0
{
	girox_hombro = KeyFrame[0].girox_hombro;
	girox_brazo = KeyFrame[0].girox_brazo;
}

void interpolation(void)
{
	KeyFrame[playIndex].girox_hombro_INC = (KeyFrame[playIndex + 1].girox_hombro - KeyFrame[playIndex].girox_hombro) / i_max_steps;
	KeyFrame[playIndex].girox_brazo_INC = (KeyFrame[playIndex + 1].girox_brazo - KeyFrame[playIndex].girox_brazo) / i_max_steps;
}


void animate(void)
{
	//Movimiento del objeto con barra espaciadora
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //fin de animación entre frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//Fin de toda la animación con último frame?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termino la animacion\n");
				playIndex = 0;
				play = false;
			}
			else //Interpolación del próximo cuadro
			{
				
				i_curr_steps = 0; //Resetea contador
				//Interpolar
				interpolation();
			}
		}
		else
		{
			//Dibujar Animación
			girox_hombro += KeyFrame[playIndex].girox_hombro_INC;
			girox_brazo += KeyFrame[playIndex].girox_brazo_INC;
			i_curr_steps++;
		}

	}
}

///////////////* FIN KEYFRAMES*////////////////////////////



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	almacenarKeyFrames();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

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
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();

	/////////////////////Modelo Kim/////////////////////////
	cuerpo_kim = Model();
	cuerpo_kim.LoadModel("Models/Kim/cuerpo_kim.obj");
	chamorro_kim = Model();
	chamorro_kim.LoadModel("Models/Kim/chamorro_kim.obj");
	bota_kim = Model();
	bota_kim.LoadModel("Models/Kim/bota_kim.obj");

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	/////////////////////Fuck World/////////////////////
	base_FW = Model();
	base_FW.LoadModel("Models/FuckWorld/base_FW.obj");
	hombroD_FW = Model();
	hombroD_FW.LoadModel("Models/FuckWorld/hombroD_FW.obj");
	brazoD_FW = Model();
	brazoD_FW.LoadModel("Models/FuckWorld/brazoD_FW.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/sp2_rt.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_lf.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_dn.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_up.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_bk.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_ft.png");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.5f, 0.5f,
		0.0f, -1.0f, 0.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
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
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movCoche = 0.0f;
	movOffset = 0.01f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;
	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);
	
	//---------PARA TENER KEYFRAMES GUARDADOS NO VOLATILES QUE SIEMPRE SE UTILIZARAN SE DECLARAN AQUÍ

	/*KeyFrame[0].movAvion_x = 0.0f;
	KeyFrame[0].movAvion_y = 0.0f;
	KeyFrame[0].giroAvion = 0.0f;


	KeyFrame[1].movAvion_x = -1.0f;
	KeyFrame[1].movAvion_y = 2.0f;
	KeyFrame[1].giroAvion = 0.0f;


	KeyFrame[2].movAvion_x = -2.0f;
	KeyFrame[2].movAvion_y = 0.0f;
	KeyFrame[2].giroAvion = 0.0f;

	KeyFrame[3].movAvion_x = -3.0f;
	KeyFrame[3].movAvion_y = -2.0f;
	KeyFrame[3].giroAvion = 0.0f;


	KeyFrame[4].movAvion_x = -3.0f;
	KeyFrame[4].movAvion_y = -2.0f;
	KeyFrame[4].giroAvion = 90.0f;


	KeyFrame[5].movAvion_x = -3.0f;
	KeyFrame[5].movAvion_y = -2.0f;
	KeyFrame[5].giroAvion = 180.0f;

	KeyFrame[6].movAvion_x = 0.0f;
	KeyFrame[6].movAvion_y = 0.0f;
	KeyFrame[6].giroAvion = 180.0f;

	KeyFrame[7].movAvion_x = 0.0f;
	KeyFrame[7].movAvion_y = 0.0f;
	KeyFrame[7].giroAvion = 0.0f;*/

	//Se agregan nuevos frames 

		printf("\nTeclas para uso de Keyframes:\n1.-Presionar barra espaciadora para reproducir animacion.\n2.-Presionar 0 para volver a habilitar reproduccion de la animacion\n");
		printf("3.-Presiona L para guardar frame\n4.-Presiona P para habilitar guardar nuevo frame\n5.-Presiona 1 para mover en X\n6.-Presiona 2 para habilitar mover en X");

	
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f*deltaTime;

		if (movCoche < 30.0f)
		{
			movCoche -= movOffset * deltaTime;
			//printf("avanza%f \n ",movCoche);
		}
		rotllanta += rotllantaOffset * deltaTime;


		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//-------Para Keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();

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
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
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
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
		
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();


		////////////////////FuckTheWorld////////////////////////////////
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, 20.0f));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		base_FW.RenderModel();

		model = glm::translate(model, glm::vec3(-0.2032f, 0.06009f, -0.0776f));
		model = glm::rotate(model, girox_hombro * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		hombroD_FW.RenderModel();

		model = glm::translate(model, glm::vec3(-0.3875 + 0.2032f, -0.0593f - 0.06009f, +0.0776f - 0.04881f));
		model = glm::rotate(model, girox_brazo * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		brazoD_FW.RenderModel();

		//model = glm::mat4(1.0);
		//posblackhawk=glm::vec3(posXavion + movAvion_x, posYavion + movAvion_y, posZavion);
		//model = glm::translate(model, posblackhawk);
		//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		//model = glm::rotate(model, giroAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		////color = glm::vec3(0.0f, 1.0f, 0.0f);
		////glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Blackhawk_M.RenderModel();
		

		

		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		
		//textura con movimiento
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetflechau += 0.001;
		toffsetflechav += 0.0;
		//para que no se desborde la variable
		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		//if (toffsetv > 1.0)
		//	toffsetv = 0;
		//printf("\ntfosset %f \n", toffsetu);
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetflechau, toffsetflechav);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		glDisable(GL_BLEND);
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}


void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("\n presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;

			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1 && reproduciranimacion>0)
		{
			printf("Ya puedes reproducir de nuevo la animación con la tecla de barra espaciadora'\n");
			reproduciranimacion = 0;
			habilitaranimacion++;
			
		}
	}

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
			reinicioFrame++;
			printf("Ya puedes guardar otro frame presionando la tecla L'\n");
		}
	}


	if (keys[GLFW_KEY_1])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			girox_hombro -= 5.0f;
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla 2 para poder habilitar la variable\n");
		}

	}
	if (keys[GLFW_KEY_2])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
			ciclo2++;
			printf("\n Ya puedes modificar tu variable movAvion_x presionando la tecla 1\n");
		}
	}

	if (keys[GLFW_KEY_3])
	{
		if (ciclo < 1)
		{
			girox_hombro += 5.0f;
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla 4 para poder habilitar la variable\n");
		}

	}
	if (keys[GLFW_KEY_4])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
			ciclo2++;
			printf("\n Ya puedes modificar tu variable movAvion_x presionando la tecla 3\n");
		}
	}

	if (keys[GLFW_KEY_5])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			girox_brazo -= 5.0f;
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla 6 para poder habilitar la variable\n");
		}

	}
	if (keys[GLFW_KEY_6])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
			ciclo2++;
			printf("\n Ya puedes modificar tu variable movAvion_y presionando la tecla 5\n");
		}
	}

	if (keys[GLFW_KEY_7])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			girox_brazo += 5.0f;
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla 8 para poder habilitar la variable\n");
		}

	}
	if (keys[GLFW_KEY_8])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
			ciclo2++;
			printf("\n Ya puedes modificar tu variable movAvion_y presionando la tecla 7\n");
		}
	}

//	if (keys[GLFW_KEY_F])
//	{
//		if (ciclo < 1)
//		{
//			//printf("movAvion_x es: %f\n", movAvion_x);
//			girox_cha += 5.0f;
//			ciclo++;
//			ciclo2 = 0;
//			printf("\n Presiona la tecla G para poder habilitar la variable  giroAvion\n");
//		}
//
//	}
//	if (keys[GLFW_KEY_G])
//	{
//		if (ciclo2 < 1)
//		{
//			ciclo = 0;
//			ciclo2++;
//			printf("\n Ya puedes modificar tu variable de giroAvion presionando la tecla 9 \n");
//		}
//	}
//
//
//
//
//if (keys[GLFW_KEY_H])
//{
//	if (ciclo < 1)
//	{
//		//printf("movAvion_x es: %f\n", movAvion_x);
//		girox_cha -= 5.0f;
//		ciclo++;
//		ciclo2 = 0;
//		printf("\n Presiona la tecla G para poder habilitar la variable  giroAvion\n");
//	}
//
//}
//if (keys[GLFW_KEY_J])
//{
//	if (ciclo2 < 1)
//	{
//		ciclo = 0;
//		ciclo2++;
//		printf("\n Ya puedes modificar tu variable de giroAvion presionando la tecla 9 \n");
//	}
//}
//
//
//
//
//if (keys[GLFW_KEY_C])
//{
//	if (ciclo < 1)
//	{
//		//printf("movAvion_x es: %f\n", movAvion_x);
//		giroy_cha += 5.0f;
//		ciclo++;
//		ciclo2 = 0;
//		printf("\n Presiona la tecla G para poder habilitar la variable  giroAvion\n");
//	}
//
//}
//if (keys[GLFW_KEY_V])
//{
//	if (ciclo2 < 1)
//	{
//		ciclo = 0;
//		ciclo2++;
//		printf("\n Ya puedes modificar tu variable de giroAvion presionando la tecla 9 \n");
//	}
//}
//
//
//
//
//if (keys[GLFW_KEY_B])
//{
//	if (ciclo < 1)
//	{
//		//printf("movAvion_x es: %f\n", movAvion_x);
//		giroy_cha -= 5.0f;
//		ciclo++;
//		ciclo2 = 0;
//		printf("\n Presiona la tecla G para poder habilitar la variable  giroAvion\n");
//	}
//
//}
//if (keys[GLFW_KEY_N])
//{
//	if (ciclo2 < 1)
//	{
//		ciclo = 0;
//		ciclo2++;
//		printf("\n Ya puedes modificar tu variable de giroAvion presionando la tecla 9 \n");
//	}
//}


}


