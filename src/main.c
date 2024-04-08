#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <PACE.h>
#include <math.h>
#include <3dpc.h>
#include <time.h>

#define _USE_MATH_DEFINES

//TODO:Implement UI functionality and fix "warping" while moving
PACE *pace;
int showCursor = 0;
float speed = 4231941*0.01, mouseSpeed = 0.001, elapsedTime = 0;
clock_t start;

int keyPressed = -1;

void mouse_callback(double x, double y)
{
	if(showCursor)
		return;

	//delta x/y rotation
	pace->cameraTransform.ry += (x-400)*mouseSpeed;//elapsedTime;
	pace->cameraTransform.rx += (y-300)*mouseSpeed;//elapsedTime;
	//calculate forward vector with new rotation
	pace->forward[0] = cos(pace->cameraTransform.rx) * sin(pace->cameraTransform.ry);
	pace->forward[1] = sin(pace->cameraTransform.rx);
	pace->forward[2] = cos(pace->cameraTransform.rx) * cos(pace->cameraTransform.ry);

	//calculate right vector with new rotation(y always 0)
	pace->right[0] = sin(pace->cameraTransform.ry-M_PI/2.0);
	pace->right[2] = cos(pace->cameraTransform.ry-M_PI/2.0);

	//cross forward and right vector to get the up vector
	vec3_mul_cross(pace->up, pace->right, pace->forward);
	glfwSetCursorPos(pace->window, 400, 300);
}

void key_callback(int key, int scancode, int action, int mods)
{
	if(action == 0)
	{
		keyPressed = -1;
		return;
	}
	keyPressed = key;
//	printf("%d\n", key);
}

int main(int argc, char **argv)
{
	pace = CreatePACE(800, 600, 0.1, 100000);

	if(!pace)
	{
		printf("Failed to create space!\n");
		return -1;
	}

	start = clock();
	glClearColor(0, 0, 0, 1);
	PACESetKeyCallback(key_callback);
	PACESetMouseMovedCallback(mouse_callback);
//	glfwSetInputMode(pace->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	PACE_hide_cursor(pace);

	PAMesh *skybox = LoadMeshFromFile("skybox.3dpc", LoadShaderFromSource("shaders/skybox.vert", "shaders/skybox.frag"));
	skybox->shader->texture = LoadTextureFromFile("skybox.jpg", GL_RGB, GL_RGB);
	skybox->transform.sx = 255;
	skybox->transform.sy = 255;
	skybox->transform.sz = 255;

	PAMesh *rect = CreateMesh();
	rect->shader = LoadShaderFromSource("shaders/skybox.vert", "shaders/skybox.frag");
	rect->shader->texture = LoadTextureFromFile("Test.jpg", GL_RGB, GL_RGB);
	float rectData[] = {
		100, 300, 0, 0, 0,
		400, 300, 0, 1, 0,
		100, 0, 0, 0, 1,
		400, 300, 0, 1, 0,
		100, 0, 0, 0, 1,
		400, 0, 0, 1, 1
	};
	rect->vertices = malloc(30*sizeof(float));
	SetPAMeshVertices(rect, rectData, 30);

	PAMesh *spaceship = LoadMeshFromFile("Spaceship.3dpc", LoadShaderFromSource("shaders/default.vert", "shaders/default.frag"));
	PAMesh *mercury = LoadMeshFromFile("planet.3dpc", LoadShaderFromSource("shaders/default.vert", "shaders/default.frag"));

	PAMesh *venus = CreateMesh();
	venus->shader = LoadShaderFromSource("shaders/default.vert", "shaders/default.frag");
	SetPAMeshVertices(venus, mercury->vertices, mercury->numVertices);

	PAMesh *earth = CreateMesh();
	earth->shader = LoadShaderFromSource("shaders/default.vert", "shaders/default.frag");
	SetPAMeshVertices(earth, mercury->vertices, mercury->numVertices);

	PAMesh *mars = CreateMesh();
	mars->shader = LoadShaderFromSource("shaders/default.vert", "shaders/default.frag");
	SetPAMeshVertices(mars, mercury->vertices, mercury->numVertices);

	PAMesh *jupiter = CreateMesh();
	jupiter->shader = LoadShaderFromSource("shaders/default.vert", "shaders/default.frag");
	SetPAMeshVertices(jupiter, mercury->vertices, mercury->numVertices);

	PAMesh *saturn = CreateMesh();
	saturn->shader = LoadShaderFromSource("shaders/default.vert", "shaders/default.frag");
	SetPAMeshVertices(saturn, mercury->vertices, mercury->numVertices);

	PAMesh *uranus = CreateMesh();
	uranus->shader = LoadShaderFromSource("shaders/default.vert", "shaders/default.frag");
	SetPAMeshVertices(uranus, mercury->vertices, mercury->numVertices);

	PAMesh *neptune = CreateMesh();
	neptune->shader = LoadShaderFromSource("shaders/default.vert", "shaders/default.frag");
	SetPAMeshVertices(neptune, mercury->vertices, mercury->numVertices);

	if(!mercury || !venus || !earth || !mars || !jupiter || !saturn || !uranus || !neptune)
	{
		printf("Failed to load model!\n");
		return -3;
	}

	PATexture *shipTex = LoadTextureFromFile("ShipTex.jpg", GL_RGB, GL_RGB);
	PATexture *mercuryTex = LoadTextureFromFile("mercury.jpg", GL_RGB, GL_RGB);
	PATexture *venusTex = LoadTextureFromFile("venus.jpg", GL_RGB, GL_RGB);
	PATexture *earthTex = LoadTextureFromFile("earth.jpg", GL_RGB, GL_RGB);
	PATexture *marsTex = LoadTextureFromFile("mars.jpg", GL_RGB, GL_RGB);
	PATexture *jupiterTex = LoadTextureFromFile("jupiter.jpg", GL_RGB, GL_RGB);
	PATexture *saturnTex = LoadTextureFromFile("saturn.jpg", GL_RGB, GL_RGB);
	PATexture *uranusTex = LoadTextureFromFile("uranus.jpg", GL_RGB, GL_RGB);
	PATexture *neptuneTex = LoadTextureFromFile("neptune.jpg", GL_RGB, GL_RGB);
	if(!mercuryTex || !venusTex || !earthTex || !marsTex || !jupiterTex || !saturnTex || !uranusTex || !neptuneTex)
	{
		printf("Failed to load texture!\n");
		return -2;
	}
	spaceship->shader->texture = shipTex;
	mercury->shader->texture = mercuryTex;
	venus->shader->texture = venusTex;
	earth->shader->texture = earthTex;
	mars->shader->texture = marsTex;
	jupiter->shader->texture = jupiterTex;
	saturn->shader->texture = saturnTex;
	uranus->shader->texture = uranusTex;
	neptune->shader->texture = neptuneTex;


	PAScene *scene = CreateScene();
	//Add UI
//	AddUIToScene(scene, rect);

	AddMeshToScene(scene, skybox);
	AddMeshToScene(scene, mercury);
	AddMeshToScene(scene, venus);
	AddMeshToScene(scene, earth);
	AddMeshToScene(scene, mars);
	AddMeshToScene(scene, jupiter);
	AddMeshToScene(scene, saturn);
	AddMeshToScene(scene, uranus);
	AddMeshToScene(scene, neptune);

//	mercury->transform.px = -230;
	mercury->transform.pz = -230;
	mercury->transform.sx = 0.035;
	mercury->transform.sy = 0.035;
	mercury->transform.sz = 0.035;

//	venus->transform.px = -230;
	venus->transform.pz = -197255*0.01;
	venus->transform.sx = 0.085;
	venus->transform.sy = 0.085;
	venus->transform.sz = 0.085;

//	earth->transform.px = -230;
	earth->transform.pz = -359608*0.01;
	earth->transform.sx = 0.089;
	earth->transform.sy = 0.089;
	earth->transform.sz = 0.089;

// 	mars->transform.px = -230;
	mars->transform.pz = -666667*0.01;
	mars->transform.sx = 0.048;
	mars->transform.sy = 0.048;
	mars->transform.sz = 0.048;

//	jupiter->transform.px = -230;
	jupiter->transform.pz = -2825098*0.01;

//	saturn->transform.px = -230;
	saturn->transform.pz = -5369020*0.01;
	saturn->transform.sx = 0.84;
	saturn->transform.sy = 0.84;
	saturn->transform.sz = 0.84;

//	uranus->transform.px = -230;
	uranus->transform.pz = -11027843*0.01;
	uranus->transform.sx = 0.36;
	uranus->transform.sy = 0.36;
	uranus->transform.sz = 0.36;

//	neptune->transform.px = -230;
	neptune->transform.pz = -17408235*0.01;
	neptune->transform.sx = 0.35;
	neptune->transform.sy = 0.35;
	neptune->transform.sz = 0.35;

//	pace->cameraTransform.px = 230;
//	pace->cameraTransform.pz = -230;

	pace->loadedScene = scene;

	while(pace->running)
	{
		elapsedTime = ((double)clock()-start)/CLOCKS_PER_SEC;
		start = clock();
		PollPACE(pace);
		float time = glfwGetTime()/2;

		//Update shit
		switch(keyPressed)
		{
			case 258:
				if(showCursor)
				{
					PACE_hide_cursor(pace);
					showCursor = 0;
					break;
				}
				PACE_show_cursor(pace);
				showCursor = 1;
				break;
			case 'W':
				pace->cameraTransform.px -= pace->forward[0] * speed * elapsedTime;
				pace->cameraTransform.py -= pace->forward[1] * speed * elapsedTime;
				pace->cameraTransform.pz -= pace->forward[2] * speed * elapsedTime;
				break;
			case 'A':
				pace->cameraTransform.px -= pace->right[0] * speed * elapsedTime;
				pace->cameraTransform.py -= pace->right[1] * speed * elapsedTime;
				pace->cameraTransform.pz -= pace->right[2] * speed * elapsedTime;
				break;
			case 'S':
				pace->cameraTransform.px += pace->forward[0] * speed * elapsedTime;
				pace->cameraTransform.py += pace->forward[1] * speed * elapsedTime;
				pace->cameraTransform.pz += pace->forward[2] * speed * elapsedTime;
				break;
			case 'D':
				pace->cameraTransform.px += pace->right[0] * speed * elapsedTime;
				pace->cameraTransform.py += pace->right[1] * speed * elapsedTime;
				pace->cameraTransform.pz += pace->right[2] * speed * elapsedTime;
				break;

		}

		mercury->transform.ry = time;
		venus->transform.ry = time;
		earth->transform.ry = time;
		mars->transform.ry = time;
		jupiter->transform.ry = time;
		saturn->transform.ry = time;
		uranus->transform.ry = time;
		neptune->transform.ry = time;

		//Render shit
		UpdateWindowContent(pace);
	}

	ClearPACE(pace);

	return 0;
}
