//Práctica 2: índices, mesh 4xn, proyecciones 4x4, transformaciones geométricas 4x4
//Modifica el nombre de la ventana en el archivo Wwindow.cpp
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector> //Sirve para crear nuestra estructura de datos de tipo lista
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp> //proyecciones y tranformaciones geometricas
#include<gtc\type_ptr.hpp> //Para mandar datos al shader
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h" //Toda la informacion de glfw (interfaces, ventanas, etc.)
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
//Creando varibales de tipo lista
std::vector<Mesh*> meshList; //Todos los mesh que voy a crear
std::vector<MeshColor*> meshColorList; //Todos los mesh color que voy a crear 
std::vector<Shader>shaderList; //Almacena shaders que creo
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
//Mis shaders creados
static const char* vShader1 = "shaders/shaderPAzul.vert";
static const char* vShader2 = "shaders/shaderPCafe.vert";
static const char* vShader3 = "shaders/shaderPRojo.vert";
static const char* vShader4 = "shaders/shaderPVerde.vert";
static const char* vShader5 = "shaders/shaderPVerdeMed.vert";

static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int trianguloA_indices[] = {
		// front
		0, 1, 2,
		
		// right
		0, 2, 3,
		
		// back
		0, 3, 4,

		// left
		0, 4, 1,
		
		// base
		1, 2, 3,
		1, 3, 4,
	};

	GLfloat trianguloA_vertices[] = {
		// punta
		0.0f, 0.25f,  0.0f,
		// base
		-0.47f, -0.25f,  0.47f,
		0.47f,  -0.25f,  0.47f,
		0.47f,  -0.25f,  -0.47f,
		-0.47f,  -0.25f,  -0.47f,
	};
	Mesh* trianguloA = new Mesh();
	trianguloA->CreateMesh(trianguloA_vertices, trianguloA_indices, 15, 18);
	meshList.push_back(trianguloA);

	unsigned int trianguloV_indices[] = {
		// front
		0, 1, 2,

		// right
		0, 2, 3,

		// back
		0, 3, 4,

		// left
		0, 4, 1,

		// base
		1, 2, 3,
		1, 3, 4,
	};

	GLfloat trianguloV_vertices[] = {
		// punta
		0.0f, 0.2f,  0.0f,
		// base
		-0.175f, -0.2f,  0.175f,
		0.175f,  -0.2f,  0.175f,
		0.175f,  -0.2f,  -0.175f,
		-0.175f,  -0.2f,  -0.175f,
	};
	Mesh* trianguloV = new Mesh();
	trianguloV->CreateMesh(trianguloV_vertices, trianguloV_indices, 15, 18);
	meshList.push_back(trianguloV);

}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cuboR_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cuboR_vertices[] = {
	// front
	-0.42f, -0.42f,  0.42f,
	0.42f, -0.42f,  0.42f,
	0.42f,  0.42f,  0.42f,
	-0.42f,  0.42f,  0.42f,
	// back
	-0.42f, -0.42f, -0.42f,
	0.42f, -0.42f, -0.42f,
	0.42f,  0.42f, -0.42f,
	-0.42f,  0.42f, -0.42f
};
Mesh *cuboR = new Mesh();
cuboR->CreateMesh(cuboR_vertices, cuboR_indices,24, 36);
meshList.push_back(cuboR);

unsigned int cuboC_indices[] = {
	// front
	0, 1, 2,
	2, 3, 0,
	// right
	1, 5, 6,
	6, 2, 1,
	// back
	7, 6, 5,
	5, 4, 7,
	// left
	4, 0, 3,
	3, 7, 4,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// top
	3, 2, 6,
	6, 7, 3
};

GLfloat cuboC_vertices[] = {
	// front
	-0.075f, -0.075f,  0.075f,
	0.075f, -0.075f,  0.075f,
	0.075f,  0.075f,  0.075f,
	-0.075f,  0.075f,  0.075f,
	// back
	-0.075f, -0.075f, -0.075f,
	0.075f, -0.075f, -0.075f,
	0.075f,  0.075f, -0.075f,
	-0.075f,  0.075f, -0.075f
};
Mesh* cuboC = new Mesh();
cuboC->CreateMesh(cuboC_vertices, cuboC_indices, 24, 36);
meshList.push_back(cuboC);

unsigned int cuboV_indices[] = {
	// front
	0, 1, 2,
	2, 3, 0,
	// right
	1, 5, 6,
	6, 2, 1,
	// back
	7, 6, 5,
	5, 4, 7,
	// left
	4, 0, 3,
	3, 7, 4,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// top
	3, 2, 6,
	6, 7, 3
};

GLfloat cuboV_vertices[] = {
	// front
	-0.125f, -0.125f,  0.125f,
	0.125f, -0.125f,  0.125f,
	0.125f,  0.125f,  0.125f,
	-0.125f,  0.125f,  0.125f,
	// back
	-0.125f, -0.125f, -0.125f,
	0.125f, -0.125f, -0.125f,
	0.125f,  0.125f, -0.125f,
	-0.125f,  0.125f, -0.125f
};
Mesh* cuboV = new Mesh();
cuboV->CreateMesh(cuboV_vertices, cuboV_indices, 24, 36);
meshList.push_back(cuboV);
}

void CrearLetrasyFiguras()
{

	GLfloat vertices_K[] = {
		//X			Y			Z			R		G		B
		-0.83f,0.4f,0.0f,				  0.0f,   0.0f,    1.0f,
		-0.65f,0.4f,0.0f,				  0.0f,   0.0f,    1.0f,
		-0.83f,-0.4f,0.0f,				  0.0f,   0.0f,    1.0f,

		-0.65f,0.4f,0.0f,				  0.0f,   0.0f,    1.0f,
		-0.65f,-0.4f,0.0f,				  0.0f,   0.0f,    1.0f,
		-0.83f,-0.4f,0.0f,				  0.0f,   0.0f,    1.0f,

		-0.65f,0.1f,0.0f,				  0.0f,   0.0f,    1.0f,
		-0.55f,0.4f,0.0f,				  0.0f,   0.0f,    1.0f,
		-0.5f,0.0f,0.0f,				  0.0f,   0.0f,    1.0f,

		-0.55f,0.4f,0.0f,				  0.0f,   0.0f,    1.0f,
		-0.37f,0.4f,0.0f,				  0.0f,   0.0f,    1.0f,
		-0.5f,0.0f,0.0f,				  0.0f,   0.0f,    1.0f,

		-0.37f,-0.4f,0.0f,				  0.0f,   0.0f,    1.0f,
		-0.55f,-0.4f,0.0f,				  0.0f,   0.0f,    1.0f,
		-0.65f,-0.05f,0.0f,				  0.0f,   0.0f,    1.0f,

		-0.5f,0.0f,0.0f,				  0.0f,   0.0f,    1.0f,
		-0.37f,-0.4f,0.0f,				  0.0f,   0.0f,    1.0f,
		-0.65f,-0.05f,0.0f,				  0.0f,   0.0f,    1.0f,

		-0.65f,0.1f,0.0f,				  0.0f,   0.0f,    1.0f,
		-0.5f,0.0f,0.0f,				  0.0f,   0.0f,    1.0f,
		-0.65f,-0.05f,0.0f,				  0.0f,   0.0f,    1.0f,

	};
	MeshColor* LetraK = new MeshColor();
	LetraK->CreateMeshColor(vertices_K, 126);
	meshColorList.push_back(LetraK);

	GLfloat vertices_S[] = {
		//X			Y			Z			R		G		B
		-0.25f,0.4f,0.0f,				  0.0f,   1.0f,    0.0f,
		0.25f,0.4f,0.0f,				  0.0f,   1.0f,    0.0f,
		-0.1f,0.25f,0.0f,				  0.0f,   1.0f,    0.0f,
						  
		0.25f,0.4f,0.0f,				  0.0f,   1.0f,    0.0f,
		0.25f,0.25f,0.0f,				  0.0f,   1.0f,    0.0f,
		-0.1f,0.25f,0.0f,				  0.0f,   1.0f,    0.0f,
						  
		-0.25f,0.4f,0.0f,				  0.0f,   1.0f,    0.0f,
		-0.1f,0.25f,0.0f,				  0.0f,   1.0f,    0.0f,
		-0.25f,-0.05f,0.0f,				  0.0f,   1.0f,    0.0f,

		-0.1f,0.25f,0.0f,				  0.0f,   1.0f,    0.0f,
		-0.1f,0.1f,0.0f,				  0.0f,   1.0f,    0.0f,
		-0.25f,-0.05f,0.0f,				  0.0f,   1.0f,    0.0f,

		-0.1f,0.1f,0.0f,				  0.0f,   1.0f,    0.0f,
		0.1f,-0.05f,0.0f,				  0.0f,   1.0f,    0.0f,
		-0.25f,-0.05f,0.0f,				  0.0f,   1.0f,    0.0f,

		-0.1f,0.1f,0.0f,				  0.0f,   1.0f,    0.0f,
		0.25f,0.1f,0.0f,				  0.0f,   1.0f,    0.0f,
		0.1f,-0.05f,0.0f,				  0.0f,   1.0f,    0.0f,

		0.25f,0.1f,0.0f,				  0.0f,   1.0f,    0.0f,
		0.1f,-0.25f,0.0f,				  0.0f,   1.0f,    0.0f,
		0.1f,-0.05f,0.0f,				  0.0f,   1.0f,    0.0f,

		0.25f,0.1f,0.0f,				  0.0f,   1.0f,    0.0f,
		0.25f,-0.4f,0.0f,				  0.0f,   1.0f,    0.0f,
		0.1f,-0.25f,0.0f,				  0.0f,   1.0f,    0.0f,

		0.25f,-0.4f,0.0f,				  0.0f,   1.0f,    0.0f,
		-0.25f,-0.25f,0.0f,				  0.0f,   1.0f,    0.0f,
		0.1f,-0.25f,0.0f,				  0.0f,   1.0f,    0.0f,

		0.25f,-0.4f,0.0f,				  0.0f,   1.0f,    0.0f,
		-0.25f,-0.4f,0.0f,				  0.0f,   1.0f,    0.0f,
		-0.25f,-0.25f,0.0f,				  0.0f,   1.0f,    0.0f,
	};
	MeshColor* LetraS = new MeshColor();
	LetraS->CreateMeshColor(vertices_S, 180);
	meshColorList.push_back(LetraS);


	GLfloat vertices_T[] = {
		//X			Y			Z			R		G		B
		0.35f,0.4f,0.0f,				  1.0f,   0.0f,    0.0f,
		0.85f,0.4f,0.0f,				  1.0f,   0.0f,    0.0f,
		0.85f,0.25f,0.0f,				  1.0f,   0.0f,    0.0f,

		0.35f,0.4f,0.0f,				  1.0f,   0.0f,    0.0f,
		0.85f,0.25f,0.0f,				  1.0f,   0.0f,    0.0f,
		0.35f,0.25f,0.0f,				  1.0f,   0.0f,    0.0f,

		0.7f,0.25f,0.0f,				  1.0f,   0.0f,    0.0f,
		0.7f,-0.4f,0.0f,				  1.0f,   0.0f,    0.0f,
		0.5f,0.25f,0.0f,				  1.0f,   0.0f,    0.0f,

		0.7f,-0.4f,0.0f,				  1.0f,   0.0f,    0.0f,
		0.5f,-0.4f,0.0f,				  1.0f,   0.0f,    0.0f,
		0.5f,0.25f,0.0f,				  1.0f,   0.0f,    0.0f,
	};
	MeshColor* LetraT = new MeshColor();
	LetraT->CreateMeshColor(vertices_T, 72);
	meshColorList.push_back(LetraT);

}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	//Shaders Propios
	Shader* shader3 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader1, fShader);
	shaderList.push_back(*shader1);

	Shader* shader4 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader2, fShader);
	shaderList.push_back(*shader1);

	Shader* shader5 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader3, fShader);
	shaderList.push_back(*shader1);

	Shader* shader6 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader4, fShader);
	shaderList.push_back(*shader1);

	Shader* shader7 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader5, fShader);
	shaderList.push_back(*shader1);

}


int main()
{
	//mainWindow = Window(800, 600); default
	//mainWindow = Window(1020, 720);
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	//Por el orden en que entraron en lista el orden importa en las funciones de Crea
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList

	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//Valores deafualt del viewport
	//Vista ortogonal
	// xmin,xmax,ymin,ymax,zmin(mayor a 0),zmax (z solo dice la distancia de dibujado), los mas deben ser mayores a sus min
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//Vista en perspectiva
	//Angulo de apertura FOV (campo de vision), Aspect Ratio (que sea un numero lo mas cercano a 1 posible), setNear, setFar (distancia de dibujado) 
	
	glm::mat4 projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 100.0f);

	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
		angulo += 0.08;
		//Definicion del shader a utilizar para dibujar elementos en pantalla.
		
		////////////////////////CUADRADO ROJO//////////////////////////////
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();

		//////////////////////////TRIANGULO AZUL///////////////////////////
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.67f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//////////////////////////CUADRADO CAFE///////////////////////////
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.645f, -0.345f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[3]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.645f, -0.345f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[3]->RenderMesh();

		//////////////////////////TRIANGULO VERDE///////////////////////////
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.645f, -0.07f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.645f, -0.07f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//////////////////////////CUADRADO VERDE///////////////////////////
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.295f, -3.58f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[4]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.21f, 0.21f, -3.58f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[4]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.21f, 0.21f, -3.58f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[4]->RenderMesh();

		////////////////////////////////////////////////////////////////////////////

		//Shader para definir el color de vertices dentro de la matriz
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		///////////////////Letra K//////////////////////////
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();

		
		///////////////////Letra S//////////////////////////
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();


		///////////////////Letra T//////////////////////////
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();


		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0); fuera del while
// reestablecer matriz: model = glm::mat4(1.0); dentro del while
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/