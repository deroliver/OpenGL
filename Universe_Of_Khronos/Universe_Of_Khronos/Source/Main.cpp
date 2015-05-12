#define GLEW_STATIC
#include <GL/glew.h>
#include <random>

std::default_random_engine GEN;
std::uniform_real_distribution<float> Random(-5.0, 5.0);

#include "../Headers/Main.h"
#include "../Headers/GraphicsObject.h"
#include "../Headers/LightSource.h"
#include "../Headers/Island.h"
#include "../Headers/Model.h"

LightSource Light;
Shader ModelShader, LightShader;
Model Island, GR1, Grass, Island2;


Vertex3 Vertices[36];

int Index = 0;
int NormIndex = 0;

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f, 10.0f, 0.0f),
	glm::vec3(0.25f, -1.0f, 0.7f)
};

glm::vec3 glowingRockPositions[] = {
	glm::vec3(1.0f, 3.0f, 0.0f),
};


void CreateFace(int A, int B, int C, int D) {

	glm::vec3 Normals[] = {
		glm::vec3( 0.0f,  0.0f,  1.0f), // Back
		glm::vec3( 0.0f,  0.0f, -1.0f), // Front
		glm::vec3(-1.0f,  0.0f,  0.0f), // Left
		glm::vec3( 1.0f,  0.0f,  0.0f), // Right
		glm::vec3( 0.0f, -1.0f,  0.0f), // Bottom
		glm::vec3( 0.0f,  1.0f,  0.0f), // Top
	};

	glm::vec3 Quad[8] {
		// Front Face
		glm::vec3(-2.0, -2.0, 2.0), // #0
		glm::vec3( 2.0, -2.0, 2.0), // #1
		glm::vec3( 2.0,  2.0, 2.0), // #2
		glm::vec3(-2.0,  2.0, 2.0), // #3

			// Rear Face
		glm::vec3(-2.0, -2.0, -2.0), // #4
		glm::vec3( 2.0, -2.0, -2.0), // #5
		glm::vec3( 2.0,  2.0, -2.0), // #6
		glm::vec3(-2.0,  2.0, -2.0), // #7
	};

	Vertices[Index].xyz = Quad[A]; Vertices[Index].norm = Normals[NormIndex]; Index++;
	Vertices[Index].xyz = Quad[B]; Vertices[Index].norm = Normals[NormIndex]; Index++;
	Vertices[Index].xyz = Quad[C]; Vertices[Index].norm = Normals[NormIndex]; Index++;

	Vertices[Index].xyz = Quad[C]; Vertices[Index].norm = Normals[NormIndex]; Index++;
	Vertices[Index].xyz = Quad[D]; Vertices[Index].norm = Normals[NormIndex]; Index++;
	Vertices[Index].xyz = Quad[A]; Vertices[Index].norm = Normals[NormIndex]; Index++;

	NormIndex++;
}


int Main::KMain() {

	Initialize();

	GameLoop();

	Destroy();

	return 0;
}


void Main::Initialize() {
	// Makes sure the WindowManager is initializes
	if (!WindowManager || WindowManager->Initialize(WIDTH, HEIGHT, "MidTerm Project", false) != 0) {
		exit(-1);
	}

	ModelShader.Initialize("Shaders/Model_Loader_Vertex.Vert", "Shaders/Model_Loader_Fragment.Frag");
	LightShader.Initialize("Shaders/Model_Light_Vertex.Vert", "Shaders/Model_Light_Fragment.Frag");


	Island.SetPath("Assets/LowPolyIsland/LowPoly.obj");
	GR1.SetPath("Assets/LowPolyIsland/GlowingRock/GR1.obj");
	Grass.SetPath("Assets/Grass/Grass.obj");
	Island2.SetPath("Assets/Island2/Island2.obj");


	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);

	Camera->SetPersepective(glm::radians(70.0f), (float)WIDTH / (float)HEIGHT, 0.01f, 1000.0f);
	Camera->PositionCamera(0, 0, 0, 0, 0);
}


void Main::GameLoop() {
	while (WindowManager->ProcessInput(true)) {

		TimeManager::Instance().CalculateFrameRate(true);

		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		ModelShader.Use();   // <-- Don't forget this one!
		// Transformation matrices
		glm::mat4 projection = glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = Camera->GetViewMat();
		glUniformMatrix4fv(glGetUniformLocation(ModelShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(ModelShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Set the lighting uniforms
		glUniform3f(glGetUniformLocation(ModelShader.Program, "viewPos"), Camera->GetPosition().x, Camera->GetPosition().y, Camera->GetPosition().z);
		// Point light 1
		glUniform3f(glGetUniformLocation(ModelShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(ModelShader.Program, "pointLights[0].ambient"), 0.71f * 0.5f, 0.0f, 0.71f * 0.5f);
		glUniform3f(glGetUniformLocation(ModelShader.Program, "pointLights[0].diffuse"), 0.71f, 0.0f, 0.71f);
		glUniform3f(glGetUniformLocation(ModelShader.Program, "pointLights[0].specular"), 0.71f, 0.0f, 0.71f);
		glUniform1f(glGetUniformLocation(ModelShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(ModelShader.Program, "pointLights[0].linear"), 0.009);
		glUniform1f(glGetUniformLocation(ModelShader.Program, "pointLights[0].quadratic"), 0.0032);

		// Point light 2
		glUniform3f(glGetUniformLocation(ModelShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(ModelShader.Program, "pointLights[1].ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(ModelShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(ModelShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(ModelShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(ModelShader.Program, "pointLights[1].linear"), 0.009);
		glUniform1f(glGetUniformLocation(ModelShader.Program, "pointLights[1].quadratic"), 0.0032);

		// Draw the loaded model
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); 
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	
		glUniformMatrix4fv(glGetUniformLocation(ModelShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Island.Draw(ModelShader);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-5.0f, -2.75f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	
		glUniformMatrix4fv(glGetUniformLocation(ModelShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Island2.Draw(ModelShader);

		//model = glm::translate(model, glm::vec3(0.0f, -25.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(5.0f, 0.0f, 5.0f));	// It's a bit too big for our scene, so scale it down
		//glUniformMatrix4fv(glGetUniformLocation(ModelShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		//Grass.Draw(ModelShader);


		LightShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(LightShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(LightShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		for (int i = 0; i < 2; i++) {
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(pointLightPositions[i]));
			glUniformMatrix4fv(glGetUniformLocation(LightShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			GR1.Draw(LightShader);
		}

		

		TimeManager::Instance().Sleep(10);

		WindowManager->SwapTheBuffers();
	}
}


void Main::Destroy() {
	if (WindowManager) {
		WindowManager->Destroy();

		delete WindowManager;
		WindowManager = nullptr;
	}

	// If there is still a camera - Destroy it
	if (Camera) {
		delete Camera;
		Camera = nullptr;
	}
}





