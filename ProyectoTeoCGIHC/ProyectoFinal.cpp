
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

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animaci�n
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
float movBrazos;
bool avanza, juegoGlobo = false, juegoTopo = false, juegoBolos = false, juegoHacha = false, juegoDados = false, juegoBateo = false;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture pisoTexture;
Texture dadoTexture;


Model Kitt_M;
Model Llanta_M;
Model Dragon_M;
Model Tiamat_M;

Model Dados_Mesa;
Model EntradaPuerta;
Model EntradaCartel;
Model EntradaMuro;
Model Camino;
Model BateoJaula;
Model BateoPiso;
Model BateoLanzador;
Model BateoBola;
Model BateoBat;
Model BateoRed;
Model GloboPared;
Model GloboDardo;
Model Globo1;
Model Globo2;
Model Globo3;
Model Globo4;
Model Globo5;
Model Globo6;
Model GloboMesa;
Model BolichePorteria;
Model Pin1;
Model Pin2;
Model Pin3;
Model Pin4;
Model Pin5;
Model Pin6;
Model Bola;
Model Diana;
Model Hacha;
Model TopoMaquina;
Model Topo;
Model TopoMazo;
//Estacionamiento
Model ParkinLot;
//Puestos
Model Papas;
Model Hamburguesa;
Model Helado;
Model Carrusel;
Model baseLuces;
Model Benzon;//Benzon
Model MuscleMan; //Muscle man
//Mordecai
Model Mordecai;
Model PieIzqMordecai;
Model PieDerMordecai;
Model BrazoIzqMordecai;
Model BrazoDerMordecai;
//Gengar
Model Gengar;
Model GengarManoIzq;
Model GengarManoDer;
Model GengarPataIzq;
Model GengarPataDer;

//Sol y luna
Model Sol;
Model Luna;

//Trampolin
Model Trampolin;

//Mesas
Model MesaComida;
//Bancas
Model Bancas;
Model BancasTematicas;

Skybox skybox;
Skybox skybox2;
Skybox skybox3;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
DirectionalLight mainLight2;
DirectionalLight mainLight3;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLights2[MAX_SPOT_LIGHTS];
SpotLight spotLights3[MAX_SPOT_LIGHTS];
SpotLight spotLights4[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//c�lculo del promedio de las normales para sombreado de Phong
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
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};

	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.51f,  0.34f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.75f,	0.34f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.75f,	0.66f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.51f,	0.66f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.77f,  0.34f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,	0.34f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		1.0f,	0.66f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.77f,	0.66f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,	0.34f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.24f,	0.34f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.24f,	0.66f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	0.66f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.51f,  0.0f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.51f,	0.32f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.75f,	0.32f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.75f,	0.0f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.51f,  1.0f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.51f,	0.68f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.75f,	0.68f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.75f,	1.0f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);
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
	CrearDado();
	CreateShaders();
	bool noche = false;

	//camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	dadoTexture = Texture("Textures/dadoPokemon.jpg");
	dadoTexture.LoadTextureA();
	pisoTexture = Texture("Textures/pokemonPasto.jpg");
	pisoTexture.LoadTexture();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Dragon_M = Model();
	Dragon_M.LoadModel("Models/17174_Tiamat_new.obj");

	Dados_Mesa = Model();
	Dados_Mesa.LoadModel("Models/Dados_Mesa.obj");
	//Estructuras
	ParkinLot = Model();
	ParkinLot.LoadModel("Models/ParkinLot.obj");
	EntradaPuerta = Model();
	EntradaPuerta.LoadModel("Models/Entrada_Puerta.obj");
	EntradaCartel = Model();
	EntradaCartel.LoadModel("Models/Entrada_Cartel.obj");
	EntradaMuro = Model();
	EntradaMuro.LoadModel("Models/Entrada_Muro.obj");
	Camino = Model();
	Camino.LoadModel("Models/Estructura_Camino.obj");
	//Bateo
	BateoBat = Model();
	BateoBat.LoadModel("Models/Bateo_Bate.obj");
	BateoBola = Model();
	BateoBola.LoadModel("Models/Bateo_Pelota.obj");
	BateoJaula = Model();
	BateoJaula.LoadModel("Models/Bateo_Jaula.obj");
	BateoLanzador = Model();
	BateoLanzador.LoadModel("Models/Bateo_Lanzador.FBX");
	BateoPiso = Model();
	BateoPiso.LoadModel("Models/Bateo_Piso.obj");
	BateoRed = Model();
	BateoRed.LoadModel("Models/Bateo_Red.obj");
	//Globos
	GloboMesa = Model();
	GloboMesa.LoadModel("Models/Globos_Mesa.obj");
	GloboPared = Model();
	GloboPared.LoadModel("Models/Globos_Pared.obj");
	GloboDardo = Model();
	GloboDardo.LoadModel("Models/Dardos_Dardo.obj");
	Globo1 = Model();
	Globo1.LoadModel("Models/Globos_GloboR1.obj");
	Globo2 = Model();
	Globo2.LoadModel("Models/Globos_GloboR2.obj");
	Globo3 = Model();
	Globo3.LoadModel("Models/Globos_GloboR3.obj");
	Globo4 = Model();
	Globo4.LoadModel("Models/Globos_GloboR4.obj");
	Globo5 = Model();
	Globo5.LoadModel("Models/Globos_GloboR5.obj");
	Globo6 = Model();
	Globo6.LoadModel("Models/Globos_GloboR6.obj");

	//Boliche
	BolichePorteria = Model();
	BolichePorteria.LoadModel("Models/Boliche_Porteria.obj");
	Bola = Model();
	Bola.LoadModel("Models/Boliche_Bola.obj");
	Pin1 = Model();
	Pin1.LoadModel("Models/Boliche_Pin1.obj");
	Pin2 = Model();
	Pin2.LoadModel("Models/Boliche_Pin2.obj");
	Pin3 = Model();
	Pin3.LoadModel("Models/Boliche_Pin3.obj");
	Pin4 = Model();
	Pin4.LoadModel("Models/Boliche_Pin4.obj");
	Pin5 = Model();
	Pin5.LoadModel("Models/Boliche_Pin5.obj");
	Pin6 = Model();
	Pin6.LoadModel("Models/Boliche_Pin6.obj");

	//Hacha
	Diana = Model();
	Diana.LoadModel("Models/Hacha_Diana.obj");
	Hacha = Model();
	Hacha.LoadModel("Models/Hacha_Hacha.obj");

	//Topos
	TopoMaquina = Model();
	TopoMaquina.LoadModel("Models/Topo_Maquina.obj");
	Topo = Model();
	Topo.LoadModel("Models/Diglett.DAE");
	TopoMazo = Model();
	TopoMazo.LoadModel("Models/Topo_Mazo.obj");


	//Puestos
	Papas = Model();
	Papas.LoadModel("Models/CarritoPapas.obj");
	Hamburguesa = Model();
	Hamburguesa.LoadModel("Models/CarritoComida.obj");
	Helado = Model();
	Helado.LoadModel("Models/CarritoHelados.obj");
	Carrusel = Model();
	Carrusel.LoadModel("Models/Carrusel.obj");
	baseLuces = Model();
	baseLuces.LoadModel("Models/baseLuces.obj");
	MuscleMan = Model();
	MuscleMan.LoadModel("Models/MuscleMan.obj");
	Benzon = Model();//Benzon
	Benzon.LoadModel("Models/Benzon.obj");
	//Mordecai
	Mordecai = Model();
	Mordecai.LoadModel("Models/TorsoMordecai.obj");
	BrazoDerMordecai = Model();
	BrazoDerMordecai.LoadModel("Models/BrazoDerMordecai.obj");
	BrazoIzqMordecai = Model();
	BrazoIzqMordecai.LoadModel("Models/BrazoIzqMordecai.obj");
	PieIzqMordecai = Model();
	PieIzqMordecai.LoadModel("Models/PiernaIzqMordecai.obj");
	PieDerMordecai = Model();
	PieDerMordecai.LoadModel("Models/PiernaDerMordecai.obj");
	//Gengar
	Gengar = Model();
	Gengar.LoadModel("Models/gengarCuerpo.dae");
	GengarManoIzq = Model();
	GengarManoIzq.LoadModel("Models/gengarBrazoIzq.dae");
	GengarManoDer = Model();
	GengarManoDer.LoadModel("Models/gengarBrazoDer.dae");
	GengarPataIzq = Model();
	GengarPataIzq.LoadModel("Models/gengarPiernaIzq.dae");
	GengarPataDer = Model();
	GengarPataDer.LoadModel("Models/gengarPiernaDer.dae");

	//Trampolin
	Trampolin = Model();
	Trampolin.LoadModel("Models/TrampolinRuggrats.obj");

	//Mesas
	MesaComida = Model();
	MesaComida.LoadModel("Models/mesasComida.obj");

	//Bancas
	Bancas = Model();
	Bancas.LoadModel("Models/bancaPS.obj");
	BancasTematicas = Model();
	BancasTematicas.LoadModel("Models/bancaP.obj");

	//Sol
	Sol = Model();
	Sol.LoadModel("Models/sol.obj");
	//Luna
	Luna = Model();
	Luna.LoadModel("Models/luna.obj");

	std::vector<std::string> skyboxFaces;
	std::vector<std::string> skyboxFaces2;
	std::vector<std::string> skyboxFaces3;
	skyboxFaces.push_back("Textures/Skybox/skybox_right.jpg"); //right
	skyboxFaces.push_back("Textures/Skybox/skybox_left.jpg"); // left
	skyboxFaces.push_back("Textures/Skybox/skybox_bottom.jpg"); // Bottom
	skyboxFaces.push_back("Textures/Skybox/skybox_top.jpg"); // Top
	skyboxFaces.push_back("Textures/Skybox/skybox_near.jpg"); // Front (Near)
	skyboxFaces.push_back("Textures/Skybox/skybox_far.jpg"); // Back (Far)

	skyboxFaces2.push_back("Textures/Skybox/skybox_rightT.jpg"); //right
	skyboxFaces2.push_back("Textures/Skybox/skybox_leftT.jpg"); // left
	skyboxFaces2.push_back("Textures/Skybox/skybox_bottomT.jpg"); // Bottom
	skyboxFaces2.push_back("Textures/Skybox/skybox_topT.jpg"); // Top
	skyboxFaces2.push_back("Textures/Skybox/skybox_nearT.jpg"); // Front (Near)
	skyboxFaces2.push_back("Textures/Skybox/skybox_farT.jpg"); // Back (Far)

	skyboxFaces3.push_back("Textures/Skybox/skybox_rightN.jpg"); //right
	skyboxFaces3.push_back("Textures/Skybox/skybox_leftN.jpg"); // left
	skyboxFaces3.push_back("Textures/Skybox/skybox_bottomN.jpg"); // Bottom
	skyboxFaces3.push_back("Textures/Skybox/skybox_topN.jpg"); // Top
	skyboxFaces3.push_back("Textures/Skybox/skybox_nearN.jpg"); // Front (Near)
	skyboxFaces3.push_back("Textures/Skybox/skybox_farN.jpg"); // Back (Far)


	skybox = Skybox(skyboxFaces);
	skybox2 = Skybox(skyboxFaces2);
	skybox3 = Skybox(skyboxFaces3);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.5f, 0.5f,
		0.0f, 0.0f, -1.0f);

	mainLight2 = DirectionalLight(1.0f, 0.2f, 0.2f,
		0.5f, 0.5f,
		0.0f, 0.0f, -1.0f);

	mainLight3 = DirectionalLight(0.3f, 0.1f, 0.5f,
		0.5f, 0.5f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//Spotlights contador
	unsigned int spotLightCount = 0;
	unsigned int spotLightCount2 = 0;
	unsigned int spotLightCount3 = 0;
	unsigned int spotLightCount4 = 0;
	//lamparas
	spotLights[0] = SpotLight(0.5f, 0.3f, 1.0f,
		1.0f, 2.0f,
		22.0f, 78.0f, 165.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;

	//lamparas
	spotLights[1] = SpotLight(0.3f, 0.0f, 1.0f,
		1.0f, 2.0f,
		27.0f, 74.0f, -136.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;

	//lamparas
	spotLights[2] = SpotLight(0.5f, 1.0f, 0.3f,
		1.0f, 2.0f,
		236.0f, 77.0f, -30.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;

	//Cmabio luz
	spotLights4[0] = SpotLight(0.9f, 0.0f, 1.0f,
		1.0f, 2.0f,
		22.0f, 78.0f, 165.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount4++;

	//lamparas
	spotLights4[1] = SpotLight(1.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		27.0f, 74.0f, -136.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount4++;

	//lamparas
	spotLights4[2] = SpotLight(0.9f, 0.1f, 0.7f,
		1.0f, 2.0f,
		236.0f, 77.0f, -30.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount4++;
	//Dosdo conjunto de luces
	spotLights2[0] = SpotLight(0.5f, 0.5f, 0.0f,
		1.0f, 0.09f,
		163.0f, 90.0f, 21.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		65.0f);
	spotLightCount2++;

	//lamparas
	spotLights2[1] = SpotLight(0.5f, 0.5f, 0.0f,
		1.0f, 0.09f,
		46.0f, 90.0f, 21.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		65.0f);
	spotLightCount2++;

	//lamparas
	spotLights2[2] = SpotLight(0.5f, 0.5f, 0.0f,
		1.0f, 0.09f,
		-62.0f, 90.0f, 21.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		65.0f);
	spotLightCount2++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	movCoche = 0.0f;
	movOffset = 0.9f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;
	float velocidad = 5.0f;
	avanza = true;
	////Loop mientras no se cierra la ventana

	//Luces juegos 
	glm::vec3 targetPositionGlobos = glm::vec3(160.0f, 4.0f, 55.0f); // Posici�n objetivo
	glm::vec3 targetPositionBoliche = glm::vec3(140.0f, 12.0f, -43.0f); // Posici�n objetivo
	glm::vec3 targetPositionBateo = glm::vec3(-35.0f, 14.0f, -48.0f); // Posici�n objetivo
	glm::vec3 targetPositionTopo = glm::vec3(59.0f, 11.0f, -34.0f); // Posici�n objetivo
	glm::vec3 targetPositionDados = glm::vec3(158.0f, 13.0f, 2.0f); // Posici�n objetivo
	glm::vec3 targetPositionHacha = glm::vec3(102.0f, 7.0f, -40.0f); // Posici�n objetivo


	float proximityRange = 25.0f; // Rango de proximidad para activar la luz
	bool lightOn = false; // Estado de la luz
	bool lightOn2 = false;
	bool lightOn3 = false;
	bool lightOn4 = false;
	bool lightOn5 = false;
	bool lightOn6 = false;


	// Definir posici�n fija de la c�mara
	glm::vec3 staticCameraPosition = glm::vec3(0.0f, 300.0f, 0.0f); // Posici�n alta
	glm::vec3 staticCameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);    // Mirando hacia el origen
	glm::vec3 upVector = glm::vec3(0.0f, 0.0f, -1.0f);             // Vector "arriba" para la c�mara


	camera = Camera(glm::vec3(102.0f, 7.0f, 143.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f * deltaTime;
		if (avanza) {
			if (movCoche > -250.0f)
			{
				movCoche -= movOffset * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;
			}
			else {
				avanza = false;
			}
		}
		else {
			if (movCoche < 350.0f)
			{
				movCoche += movOffset * deltaTime;
				rotllanta -= rotllantaOffset * deltaTime;
			}
			else {
				avanza = true;
			}
		}


		//Recibir eventos del usuario
		glfwPollEvents();
		//camera.keyControl(mainWindow.getsKeys(), deltaTime);
		//camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 viewMatrix;

		switch (mainWindow.getTipoCam())
		{
		case 1: // C�mara est�tica
			viewMatrix = glm::lookAt(
				staticCameraPosition, // Posici�n de la c�mara
				staticCameraTarget,   // Punto al que mira la c�mara
				upVector              // Vector "arriba" de la c�mara
			);
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
			break;
		case 2: // C�mara din�mica  
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());


			glm::vec3 pos = camera.getCameraPosition();
			printf("Posición de la cámara: X=%.2f, Y=%.2f, Z=%.2f\n", pos.x, pos.y, pos.z);
			viewMatrix = camera.calculateViewMatrix();

			
			break;
            
		}


		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

		if (movCoche < -150) {
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
			noche = false;
		}
		else if (movCoche < 65) {
			skybox2.DrawSkybox(camera.calculateViewMatrix(), projection);
			noche = false;
		}
		else if (movCoche > 200) {
			skybox3.DrawSkybox(camera.calculateViewMatrix(), projection);
			noche = true;
		}
		else {
			// Dibuja un skybox por defecto
			skybox2.DrawSkybox(camera.calculateViewMatrix(), projection);
			noche = false;
		}
		shaderList[0].UseShader();

		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//// luz ligada a la c�mara de tipo flash
		//glm::vec3 lowerLight = camera.getCameraPosition();
		//lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());


		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

		if (movCoche < -150) {
			shaderList[0].SetDirectionalLight(&mainLight);
		}
		else if (movCoche < 65) {
			shaderList[0].SetDirectionalLight(&mainLight2);
		}
		else if (movCoche > 200)
		{
			shaderList[0].SetDirectionalLight(&mainLight3);
		}
		else {
			shaderList[0].SetDirectionalLight(&mainLight2);
		}


		if (mainWindow.getEncender() == false) {
			// Obtener la posici�n de la c�mara
			glm::vec3 cameraPosition = camera.getCameraPosition();

			// Calcular la distancia entre la c�mara y la posici�n objetivo
			//Globos
			float distance = glm::distance(cameraPosition, targetPositionGlobos);
			float distance2 = glm::distance(cameraPosition, targetPositionTopo);
			float distance3 = glm::distance(cameraPosition, targetPositionBateo);
			float distance4 = glm::distance(cameraPosition, targetPositionBoliche);
			float distance5 = glm::distance(cameraPosition, targetPositionDados);
			float distance6 = glm::distance(cameraPosition, targetPositionHacha);
			// Encender o apagar la luz seg�n la distancia
			//Globos
			if (distance <= proximityRange) {
				if (!lightOn) {
					// Encender la luz
					spotLights3[0] = SpotLight(0.6f, 0.3f, 0.7f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						195.0f, 35.0f, 57.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[1] = SpotLight(0.1f, 0.5f, 0.5f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						160.0f, 35.0f, 68.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[2] = SpotLight(0.8f, 0.8f, 0.2f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						160.0f, 35.0f, 38.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					lightOn = true;
				}
				shaderList[0].SetSpotLights(spotLights3, spotLightCount3);
			}
			else {
				if (lightOn) {
					// Apagar la luz
					spotLights3[0] = SpotLight(0.0f, 0.0f, 0.0f, // Color apagado
						0.0f, 0.0f,       // Intensidad apagada
						195.0f, 35.0f, 57.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[1] = SpotLight(0.0f, 0.0f, 0.0f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						160.0f, 35.0f, 68.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[2] = SpotLight(0.0f, 0.0f, 0.0f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						160.0f, 35.0f, 38.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;
					lightOn = false;
				}
				shaderList[0].SetSpotLights(spotLights3, spotLightCount3);
			}

			//Topo
			if (distance2 <= proximityRange) {
				if (!lightOn2) {
					// Encender la luz
					spotLights3[0] = SpotLight(1.0f, 0.3f, 0.7f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						60.0f, 20.0f, -44.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[1] = SpotLight(0.0f, 1.0f, 0.0f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						70.0f, 20.0f, -44.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[2] = SpotLight(0.0f, 0.0f, 1.0f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						46.0f, 20.0f, -44.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					lightOn2 = true;
				}
				shaderList[0].SetSpotLights(spotLights3, spotLightCount3);
			}
			else {
				if (lightOn2) {
					// Apagar la luz
					spotLights3[0] = SpotLight(0.0f, 0.0f, 0.0f, // Color de la luz
						0.0f, 0.0f,       // Intensidad apagada
						60.0f, 20.0f, -44.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;


					spotLights3[1] = SpotLight(0.0f, 0.0f, 0.0f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						70.0f, 20.0f, -44.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[2] = SpotLight(0.0f, 0.0f, 0.0f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						46.0f, 20.0f, -44.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;


					lightOn2 = false;
				}
				shaderList[0].SetSpotLights(spotLights3, spotLightCount3);
			}

			//Bateo
			if (distance3 <= proximityRange) {
				if (!lightOn3) {
					// Encender la luz
					spotLights3[0] = SpotLight(0.6f, 0.3f, 0.7f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						-66.0f, 63.0f, -44.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[1] = SpotLight(1.0f, 0.0f, 0.0f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						-40.0f, 63.0f, -44.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[2] = SpotLight(0.0f, 0.0f, 1.0f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						-78.0f, 63.0f, -44.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;
					lightOn3 = true;
				}
				shaderList[0].SetSpotLights(spotLights3, spotLightCount3);
			}
			else {
				if (lightOn3) {
					// Apagar la luz
					spotLights3[0] = SpotLight(0.0f, 0.0f, 0.0f, // Color apagado
						0.0f, 0.0f,       // Intensidad apagada
						-66.0f, 63.0f, -44.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;
					
					spotLights3[1] = SpotLight(0.0f, 0.0f, 0.0f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						-40.0f, 63.0f, -44.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[2] = SpotLight(0.0f, 0.0f, 0.0f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						-78.0f, 63.0f, -44.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					lightOn3 = false;
				}
				shaderList[0].SetSpotLights(spotLights3, spotLightCount3);
			}
			//Boliche
			if (distance4 <= proximityRange) {
				if (!lightOn4) {
					// Encender la luz
					spotLights3[0] = SpotLight(0.6f, 0.3f, 0.7f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						141.0f, 35.0f, -62.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[1] = SpotLight(0.3f, 0.6f, 0.1f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						141.0f, 35.0f, -37.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[2] = SpotLight(0.9f, 0.3f, 0.7f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						141.0f, 35.0f, -80.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					lightOn4 = true;
				}
				shaderList[0].SetSpotLights(spotLights3, spotLightCount3);
			}
			else {
				if (lightOn4) {
					// Apagar la luz
					spotLights3[0] = SpotLight(0.0f, 0.0f, 0.0f, // Color apagado
						0.0f, 0.0f,       // Intensidad apagada
						141.0f, 35.0f, -62.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[1] = SpotLight(0.0f, 0.0f, 0.0f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						141.0f, 35.0f, -37.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[2] = SpotLight(0.0f, 0.0f, 0.0f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						141.0f, 35.0f, -80.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					lightOn4 = false;
				}
				shaderList[0].SetSpotLights(spotLights3, spotLightCount3);
			}
			//Dados
			if (distance5 <= proximityRange) {
				if (!lightOn5) {
					// Encender la luz
					spotLights3[0] = SpotLight(0.6f, 0.3f, 0.7f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						166.0f, 25.0f, 5.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[1] = SpotLight(0.2f, 0.3f, 0.4f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						140.0f, 25.0f, 5.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[2] = SpotLight(0.9f, 0.1f, 0.7f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						188.0f, 25.0f, 5.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;
					lightOn5 = true;
				}
				shaderList[0].SetSpotLights(spotLights3, spotLightCount3);
			}
			else {
				if (lightOn5) {
					// Apagar la luz
					spotLights3[0] = SpotLight(0.0f, 0.0f, 0.0f, // Color apagado
						0.0f, 0.0f,       // Intensidad apagada
						166.0f, 19.0f, 5.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[1] = SpotLight(0.0f, 0.0f, 0.0f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						140.0f, 25.0f, 5.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[2] = SpotLight(0.0f, 0.0f, 0.0f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						188.0f, 25.0f, 5.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					lightOn5 = false;
				}
				shaderList[0].SetSpotLights(spotLights3, spotLightCount3);
			}
			//Hacha
			if (distance6 <= proximityRange) {
				if (!lightOn6) {
					// Encender la luz
					spotLights3[0] = SpotLight(0.6f, 0.3f, 0.7f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						110.0f, 45.0f, -72.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[1] = SpotLight(0.15f, 0.8f, 0.3f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						110.0f, 45.0f, -42.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[2] = SpotLight(0.66f, 0.3f, 0.3f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						110.0f, 45.0f, -82.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					lightOn6 = true;
				}
				shaderList[0].SetSpotLights(spotLights3, spotLightCount3);
			}
			else {
				if (lightOn6) {
					// Apagar la luz
					spotLights3[0] = SpotLight(0.0f, 0.0f, 0.0f, // Color apagado
						0.0f, 0.0f,       // Intensidad apagada
						110.0f, 17.0f, -72.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[1] = SpotLight(0.0f, 0.0f, 0.0f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						110.0f, 45.0f, -42.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;

					spotLights3[2] = SpotLight(0.0f, 0.0f, 0.0f, // Color de la luz
						1.0f, 2.0f,       // Intensidad ambiental y difusa
						110.0f, 45.0f, -82.0f, // Posici�n de la luz
						0.0f, -1.0f, 0.0f, // Direcci�n de la luz
						1.0f, 0.0f, 0.0f, // Atenuaci�n
						30.0f);           // �ngulo del cono
					spotLightCount3++;


					lightOn6 = false;
				}
				shaderList[0].SetSpotLights(spotLights3, spotLightCount3);
			}



		}
		else if (mainWindow.getEncender() == true) {
			if (noche == false) {
				shaderList[0].SetSpotLights(spotLights2, spotLightCount2);
			}
			else {
				shaderList[0].SetSpotLights(spotLights, spotLightCount);
			}
		}

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelGengar(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
		glm::vec3 posicionPersonaje;
		glm::vec4 nuevaPosicionPersonaje;

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




		//============== ENTRADA ================
		//Entrada
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 22.0f, 150.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaPuerta.RenderModel();
		model = modelaux;

		//Cartel
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 6.5f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaCartel.RenderModel();
		model = modelaux;

		// ============== MURAL ================
		modelaux = model;
		model = glm::translate(model, glm::vec3(-18.0f, -25.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaMuro.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-80.5f, -25.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaMuro.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-141.0f, -25.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaMuro.RenderModel();
		model = modelaux;
		//Giro1
		modelaux = model;
		model = glm::translate(model, glm::vec3(-201.5f, -25.0f, -2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaMuro.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-201.5f, -25.0f, -62.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaMuro.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-201.5f, -25.0f, -62.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaMuro.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-201.5f, -25.0f, -123.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaMuro.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-201.5f, -25.0f, -183.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaMuro.RenderModel();
		model = modelaux;

		//Giro2
		modelaux = model;
		model = glm::translate(model, glm::vec3(-199.5f, -25.0f, -244.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaMuro.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-139.0f, -25.0f, -244.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaMuro.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-78.5f, -25.0f, -244.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaMuro.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-18.0f, -25.0f, -244.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaMuro.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(42.5f, -25.0f, -244.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaMuro.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(103.0f, -25.0f, -242.0f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaMuro.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(103.0f, -25.0f, -181.5f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaMuro.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(103.0f, -25.0f, -121.0f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaMuro.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(103.0f, -25.0f, -60.5f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaMuro.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(103.0f, -25.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaMuro.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(42.5f, -25.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		EntradaMuro.RenderModel();
		model = modelaux;

		//===== CAMINO ==================
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, -2.0f, 150.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -36.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(20.5f, 0.0f, -36.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(41.0f, 0.0f, -36.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(41.0f, 0.0f, -61.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(41.0f, 0.0f, -85.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(41.0f, 0.0f, -110.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(41.0f, 0.0f, -134.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(41.0f, 0.0f, -159.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(20.5f, 0.0f, -159.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -159.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-20.5f, 0.0f, -159.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-41.0f, 0.0f, -159.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-61.5f, 0.0f, -159.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-82.0f, 0.0f, -159.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-102.5f, 0.0f, -159.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-123.0f, 0.0f, -159.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-123.0f, 0.0f, -183.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-123.0f, 0.0f, -208.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-123.0f, 0.0f, -134.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-123.0f, 0.0f, -110.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-123.0f, 0.0f, -85.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-123.0f, 0.0f, -61.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-123.0f, 0.0f, -36.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-123.0f, 0.0f, -0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-20.5f, 0.0f, -36.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-41.0f, 0.0f, -36.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-61.5f, 0.0f, -36.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-82.0f, 0.0f, -36.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-102.5f, 0.0f, -36.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-143.5f, 0.0f, -36.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-164.0f, 0.0f, -36.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-184.5f, 0.0f, -36.5f));
		model = glm::scale(model, glm::vec3(2.5f, 1.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camino.RenderModel();
		model = modelaux;
		//Estacionamiento
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 200.5f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ParkinLot.RenderModel();
		//============ JAULA DE BATEO ==================
		//Lanzador
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.5f, -3.0f, -45.5f));
		modelaux = model;
		model = glm::rotate(model, -270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BateoLanzador.RenderModel();
		model = modelaux;
		//Pasto bat
		modelaux = model;
		model = glm::translate(model, glm::vec3(50.0f, 1.0f, 1.0f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BateoPiso.RenderModel();
		model = modelaux;
		//Jaula
		modelaux = model;
		model = glm::translate(model, glm::vec3(50.0f, 1.0f, -12.0f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		BateoJaula.RenderModel();
		model = modelaux;

		//Bat
		modelaux = model;
		model = glm::translate(model, glm::vec3(49.0f, 1.0f, -9.0f));
		model = glm::rotate(model, -25 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BateoBat.RenderModel();
		model = modelaux;

		//Pelota
		modelaux = model;
		model = glm::translate(model, glm::vec3(30.0f, 1.5f, -4.0f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BateoBola.RenderModel();
		model = modelaux;

		//Pelota
		modelaux = model;
		model = glm::translate(model, glm::vec3(15.0f, 1.5f, 4.0f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BateoBola.RenderModel();
		model = modelaux;

		//Pelota
		modelaux = model;
		model = glm::translate(model, glm::vec3(43.0f, 1.5f, 9.0f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BateoBola.RenderModel();
		model = modelaux;

		// ============= GLOBOS ==================
		//Mesa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(160.0f, 4.0f, 55.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GloboMesa.RenderModel();
		model = modelaux;

		//DARDOS

		modelaux = model;
		model = glm::translate(model, glm::vec3(-3.0f, 0.3f, -5.0f));
		model = glm::rotate(model, -15 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GloboDardo.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-3.0f, 0.3f, -1.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GloboDardo.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-3.0f, 0.3f, 3.0f));
		model = glm::rotate(model, -25 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GloboDardo.RenderModel();
		model = modelaux;

		//Tabla globos
		modelaux = model;
		model = glm::translate(model, glm::vec3(35.0f, 0.3f, 0.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.0f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GloboPared.RenderModel();
		model = modelaux;

		//Globos
		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 8.0f, -10.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo1.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 8.0f, -7.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo2.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 8.0f, -4.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo3.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 8.0f, -1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo4.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 8.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo5.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 8.0f, 5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo6.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 8.0f, 8.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo1.RenderModel();
		model = modelaux;

		//Segunda hilera
		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 5.0f, -10.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo6.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 5.0f, -7.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo5.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 5.0f, -4.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo4.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 5.0f, -1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo3.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 5.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo2.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 5.0f, 5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo1.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 5.0f, 8.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo6.RenderModel();
		model = modelaux;

		//Tercera hilera
		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 2.0f, -10.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo1.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 2.0f, -7.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo2.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 2.0f, -4.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo3.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 2.0f, -1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo4.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 2.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo5.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 2.0f, 5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo6.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(34.5f, 2.0f, 8.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo1.RenderModel();
		model = modelaux;

		//Jaula de precuacion globos
		modelaux = model;
		model = glm::translate(model, glm::vec3(10.0f, -8.0f, 22.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 1.5f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		BateoJaula.RenderModel();
		model = modelaux;

		// ===================== DADOS ==================
		//Mesa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(165.0f, 4.0f, 5.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dados_Mesa.RenderModel();

		//Dados
		modelaux = model;
		model = glm::translate(model, glm::vec3(-5.5f, 2.0f, 10.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[7]->RenderMesh();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-8.5f, 2.0f, 8.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[7]->RenderMesh();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-11.5f, 2.0f, 10.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[7]->RenderMesh();
		model = modelaux;

		//Jaula dados premios
		modelaux = model;
		model = glm::translate(model, glm::vec3(10.0f, -13.0f, 22.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.0f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		BateoJaula.RenderModel();
		model = modelaux;

		// ===================== BOLOS ==================
		//Piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, -2.0f, -45.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BateoPiso.RenderModel();

		//Porteria
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -35.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		BolichePorteria.RenderModel();
		model = modelaux;

		//Pines
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -22.0f));
		model = glm::rotate(model, 125 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pin1.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(1.5f, 0.0f, -26.0f));
		model = glm::rotate(model, 125 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pin2.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, -26.0f));
		model = glm::rotate(model, 125 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pin3.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(3.0f, 0.0f, -30.0f));
		model = glm::rotate(model, 125 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pin4.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, -30.0f));
		model = glm::rotate(model, 125 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pin5.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -30.0f));
		model = glm::rotate(model, 125 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pin6.RenderModel();
		model = modelaux;

		//Bola boliche
		modelaux = model;
		model = glm::translate(model, glm::vec3(7.0f, 1.0f, -2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bola.RenderModel();
		model = modelaux;

		// ================== HACHA =================

		//Diana
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(110.0f, -2.0f, -75.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Diana.RenderModel();
		model = modelaux;

		//Pared madera
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, -7.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GloboPared.RenderModel();
		model = modelaux;

		//Jaula
		modelaux = model;
		model = glm::translate(model, glm::vec3(9.0f, -1.0f, 27.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(3.5f, 2.8f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		BateoJaula.RenderModel();
		model = modelaux;

		//Hacha
		modelaux = model;
		model = glm::translate(model, glm::vec3(9.0f, 4.0f, 29.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.1f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Hacha.RenderModel();
		model = modelaux;

		//Mesa
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 29.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.2f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GloboMesa.RenderModel();
		model = modelaux;

		// =============== TOPOS ===================
		//Mesa topo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, -2.0f, -45.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TopoMaquina.RenderModel();
		model = modelaux;

		//Topo
		modelaux = model;
		model = glm::translate(model, glm::vec3(1.6f, 6.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Topo.RenderModel();
		model = modelaux;

		//Topo
		modelaux = model;
		model = glm::translate(model, glm::vec3(1.6f, 6.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Topo.RenderModel();
		model = modelaux;

		//Topo
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.5f, 6.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Topo.RenderModel();
		model = modelaux;

		//Topo
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.5f, 6.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Topo.RenderModel();
		model = modelaux;

		//Topo
		modelaux = model;
		model = glm::translate(model, glm::vec3(-0.6f, 6.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Topo.RenderModel();
		model = modelaux;

		//Topo
		modelaux = model;
		model = glm::translate(model, glm::vec3(-0.6f, 6.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Topo.RenderModel();
		model = modelaux;

		//Topo
		modelaux = model;
		model = glm::translate(model, glm::vec3(-1.7f, 6.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Topo.RenderModel();
		model = modelaux;

		//Topo
		modelaux = model;
		model = glm::translate(model, glm::vec3(-1.7f, 6.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Topo.RenderModel();
		model = modelaux;

		//Mazo
		modelaux = model;
		model = glm::translate(model, glm::vec3(1.8f, 7.0f, 3.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		//model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TopoMazo.RenderModel();
		model = modelaux;

		//Mazo
		modelaux = model;
		model = glm::translate(model, glm::vec3(18.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 1.0f));
		//model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BateoRed.RenderModel();
		model = modelaux;

		//================== Puestos ==================

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, -2.0f, -60.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(3.4f, 3.4f, 3.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carrusel.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, -2.0f, 20.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.9f, 2.7f, 1.9f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Trampolin.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, -2.0f, 20.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(3.4f, 3.4f, 3.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carrusel.RenderModel();
		model = modelaux;


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, -2.0f, 80.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Hamburguesa.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, -2.0f, 80.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Papas.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, -2.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Papas.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, -2.0f, 80.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Helado.RenderModel();
		model = modelaux;

		//Alimentos nesas 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-65.0f, -2.0f, 50.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MesaComida.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-65.0f, -2.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MesaComida.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(180.0f, -2.0f, -58.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MesaComida.RenderModel();
		model = modelaux;

		//Bancas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-65.0f, -2.0f, -10.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bancas.RenderModel();
		model = modelaux;

		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-65.0f, -2.0f, 10.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bancas.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-65.0f, -2.0f, 40.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bancas.RenderModel();
		model = modelaux;

		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-65.0f, -2.0f, 60.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bancas.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(190.0f, -2.0f, -58.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bancas.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(170.0f, -2.0f, -58.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bancas.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(120.0f, -2.0f, 74.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BancasTematicas.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(120.0f, -2.0f, 44.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bancas.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(120.0f, -2.0f, 10.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BancasTematicas.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.0f, -2.0f, 80.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BancasTematicas.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-82.0f, -2.0f, 80.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BancasTematicas.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.0f, -2.0f, 69.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bancas.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, -2.0f, -74.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BancasTematicas.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.0f, -2.0f, -50.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bancas.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.0f, -2.0f, -70.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bancas.RenderModel();
		model = modelaux;
		//Puestos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, -2.0f, 120.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Hamburguesa.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, -2.0f, 120.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Hamburguesa.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, -2.0f, 120.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Papas.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(130.0f, -2.0f, 120.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Papas.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(160.0f, -2.0f, 100.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Helado.RenderModel();
		model = modelaux;

		//Luces base
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(50.0f, -10.0f, 20.0f)); // Posici�n base
		modelaux = model;
		model = glm::scale(model, glm::vec3(14.0f, 7.0f, 14.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		baseLuces.RenderModel();
		model = modelaux;

		if (mainWindow.getarticulacion1() != 0.0f) {
			movBrazos = sin(glfwGetTime() * velocidad) * 45.0f * toRadians;
		}
		else {
			movBrazos = 0.0f;
		}
		//Personaje Gengar
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(100.0f, 4.0f, 150.0f)); // Posici�n base
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, mainWindow.getarticulacion1())); // Movimiento hacia adelante en local

		modelaux = model;
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gengar.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-3.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, movBrazos, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GengarManoDer.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(3.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -movBrazos, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GengarManoIzq.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(3.0f, -5.5f, 1.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GengarPataDer.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-3.0f, -5.5f, 1.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GengarPataIzq.RenderModel();
		//Muscle Man
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(105.0f, 0.0f, 140.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, mainWindow.getarticulacion1()));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(250.08f, 250.08f, 250.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MuscleMan.RenderModel();
		//Benzon
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(105.0f, 4.0f, 145.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, mainWindow.getarticulacion1()));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(25.08f, 25.08f, 25.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Benzon.RenderModel();
		model = modelaux;
		//Personaje Mordecai
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(105.0f, -1.0f, 150.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, mainWindow.getarticulacion1()));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, -270 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.08f, 8.08f, 8.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mordecai.RenderModel();

		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BrazoDerMordecai.RenderModel();
		model = modelaux;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BrazoIzqMordecai.RenderModel();
		model = modelaux;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PieDerMordecai.RenderModel();
		model = modelaux;

		//model = glm::translate(model, glm::vec3(-0.3f, -0.8f, 0.1));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(8.08f, 8.08f, 8.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PieIzqMordecai.RenderModel();
		model = modelaux;
		//Luces interactivas
		glm::vec3 posicionLuz2;
		glm::vec4 nuevaPosicionLuz2;
		//Luz 1
		model = glm::mat4(1.0f);
		modelaux = model;
		model = glm::translate(model, glm::vec3(163.0f, 90.0f, 21.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 1.0f, 0.0f));
		posicionLuz2 = glm::vec3(-61.0f, 90.0f, 21.0f);
		nuevaPosicionLuz2 = model * glm::vec4(posicionLuz2, 1.0f);
		spotLights[0].SetPos(nuevaPosicionLuz2);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BateoBola.RenderModel();
		model = modelaux;

		//Luz 2
		modelaux = model;
		model = glm::translate(model, glm::vec3(46.0f, 90.0f, 21.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 1.0f, 0.0f));
		posicionLuz2 = glm::vec3(46.0f, 90.0f, 21.0f);
		nuevaPosicionLuz2 = model * glm::vec4(posicionLuz2, 1.0f);
		spotLights[1].SetPos(nuevaPosicionLuz2);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BateoBola.RenderModel();
		model = modelaux;

		//Luz 3
		modelaux = model;
		model = glm::translate(model, glm::vec3(-62.0f, 90.0f, 21.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 1.0f, 0.0f));
		posicionLuz2 = glm::vec3(-62.0f, 90.0f, 21.0f);
		nuevaPosicionLuz2 = model * glm::vec4(posicionLuz2, 1.0f);
		spotLights[2].SetPos(nuevaPosicionLuz2);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BateoBola.RenderModel();
		model = modelaux;


		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche - 50.0f, -10.5f, -2.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		if (noche == false) {
			//Instancia del sol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(movCoche - 50.0f, 250.5f, -2.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
			//model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Sol.RenderModel();
		}
		else{
			//Instancia de la luna
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(movCoche - 50.0f, 250.5f, -2.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Luna.RenderModel();
		}

		// ================== FIN DE ESCENA ==================

		glDisable(GL_BLEND);


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
