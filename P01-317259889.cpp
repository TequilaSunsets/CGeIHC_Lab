#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <cstdlib>  
#include <ctime>

//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shader;

float rojo, verde, azul = 0.0f;
int palRojo, palVerde, palAzul;

//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";
//recibir Vcolor y dar de salida color
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(1.0f,1.0f,1.0f,1.0f);	 			\n\
}";



void CrearTriangulo()
{
	GLfloat vertices[] = {
		//ELEMENTOS PARA DIBUJAR K
		//1-2-11
		-0.83f,0.4f,0.0f,
		-0.65f,0.4f,0.0f,
		-0.83f,-0.4f,0.0f,

		//2-10-11
		-0.65f,0.4f,0.0f,
		-0.65f,-0.4f,0.0f,
		-0.83f,-0.4f,0.0f,

		//3-4-6
		-0.65f,0.1f,0.0f,
		-0.55f,0.4f,0.0f,
		-0.5f,0.0f,0.0f,

		//4-5-6
		-0.55f,0.4f,0.0f,
		-0.37f,0.4f,0.0f,
		-0.5f,0.0f,0.0f,

		//7-8-9
		-0.37f,-0.4f,0.0f,
		-0.55f,-0.4f,0.0f,
		-0.65f,-0.05f,0.0f,

		//6-7-9
		-0.5f,0.0f,0.0f,
		-0.37f,-0.4f,0.0f,
		-0.65f,-0.05f,0.0f,

		//3-6-9
		-0.65f,0.1f,0.0f,
		-0.5f,0.0f,0.0f,
		-0.65f,-0.05f,0.0f,


		//ELEMENTOS PARA DIBUJAR S
		//1-2-4
		-0.25f,0.4f,0.0f,
		0.25f,0.4f,0.0f,
		-0.1f,0.25f,0.0f,

		//2-3-4
		0.25f,0.4f,0.0f,
		0.25f,0.25f,0.0f,
		-0.1f,0.25f,0.0f,

		//1-4-12
		-0.25f,0.4f,0.0f,
		-0.1f,0.25f,0.0f,
		-0.25f,-0.05f,0.0f,

		//4-5-12
		-0.1f,0.25f,0.0f,
		-0.1f,0.1f,0.0f,
		-0.25f,-0.05f,0.0f,

		//5-11-12
		-0.1f,0.1f,0.0f,
		0.1f,-0.05f,0.0f,
		-0.25f,-0.05f,0.0f,

		//5-6-11
		-0.1f,0.1f,0.0f,
		0.25f,0.1f,0.0f,
		0.1f,-0.05f,0.0f,

		//6-10-11
		0.25f,0.1f,0.0f,
		0.1f,-0.25f,0.0f,
		0.1f,-0.05f,0.0f,

		//6-7-10
		0.25f,0.1f,0.0f,
		0.25f,-0.4f,0.0f,
		0.1f,-0.25f,0.0f,

		//7-9-10
		0.25f,-0.4f,0.0f,
		-0.25f,-0.25f,0.0f,
		0.1f,-0.25f,0.0f,

		//7-8-9
		0.25f,-0.4f,0.0f,
		-0.25f,-0.4f,0.0f,
		-0.25f,-0.25f,0.0f,


		//ELEMENTOS PARA DIBUJAR T
		//1-2-3
		0.35f,0.4f,0.0f,
		0.85f,0.4f,0.0f,
		0.85f,0.25f,0.0f,

		//1-3-8
		0.35f,0.4f,0.0f,
		0.85f,0.25f,0.0f,
		0.35f,0.25f,0.0f,

		//4-5-7
		0.7f,0.25f,0.0f,
		0.7f,-0.4f,0.0f,
		0.5f,0.25f,0.0f,

		//5-6-7
		0.7f,-0.4f,0.0f,
		0.5f,-0.4f,0.0f,
		0.5f,0.25f,0.0f
	};

	glGenVertexArrays(1, &VAO); //generar 1 VAO
	glBindVertexArray(VAO);//asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es estático pues no se modificarán los valores

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
		glEnableVertexAttribArray(0);
		//agregar valores a vèrtices y luego declarar un nuevo vertexAttribPointer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

}
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Función para agregar los shaders a la tarjeta gráfica
//the Program recibe los datos de theShader


{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader,1, theCode, codeLength);//Se le asigna al shader el código
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevención de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n",shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
}

void CompileShaders() {
	shader= glCreateProgram(); //se crea un programa
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica
	 //verificaciones y prevención de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}



}
int main()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

	//Crear tríangulo
	CrearTriangulo();
	CompileShaders();

	std::srand(std::time(0));
	std::time_t tiempoInicial = std::time(0);

	//Loop mientras no se cierra la ventana
	palRojo = std::rand() % 101;
	palVerde = std::rand() % 101;
	palAzul = std::rand() % 101;

	rojo = static_cast<double>(palRojo) / 100.0;
	verde = static_cast<double>(palVerde) / 100.0;
	azul = static_cast<double>(palAzul) / 100.0;
	
	while (!glfwWindowShouldClose(mainWindow))
	{
		
		glfwPollEvents();

		std::time_t tiempoActual = std::time(0);

		if (tiempoActual - tiempoInicial >= 2) {
			palRojo = std::rand() % 101; 
			palVerde = std::rand() % 101; 
			palAzul = std::rand() % 101; 

			rojo = static_cast<double>(palRojo) / 100.0;
			verde = static_cast<double>(palVerde) / 100.0;
			azul = static_cast<double>(palAzul) / 100.0;

			tiempoInicial = tiempoActual;
		}

		glClearColor(rojo, verde, azul, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES,0,63);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);

	}


	return 0;
}