/*
Animación:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Compleja: Por medio de funciones y algoritmos.
Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

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
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza = true;
bool avanzaDragon = true;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;
float dragonavance = 0.0f;
float alaAngulo = 0.0f;
float timer = 0.0f;
bool cambio = true;

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
Texture ArcoTexture;
Texture LetreroTexture;
Texture PuertaTexture;
Texture DadoTexture;


Model Kitt_M;
Model Llanta_M;
Model Dragon_M;
Model alaDerecha_M;
Model alaIzquierda_M;
Model Tiamat_M;
Model Arco_M;
Model Letrero_M;
Model Puerta_Derecha_M;
Model Puerta_Izquierda_M;
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
		glm::vec3 normal = glm::cross(v2, v1);
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

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
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
	meshList.push_back(obj6); // todos los números

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número

}

void CrearOctaedro()
{
	// Índices: 8 triángulos (caras)
	unsigned int oct_indices[] = {
		// (0,2,4)
		0,  1,  2,
		// (2,1,4)
		3,  4,  5,
		// (1,3,4)
		6,  7,  8,
		// (3,0,4)
		9, 10, 11,
		// (2,0,5)
		12, 13, 14,
		// (1,2,5)
		15, 16, 17,
		// (3,1,5)
		18, 19, 20,
		// (0,3,5)
		21, 22, 23
	};

	// Vértices: 24 (3 por cara). UV por tri: (0,0), (1,0), (0.5,1)
	// Formato: x, y, z, s, t, nx, ny, nz
	GLfloat oct_vertices[] = {
		// Cara (0,2,4) 1
		 1.f,  0.f,  0.f,   0.24f,0.51f,   0,0,0, //derecha
		 0.f,  1.f,  0.f,   0.125f,0.99f,   0,0,0,//punta
		 0.f,  0.f,  1.f,   0.01f, 0.51f,   0,0,0,//izquierda

		 // Cara (2,1,4)4
		  0.f,  1.f,  0.f,   0.875f, 0.99f,   0,0,0,//punta
		 -1.f,  0.f,  0.f,   0.76f, 0.51f,   0,0,0,//izquierda
		  0.f,  0.f,  1.f,   0.99f, 0.51f,   0,0,0,//derecha

		  // Cara (1,3,4) 8
		  -1.f,  0.f,  0.f,   0.98f, 0.49f,   0,0,0,//derecha
		   0.f, -1.f,  0.f,   0.875f, 0.01f,    0,0,0, //punta
		   0.f,  0.f,  1.f,   0.77f, 0.49f,    0,0,0, //izquierda

		   // Cara (3,0,4) 5
		   0.f, -1.f,  0.f,   0.125f, 0.01f,  0,0,0, //punta
		   1.f,  0.f,  0.f,   0.24f, 0.49f,   0,0,0, //derecha
		   0.f,  0.f,  1.f,   0.01f, 0.49f,    0,0,0, //izquierda

		   // Cara (2,0,5) 2
			0.f,  1.f,  0.f,   0.375f, 1.0f,   0,0,0,
			1.f,  0.f,  0.f,   0.25f, 0.5f,   0,0,0,
			0.f,  0.f, -1.f,   0.5f, 0.5f,   0,0,0,

			// Cara (1,2,5) 3
			-1.f,  0.f,  0.f,   0.75f, 0.5f,    0,0,0,
			 0.f,  1.f,  0.f,   0.625f, 1.0f,   0,0,0,
			 0.f,  0.f, -1.f,    0.5f, 0.5f,   0,0,0,

			 // Cara (3,1,5) 7
			  0.f, -1.f,  0.f,   0.625f, 0.0f,   0,0,0, //punta
			 -1.f,  0.f,  0.f,   0.75f, 0.5f,   0,0,0, //derecha
			  0.f,  0.f, -1.f,   0.5f, 0.5f,   0,0,0, //izquierda

			  // Cara (0,3,5) 6
			   1.f,  0.f,  0.f,   0.25f, 0.5f,   0,0,0,
			   0.f, -1.f,  0.f,   0.375f, 0.0f,    0,0,0,
			   0.f,  0.f, -1.f,    0.5f, 0.5f,    0,0,0,
	};;

	calcAverageNormals(oct_indices, 24, oct_vertices, 192, 8, 5);

	Mesh* oct = new Mesh();
	oct->CreateMesh(oct_vertices, oct_indices, 192, 24);
	meshList.push_back(oct);
	calcAverageNormals(oct_indices, 24, oct_vertices, 192, 8, 5);
}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearOctaedro();
	CreateShaders();

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
	ArcoTexture = Texture("Textures/Mossy_Stone_Floor_veilfjxn_1K_BaseColor.jpg");
	ArcoTexture.LoadTextureA();
	PuertaTexture = Texture("Textures/Rusty_Metal_Sheet_tj4kedvcw_1K_BaseColor.jpg");
	PuertaTexture.LoadTextureA();
	DadoTexture = Texture("Textures/dado.png");
	DadoTexture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Dragon_M = Model();
	Dragon_M.LoadModel("Models/DragonCuerpo.fbx");
	alaDerecha_M = Model();
	alaDerecha_M.LoadModel("Models/alaDerecha.fbx");
	alaIzquierda_M = Model();
	alaIzquierda_M.LoadModel("Models/alaIzquierda.fbx");

	Arco_M = Model();
	Arco_M.LoadModel("Models/Arco.fbx");
	Letrero_M = Model();
	Letrero_M.LoadModel("Models/Letrero.fbx");
	Puerta_Derecha_M = Model();
	Puerta_Derecha_M.LoadModel("Models/Puerta_Der.fbx");
	Puerta_Izquierda_M = Model();
	Puerta_Izquierda_M.LoadModel("Models/Puerta_Izq.fbx");



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
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	movCoche = 0.0f;
	movOffset = 0.1f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;

	glm::vec3 lowerLight(0.0f, 0.0f, 0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
	float anguloPuerta = 0.0f;
	bool puertaAbierta = false;
	bool puertaAnimando = false;
	float velocidadPuerta = 5.0f;
	bool teclaEPresionada = false;
	// Puerta izquierda
	glm::vec3 posPuertaIzq = glm::vec3(3.1f, 0.0f, 3.3f);
	glm::vec3 destino1 = posPuertaIzq + glm::vec3(0.0f, 1.5f, 0.0f);
	glm::vec3 destino2 = destino1 + glm::vec3(2.6f, 0.0f, 0.0f);
	glm::vec3 objetivoPuertaIzq = posPuertaIzq;

	bool puertaIzqAbierta = false;
	bool puertaIzqAnimando = false;
	bool teclaRPresionada = false;
	float velocidadPuertaIzq = 0.2f;
	int fasePuertaIzq = 0;

	bool teclaTPresionada = false;
	bool dadoCayendo = false;
	bool dadoSubiendo = false;

	float velY = 0.0f;
	float gravedad = -9.8f;
	float velocidadSubida = 1.0f;   
	float velocidadCaida = 0.01f;   
	float alturaPiso = -1.0f;

	glm::vec3 posInicialDado(0.0f, 10.0f, 0.0f); 
	glm::vec3 posDado = posInicialDado;          

	glm::vec3 rotDado(0.0f);
	glm::vec3 velRotDado(0.0f);

	int caraFinal = 1;


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f * deltaTime;
		//dragonavance
		alaAngulo = 25.0f * sin(angulovaria / 8);

		if (avanzaDragon)
		{
			if (dragonavance > -20.0f)
			{
				dragonavance -= 0.1f * deltaTime;
			}
			else
			{
				avanzaDragon = !avanzaDragon;
			}
		}
		else
		{
			if (dragonavance < 0.0f) {
				dragonavance += 0.1f * deltaTime;
			}
			else
			{
				avanzaDragon = !avanzaDragon;
			}
		}
		if (avanza)
		{
			if (movCoche > -250.0f)
			{
				movCoche -= movOffset * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;
			}
			else
			{
				avanza = !avanza;
			}
		}
		else
		{
			if (movCoche < 300.0f) {
				movCoche += movOffset * deltaTime;
				rotllanta -= rotllantaOffset * deltaTime;
			}
			else
			{
				avanza = !avanza;
			}
		}

		// ---- Control de puerta con tecla E ----
		if (mainWindow.getsKeys()[GLFW_KEY_E])
		{
			if (!teclaEPresionada)
			{
				puertaAbierta = !puertaAbierta;
				puertaAnimando = true; 
				teclaEPresionada = true;
			}
		}
		else
		{
			teclaEPresionada = false; 
		}

		if (puertaAnimando)
		{
			if (puertaAbierta)
			{
				
				anguloPuerta -= velocidadPuerta * deltaTime;
				if (anguloPuerta <= -90.0f)
				{
					anguloPuerta = -90.0f;
					puertaAnimando = false; 
				}
			}
			else
			{
				
				anguloPuerta += velocidadPuerta * deltaTime;
				if (anguloPuerta >= 0.0f)
				{
					anguloPuerta = 0.0f;
					puertaAnimando = false; 
				}
			}
		}

		// --- Toggle de puerta izquierda ---
		if (mainWindow.getsKeys()[GLFW_KEY_R])
		{
			if (!teclaRPresionada)
			{
				puertaIzqAbierta = !puertaIzqAbierta;
				puertaIzqAnimando = true;
				fasePuertaIzq = 1; 
				teclaRPresionada = true;
			}
		}
		else
		{
			teclaRPresionada = false;
		}
		if (puertaIzqAnimando)
		{
			if (puertaIzqAbierta)
			{
				if (fasePuertaIzq == 1)
				{
					posPuertaIzq = glm::mix(posPuertaIzq, destino1, velocidadPuertaIzq * deltaTime);
					if (glm::distance(posPuertaIzq, destino1) < 0.05f)
					{
						posPuertaIzq = destino1;
						fasePuertaIzq = 2; 
					}
				}
				else if (fasePuertaIzq == 2)
				{
					posPuertaIzq = glm::mix(posPuertaIzq, destino2, velocidadPuertaIzq * deltaTime);
					if (glm::distance(posPuertaIzq, destino2) < 0.05f)
					{
						posPuertaIzq = destino2;
						puertaIzqAnimando = false; // terminó de abrir
					}
				}
			}
			else
			{
				if (fasePuertaIzq == 1)
				{
					posPuertaIzq = glm::mix(posPuertaIzq, destino1, velocidadPuertaIzq * deltaTime);
					if (glm::distance(posPuertaIzq, destino1) < 0.05f)
					{
						posPuertaIzq = destino1;
						fasePuertaIzq = 2;
					}
				}
				else if (fasePuertaIzq == 2)
				{
					posPuertaIzq = glm::mix(posPuertaIzq, glm::vec3(3.1f, 0.0f, 3.3f), velocidadPuertaIzq * deltaTime);
					if (glm::distance(posPuertaIzq, glm::vec3(3.1f, 0.0f, 3.3f)) < 0.05f)
					{
						posPuertaIzq = glm::vec3(3.1f, 0.0f, 3.3f);
						puertaIzqAnimando = false; // terminó de cerrar
					}
				}
			}
		}

		// --- Alterna entre caer y subir ---
		if (mainWindow.getsKeys()[GLFW_KEY_T])
		{
			if (!teclaTPresionada)
			{
				teclaTPresionada = true;

				if (!dadoCayendo && !dadoSubiendo && posDado.y >= posInicialDado.y - 0.1f)
				{
					dadoCayendo = true;
					velY = 0.0f;
					rotDado = glm::vec3(0.0f);
					velRotDado = glm::vec3(
						200.0f + rand() % 50,
						150.0f + rand() % 50,
						180.0f + rand() % 50
					);

					caraFinal = 1 + rand() % 8;
				}
				else if (!dadoCayendo && !dadoSubiendo && posDado.y <= alturaPiso + 0.1f)
				{
					dadoSubiendo = true;
				}
			}
		}
		else
		{
			teclaTPresionada = false;
		}


		// ---- Movimiento ----
		if (dadoCayendo)
		{
			velY += gravedad * deltaTime * velocidadCaida;
			posDado.y += velY * deltaTime;
			rotDado += velRotDado * deltaTime;

			if (posDado.y <= alturaPiso)
			{
				posDado.y = alturaPiso;
				dadoCayendo = false;

				switch (caraFinal)
				{
				case 1: rotDado = glm::vec3(0.0f, 90.0f, 0.0f); break;
				case 2: rotDado = glm::vec3(0.0f, 0.0f, 0.0f); break;
				case 3: rotDado = glm::vec3(0.0f, 0.0f, -90.0f); break;
				case 4: rotDado = glm::vec3(0.0f, 180.0f, 0.0f); break;
				case 5: rotDado = glm::vec3(180.0f, 0.0f, 0.0f); break;
				case 6: rotDado = glm::vec3(90.0f, 0.0f, 0.0f); break;
				case 7: rotDado = glm::vec3(0.0f, -90.0f, 270.0f); break;
				case 8: rotDado = glm::vec3(180.0f, -10.0f, 90.0f); break;
				}

			}
		}

		if (dadoSubiendo)
		{
			posDado.y += velocidadSubida * deltaTime;

			if (posDado.y >= posInicialDado.y)
			{
				posDado.y = posInicialDado.y;
				dadoSubiendo = false;
			}
		}

		timer += deltaTime;
		if (timer >= 100.0f) {
			timer = 0.0f;
			cambio = !cambio;
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
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//Reinicializando variables cada ciclo de reloj
		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);

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



		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche - 50.0f, 0.5f, -2.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//dragonavance para moverlo hacia adelante y atrás en X  5.0f+sin(glm::radians(angulovaria*5))
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(dragonavance, 5.0f + sin(glm::radians(angulovaria * 5)), 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		/*color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dragon_M.RenderModel();

		//ala derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.8f, 0.0f, 3.0f));
		model = glm::rotate(model, glm::radians(alaAngulo), glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		alaDerecha_M.RenderModel();
		//ala izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.5f, 0.5f, 3.0f));
		model = glm::rotate(model, -glm::radians(alaAngulo), glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		alaIzquierda_M.RenderModel();

		//Arco
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, -2.0f, 10.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ArcoTexture.UseTexture();
		Arco_M.RenderModel();

		//Puertas
		//Puerta derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.1f, 0.0f, 3.4f));
		model = glm::rotate(model, -glm::radians(anguloPuerta), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaTexture.UseTexture();
		Puerta_Derecha_M.RenderModel();
		//Puerta izquierda
		model = modelaux;
		model = glm::translate(model, posPuertaIzq);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaTexture.UseTexture();
		Puerta_Izquierda_M.RenderModel();

		//Letrero
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Letrero_M.RenderModel();

		//Dado
		model = glm::mat4(1.0f);
		model = glm::translate(model, posDado);
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(rotDado.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotDado.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotDado.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		DadoTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh(); 



		/*color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
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
		/*toffsetflechau += 0.1;
		toffsetflechav = 0.000;*/
		toffsetflechau += 0.2 * deltaTime;
		//para que no se desborde la variable
		/*if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;*/
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

		//plano con todos los números
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//número 1
		//toffsetnumerou = 0.0;
		//toffsetnumerov = 0.0;
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		for (int i = 1; i < 4; i++)
		{
			//números 2-4
			toffsetnumerou += 0.25;
			toffsetnumerov = 0.0;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-10.0f - (i * 3.0), 2.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();

		}

		for (int j = 1; j < 5; j++)
		{
			//números 5-8
			toffsetnumerou += 0.25;
			toffsetnumerov = -0.33;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-7.0f - (j * 3.0), 5.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();
		}


		//número cambiante 
		/*
		¿Cómo hacer para que sea a una velocidad visible?
		*/
		toffsetnumerocambiau += 0.25;
		if (toffsetnumerocambiau > 1.0)
			toffsetnumerocambiau = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		//cambiar automáticamente entre textura número 1 y número 2
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		if (cambio)
			Numero1Texture.UseTexture();
		else
			Numero2Texture.UseTexture();

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();




		glDisable(GL_BLEND);





		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
