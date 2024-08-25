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
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
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

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	//GLfloat vertices_letras[] = {	
	//		//X			Y			Z			R		G		B
	//		-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
	//		1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
	//		1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
	//		/*1.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
	//		-1.0f,  1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
	//		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,*/
	//		
	//};
	//MeshColor *letras = new MeshColor();
	//letras->CreateMeshColor(vertices_letras,18);
	//meshColorList.push_back(letras); //Agrega elemento a la lista

	GLfloat vertices_trianguloAzul[] = {
		//X			Y			Z			R		G		B
		-0.47f,	-0.25f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.47f,	-0.25f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.0f,	0.25f,		0.5f,			0.0f,	0.0f,	1.0f,
	};
	MeshColor* trianguloAzul = new MeshColor();
	trianguloAzul->CreateMeshColor(vertices_trianguloAzul, 18);
	meshColorList.push_back(trianguloAzul);

	GLfloat vertices_trianguloVerde[] = {
		//X			Y			Z			R		G		B
		-0.175f,-0.20f,		0.5f,			0.0f,	0.5f,	0.0f,
		0.175f,	-0.20f,		0.5f,			0.0f,	0.5f,	0.0f,
		0.0f,	0.20f,		0.5f,			0.0f,	0.5f,	0.0f,
	};
	MeshColor* trianguloVerde = new MeshColor();
	trianguloVerde->CreateMeshColor(vertices_trianguloVerde, 18);
	meshColorList.push_back(trianguloVerde);

	GLfloat vertices_cuadradoVerde[] = {
		//X			Y			Z			R		G		B
		-0.125f, -0.125f,	0.5f,			0.0f,	1.0f,	0.0f,
		0.125f,	 -0.125f,	0.5f,			0.0f,	1.0f,	0.0f,
		0.125f,	 0.125f,	0.5f,			0.0f,	1.0f,	0.0f,
		-0.125f, -0.125f,	0.5f,			0.0f,	1.0f,	0.0f,
		0.125f,	 0.125f,	0.5f,			0.0f,	1.0f,	0.0f,
		-0.125f, 0.125f,	0.5f,			0.0f,	1.0f,	0.0f,

	};
	MeshColor* cuadradoVerde = new MeshColor();
	cuadradoVerde->CreateMeshColor(vertices_cuadradoVerde, 36);
	meshColorList.push_back(cuadradoVerde);

	GLfloat vertices_cuadradoRojo[] = {
		//X			Y			Z			R		G		B
		-0.42f,  -0.42f,	  0.5f,			1.0f,	0.0f,	0.0f,
		0.42f,	 -0.42f,	  0.5f,			1.0f,	0.0f,	0.0f,
		0.42f,	 0.42f,	      0.5f,			1.0f,	0.0f,	0.0f,
		-0.42f,  -0.42f,	  0.5f,			1.0f,	0.0f,	0.0f,
		0.42f,	 0.42f,		  0.5f,			1.0f,	0.0f,	0.0f,
		-0.42f,  0.42f,		  0.5f,			1.0f,	0.0f,	0.0f,

	};
	MeshColor* cuadradoRojo = new MeshColor();
	cuadradoRojo->CreateMeshColor(vertices_cuadradoRojo, 36);
	meshColorList.push_back(cuadradoRojo);


	GLfloat vertices_cuadradoCafe[] = {
		//X			Y			Z			R		G		B
		-0.075f,  -0.075f,	  0.075f,		0.478f,	0.255f,	0.067f,
		0.075f,  -0.075f,	  0.075f,		0.478f,	0.255f,	0.067f,
		0.075f,  0.075f,	  0.075f,		0.478f,	0.255f,	0.067f,
		-0.075f,  -0.075f,	  0.075f,		0.478f,	0.255f,	0.067f,
		0.075f,  0.075f,	  0.075f,		0.478f,	0.255f,	0.067f,
		-0.075f,  0.075f,	  0.075f,		0.478f,	0.255f,	0.067f,
	};
	MeshColor* cuadradoCafe = new MeshColor();
	cuadradoCafe->CreateMeshColor(vertices_cuadradoCafe, 36);
	meshColorList.push_back(cuadradoCafe);

}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
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
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//Vista en perspectiva
	//Angulo de apertura FOV (campo de vision), Aspect Ratio (que sea un numero lo mas cercano a 1 posible), setNear, setFar (distancia de dibujado) 
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f,1.0f,1.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
													
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		//Le estamos indicando que el programa cambie entre uno u otro shader
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();


		///////////////////Cuadrado Rojo//////////////////////////

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[3]->RenderMeshColor();

		///////////////////Triangulo Azul//////////////////////////
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.67f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();

		///////////////////Triangulo verde//////////////////////////

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.645f, -0.07f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();		model = glm::mat4(1.0);
		
		model = glm::translate(model, glm::vec3(-0.645f, -0.07f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();

		////////////////////Cuadrado verde////////////////////

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.295f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.21f, 0.21f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.21f, 0.21f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();

		///////////////////Cuadrado Cafe//////////////////////////

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.645f, -0.345f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[4]->RenderMeshColor();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.645f, -0.345f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[4]->RenderMeshColor();

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