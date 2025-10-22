/*
Práctica 8: Iluminación 2 
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

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

Texture parabrisasTexture;
Texture llantaTexture;
Texture cofreTexture;
Texture parrillaTexture;
Texture rinTexture;

Texture lumbreraTexture;

Texture dado8Texture;
Texture estuataTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

Model parabrisas_M;
Model llanta;
Model cofre_M;
Model parrilla_M;
Model rin;
Model gtr_M;

Model lumbrera;
Model estuata_M;

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
SpotLight spotLights2[MAX_SPOT_LIGHTS];

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
	float mvnt=0;
	float mvntCarro = 0;

	CreateObjects();
	CrearOctaedro();
	CreateShaders();

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
	parabrisasTexture = Texture("Textures/ojos.png");
	parabrisasTexture.LoadTextureA();
	llantaTexture = Texture("Textures/llanta.png");
	llantaTexture.LoadTextureA();
	cofreTexture = Texture("Textures/cofre.png");
	cofreTexture.LoadTextureA();
	parrillaTexture = Texture("Textures/parrilla.png");
	parrillaTexture.LoadTextureA();
	rinTexture = Texture("Textures/fibra.png");
	rinTexture.LoadTextureA();

	lumbreraTexture = Texture("Textures/lumbrera.png");
	lumbreraTexture.LoadTextureA();

	dado8Texture = Texture("Textures/dado.png");
	dado8Texture.LoadTextureA();

	estuataTexture = Texture("Textures/Liberty-GreenBronze-1.bmp");


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	parabrisas_M = Model();
	parabrisas_M.LoadModel("Models/Parabrisas.fbx");
	llanta = Model();
	llanta.LoadModel("Models/caucho_FD.fbx");
	rin = Model();
	rin.LoadModel("Models/rin_FD.fbx");
	cofre_M = Model();
	cofre_M.LoadModel("Models/cofre.fbx");
	parrilla_M = Model();
	parrilla_M.LoadModel("Models/Parrilla.fbx");
	gtr_M = Model();
	gtr_M.LoadModel("Models/GTR.fbx");

	lumbrera = Model();
	lumbrera.LoadModel("Models/Lumbrera.fbx");
	estuata_M = Model();
	estuata_M.LoadModel("Models/estuata.fbx");
	

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
		-15.0f, 1.5f, 1.5f,
		0.1f, 0.1f, 1.0f);
	pointLightCount++;

	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,   
		0.1f, 1.5f,                             
		1.0f, 1.0f, 1.0f,                        
		0.3f, 0.1f, 0.05f);                         
	pointLightCount++;

	pointLights[2] = PointLight(
		1.0f, 0.74f, 0.20f,   
		0.35f, 2.8f,          
		10.0f, 1.5f, 1.5f,    
		1.0f, 0.045f, 0.0075f 
	);
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

	//Calabera
	spotLights[1] = SpotLight(1.0f, 0.0f, 0.0f,
		0.0f, 2.0f,
		7.0f, 0.5f, 2.5f,
		0.0f, -0.1f, 1.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//Faros del coche
	spotLights[2] = SpotLight(0.0f, 0.0f, 1.0f,
		0.0f, 2.0f,
		7.0f, 0.5f, 2.5f,
		0.0f, -0.1f, -1.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//luz Cofre
	spotLights[3] = SpotLight(
		1.0f, 0.75f, 0.25f,   
		0.4f, 2.0f,           
		0.0f, 0.0f, 0.0f,     
		0.0f, -1.0f, 0.0f,    
		1.0f, 0.02f, 0.01f,   
		10.0f                 
	);
	spotLightCount++;

	//Luces Truco
	spotLights[1].TurnOff();
	spotLights[2].TurnOff();
	
	
	//se crean mas luces puntuales y spotlight 

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelPared(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec4 FaroFrrontal(+0.0f, 20.0f, 110.0f, 1.0f);
	glm::vec4 Calavera(+0.0f, 20.0f, -140.0f, 1.0f);
	
	glm::vec3 dir_local = glm::normalize(glm::vec3(0.0f, -0.15f, 1.0f));
	glm::vec4 foco_local = glm::vec4(0.0f, 0.0f, 3.0f,1.0f);
	static bool teclaPresionada = false;
	static bool teclaPresionada2 = false;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

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



		

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//Instancia del coche 
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), 0.5f, -3.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Padred
		modelPared = glm::mat4(1.0);
		modelPared = glm::translate(modelPared, glm::vec3(0.0f, 5.0f, 15.0f));
		modelPared = glm::scale(modelPared, glm::vec3(1.0f, 2.0f, 1.0f));
		modelPared = glm::rotate(modelPared, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelPared = glm::rotate(modelPared, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPared));
		brickTexture.UseTexture();
		meshList[2]->RenderMesh();

		//otra pared
		modelPared = glm::translate(modelPared, glm::vec3(0.0f, 30.0f, 0.0f));
		modelPared = glm::rotate(modelPared, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPared));
		brickTexture.UseTexture();
		meshList[2]->RenderMesh();

		//GTR
		//Instancia del coche 
		model = glm::mat4(1.0);
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::translate(model, glm::vec3(-100.0f, 0.0f, 0.0f));
		mvntCarro = mainWindow.getarticulacion11() + mainWindow.getarticulacion5();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, mvntCarro));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		gtr_M.RenderModel();

		spotLights[2].SetFlash(modelaux* FaroFrrontal, glm::normalize(glm::mat3(modelaux)* dir_local));
		spotLights[1].SetFlash(modelaux* Calavera, glm::normalize(glm::mat3(modelaux) * -dir_local));
		
		/*
		2.Agregar luz de tipo spotlight para el coche de tal forma que al avanzar
		(mover con teclado hacia dirección de X negativa/positiva ) ilumine con un
		spotlight hacia adelante y al retroceder ((mover con teclado
		hacia dirección de X opuesta) ilumine con un spotlight hacia atrás.
		Son dos spotlights diferentes que se prenderán
		y apagarán de acuerdo a alguna bandera asignada por ustedes.
		*/

		if (mainWindow.getsKeys()[GLFW_KEY_I])
		{
			spotLights[2].TurnOn();
			spotLights[1].TurnOff();
		}
		
		if (mainWindow.getsKeys()[GLFW_KEY_K])
		{
			spotLights[1].TurnOn();
			spotLights[2].TurnOff();
		}

		
			
		//llanta Frente Derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 85.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaTexture.UseTexture();
		llanta.RenderModel();
		//rin
		rinTexture.UseTexture();
		rin.RenderModel();

		//llanta Frente Izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 85.0f));
		//model = glm::rotate(model, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaTexture.UseTexture();
		llanta.RenderModel();
		//rin
		rinTexture.UseTexture();
		rin.RenderModel();

		//llanta Trasera Derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -80.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaTexture.UseTexture();
		llanta.RenderModel();
		//rin
		rinTexture.UseTexture();
		rin.RenderModel();

		//llanta Trasera Izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -80.0f));
		//model = glm::rotate(model, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaTexture.UseTexture();
		llanta.RenderModel();
		//rin
		rinTexture.UseTexture();
		rin.RenderModel();

		//parabrisas
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		parabrisasTexture.UseTexture();
		parabrisas_M.RenderModel();

		//cofre
		/*
		1.Agregar un spotlight (que no sea luz de color blanco ni azul)
		que parta del cofre de su coche y al abrir y cerrar el cofre ilumine
		en esa dirección.
		*/
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 50.0f, 50.0f));
		model = glm::rotate(model, glm::radians(-mainWindow.getarticulacion10()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cofreTexture.UseTexture();
		cofre_M.RenderModel();
		spotLights[3].SetFlash(glm::vec3(model* glm::vec4(0.0f, -4.0f, 40.0f, 1.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
		
		//parrilla
		model = modelaux;
		model = glm::translate(model, glm::vec3(83.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		parrillaTexture.UseTexture();
		parrilla_M.RenderModel();
	
		//Helicopter
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 5.0f, 6.0));
		mvnt = mainWindow.getarticulacion12() + mainWindow.getarticulacion13();
		model = glm::translate(model, glm::vec3(mvnt,0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

	  
		

		//Lumbrera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0, -0.5f, 3.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lumbreraTexture.UseTexture();
		lumbrera.RenderModel();

		pointLights[1].SetPosition(model * foco_local);

		
		if (mainWindow.getsKeys()[GLFW_KEY_P])
		{
			if (!teclaPresionada)
			{
				pointLights[1].Toggle();  
				teclaPresionada = true;   
			}
		}
		else
		{
			teclaPresionada = false; 
		}


		//Dado8
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.5f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dado8Texture.UseTexture();
		meshList[4]->RenderMesh();


		
		/*
		3.- Agregar otra luz de tipo puntual ligada a un modelo elegido por ustedes 
		(no lámpara) y que puedan prender y apagar de forma independiente con teclado 
		tanto la luz de la lámpara como la luz de este modelo ( la luz de la lámpara 
		debe de ser puntual, si la crearon spotlight en su reporte 7 tienen que cambiarla a luz puntual)
		*/

		//Estuata
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, -10.0f));
		model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		estuataTexture.UseTexture();
		estuata_M.RenderModel();

		//luz estatua
		pointLights[2].SetPosition(glm::vec3(model * glm::vec4(-0.3f, 1.4f, 0.3f, 1.0f)));
		if (mainWindow.getsKeys()[GLFW_KEY_O])
		{
			if (!teclaPresionada2)
			{
				pointLights[2].Toggle();
				teclaPresionada2 = true;
			}
		}
		else
		{
			teclaPresionada2 = false;
		}



		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
