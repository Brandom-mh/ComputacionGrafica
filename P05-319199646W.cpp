#include "Window.h"
#include <iostream>
using namespace std;
Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	articulacion5 = 0.0f;
	articulacion6 = 0.0f;
	articulacion7 = 0.0f;
	articulacion8 = 0.0f;
	articulacion9 = 0.0f;
	articulacion10= 0.0f;
	articulacion11 = 0.0f;
	articulacion12 = 0.0f;
	articulacion13 = 0.0f;
	articulacion14 = 0.0f;
	articulacion15 = 0.0f;
	

	
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
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
	mainWindow = glfwCreateWindow(width, height, "Practica XX: Nombre de la pr�ctica", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se est� usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));


	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}


	if (key == GLFW_KEY_E)
	{
		theWindow->rotax += 10.0;
	}
	if (key == GLFW_KEY_R)
	{
		theWindow->rotay += 10.0; //rotar sobre el eje y 10 grados
	}
	if (key == GLFW_KEY_T)
	{
		theWindow->rotaz += 10.0;
	}
	if (key == GLFW_KEY_F) {
		static bool subiendo = false;
		float minAngulo = -45.0f;
		float maxAngulo = 0.0f;
		if (subiendo) {
			theWindow->articulacion1 += 1;
			if (theWindow->articulacion1 >= maxAngulo) {
				theWindow->articulacion1 = maxAngulo;
				subiendo = false;
			}

		}
		else {
			theWindow->articulacion1 -= 1;
			if (theWindow->articulacion1 <= minAngulo) {
				theWindow->articulacion1 = minAngulo;
				subiendo = true;
			}

		}
	}
	static bool subiendo = true;
	float minAngulo = -15.0f;
	float maxAngulo = 30.0f;

	if (key == GLFW_KEY_G)
	{

		if (subiendo) {
			theWindow->articulacion2 += 1;
			if (theWindow->articulacion2 >= maxAngulo) {
				theWindow->articulacion2 = maxAngulo;
				subiendo = false;
			}
		}
		else {
			theWindow->articulacion2 -= 1;
			if (theWindow->articulacion2 <= minAngulo) {
				theWindow->articulacion2 = minAngulo;
				subiendo = true;
			}
		}
	}
	if (key == GLFW_KEY_H)
	{
		if (subiendo) {
			theWindow->articulacion3 += 1;
			if (theWindow->articulacion3 >= maxAngulo) {
				theWindow->articulacion3 = maxAngulo;
				subiendo = false;
			}

		}
		else {
			theWindow->articulacion3 -= 1;
			if (theWindow->articulacion3 <= minAngulo) {
				theWindow->articulacion3 = minAngulo;
				subiendo = true;
			}

		}
	}
	if (key == GLFW_KEY_J)
	{
		if (subiendo) {
			theWindow->articulacion4 += 1;
			if (theWindow->articulacion4 >= maxAngulo) {
				theWindow->articulacion4 = maxAngulo;
				subiendo = false;
			}

		}
		else {
			theWindow->articulacion4 -= 1;
			if (theWindow->articulacion4 <= minAngulo) {
				theWindow->articulacion4 = minAngulo;
				subiendo = true;
			}

		}
	}
	if (key == GLFW_KEY_K)
	{
		if (subiendo) {
			theWindow->articulacion5 += 1;
			if (theWindow->articulacion5 >= maxAngulo) {
				theWindow->articulacion5 = maxAngulo;
				subiendo = false;
			}

		}
		else {
			theWindow->articulacion5 -= 1;
			if (theWindow->articulacion5 <= minAngulo) {
				theWindow->articulacion5 = minAngulo;
				subiendo = true;
			}

		}
	}
	if (key == GLFW_KEY_L)
	{
		theWindow->articulacion6 -= 5.0;
	}
	if (key == GLFW_KEY_V)
	{
		theWindow->articulacion7 -= 10.0;
	}
	if (key == GLFW_KEY_B)
	{
		theWindow->articulacion8 -= 10.0;
	}
	if (key == GLFW_KEY_N)
	{
		theWindow->articulacion9 -= 10.0;
	}
	if (key == GLFW_KEY_6)
	{
		theWindow->articulacion12 += 5.0;
	}
	if (key == GLFW_KEY_7)
	{
		theWindow->articulacion13 += 10.0;
	}
	if (key == GLFW_KEY_8)
	{
		theWindow->articulacion14 += 10.0;
	}
	if (key == GLFW_KEY_9)
	{
		theWindow->articulacion15 += 10.0;
	}
	if (key == GLFW_KEY_M)
	{
		static bool arriba = true;
		float minAngulo = 0.0f;
		float maxAngulo = 45.0f;
		if (arriba) {
			theWindow->articulacion10 += 1;
			if (theWindow->articulacion10 >= maxAngulo) {
				theWindow->articulacion10 = maxAngulo;
				arriba = false;
			}

		}
		else {
			theWindow->articulacion10 -= 1;
			if (theWindow->articulacion10 <= minAngulo) {
				theWindow->articulacion10 = minAngulo;
				arriba = true;
			}

		}
	}

	if (key == GLFW_KEY_I)
	{
		theWindow->articulacion11 += 1.0;
	}



		if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
			const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
			//printf("se presiono la tecla: %s\n",key_name);
		}

		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS)
			{
				theWindow->keys[key] = true;
				//printf("se presiono la tecla %d'\n", key);
			}
			else if (action == GLFW_RELEASE)
			{
				theWindow->keys[key] = false;
				//printf("se solto la tecla %d'\n", key);
			}
		}
	}

	void Window::ManejaMouse(GLFWwindow * window, double xPos, double yPos)
	{
		Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

		if (theWindow->mouseFirstMoved)
		{
			theWindow->lastX = xPos;
			theWindow->lastY = yPos;
			theWindow->mouseFirstMoved = false;
		}

		theWindow->xChange = xPos - theWindow->lastX;
		theWindow->yChange = theWindow->lastY - yPos;

		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
	}


	Window::~Window()
	{
		glfwDestroyWindow(mainWindow);
		glfwTerminate();

	}


