#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <PACE.h>
#include <PACEExtentions.h>
#include <math.h>
#include <3dpc.h>
#include <time.h>

#define _USE_MATH_DEFINES

//TODO:Implement UI functionality
PACamera *camera;
PACE *pace;
int showCursor = 0;
float speed = 4231941*0.01, mouseSpeed = 5, elapsedTime = 0;
clock_t start;

int keyPressed = -1;
int pressed = 0;

void mouse_callback(double x, double y)
{
	if(showCursor)
		return;

	camera->transform.ry += (x-400)*mouseSpeed * elapsedTime;
	camera->transform.rx += (y-300)*mouseSpeed * elapsedTime;

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
	pressed = 1;
//	printf("%d\n", key);
}

int main(int argc, char **argv)
{
//	PAExtention *testExt = LoadExtention("./ext.so");

/*	if(!testExt)
	{
		printf("Failed to load extention!\n");
		return 0;
	}
*/
	camera = CreateCamera(800, 600, 0.1, 100000);

	pace = InitPACE(800, 600, camera);

	if(!pace)
	{
		printf("Failed to create space!\n");
		return -1;
	}

	start = clock();
	glClearColor(0, 0, 0, 1);
	PACESetKeyCallback(key_callback);
	PACESetMouseMovedCallback(mouse_callback);
	PACE_hide_cursor(pace);

	PAMesh *skybox = LoadMeshFromFile("skybox.3dpc", LoadShaderFromSource("shaders/skybox.vert", "shaders/skybox.frag"));
	skybox->shader->texture = LoadTextureFromFile("skybox.jpg", GL_RGB, GL_RGB);
	skybox->transform.sx = 255;
	skybox->transform.sy = 255;
	skybox->transform.sz = 255;

	PAMesh *rect = CreateMesh();
	rect->shader = LoadShaderFromSource("shaders/default.vert", "shaders/default.frag");
	rect->shader->texture = LoadTextureFromFile("Test.jpg", GL_RGB, GL_RGB);
	float rectData[] = {
		350, 350, 0, 0, 0,
		450, 350, 0, 1, 0,
		350, 250, 0, 0, 1,
		450, 350, 0, 1, 0,
		350, 250, 0, 0, 1,
		450, 250, 0, 1, 1
	};
	rect->vertices = malloc(30*sizeof(float));
	SetPAMeshVertices(rect, rectData, 30);

	PAMesh *mercury = LoadMeshFromFile("planet.3dpc", LoadShaderFromSource("shaders/default.vert", "shaders/default.frag"));
	PAMesh *venus = LoadMeshFromFile("planet.3dpc", LoadShaderFromSource("shaders/default.vert", "shaders/default.frag"));
	PAMesh *earth = LoadMeshFromFile("planet.3dpc", LoadShaderFromSource("shaders/default.vert", "shaders/default.frag"));
	PAMesh *mars = LoadMeshFromFile("planet.3dpc", LoadShaderFromSource("shaders/default.vert", "shaders/default.frag"));
	PAMesh *jupiter = LoadMeshFromFile("planet.3dpc", LoadShaderFromSource("shaders/default.vert", "shaders/default.frag"));
	PAMesh *saturn = LoadMeshFromFile("planet.3dpc", LoadShaderFromSource("shaders/default.vert", "shaders/default.frag"));
	PAMesh *uranus = LoadMeshFromFile("planet.3dpc", LoadShaderFromSource("shaders/default.vert", "shaders/default.frag"));
	PAMesh *neptune = LoadMeshFromFile("planet.3dpc", LoadShaderFromSource("shaders/default.vert", "shaders/default.frag"));

	if(!mercury || !venus || !earth || !mars || !jupiter || !saturn || !uranus || !neptune)
	{
		printf("Failed to load model!\n");
		return -3;
	}

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

	mercury->transform.pz = -230;
	mercury->transform.sx = 0.035;
	mercury->transform.sy = 0.035;
	mercury->transform.sz = 0.035;

	venus->transform.pz = -197255*0.01;
	venus->transform.sx = 0.085;
	venus->transform.sy = 0.085;
	venus->transform.sz = 0.085;

	earth->transform.pz = -359608*0.01;
	earth->transform.sx = 0.089;
	earth->transform.sy = 0.089;
	earth->transform.sz = 0.089;

	mars->transform.pz = -666667*0.01;
	mars->transform.sx = 0.048;
	mars->transform.sy = 0.048;
	mars->transform.sz = 0.048;

	jupiter->transform.pz = -2825098*0.01;

	saturn->transform.pz = -5369020*0.01;
	saturn->transform.sx = 0.84;
	saturn->transform.sy = 0.84;
	saturn->transform.sz = 0.84;

	uranus->transform.pz = -11027843*0.01;
	uranus->transform.sx = 0.36;
	uranus->transform.sy = 0.36;
	uranus->transform.sz = 0.36;

	neptune->transform.pz = -17408235*0.01;
	neptune->transform.sx = 0.35;
	neptune->transform.sy = 0.35;
	neptune->transform.sz = 0.35;

	pace->loadedScene = scene;

//	testExt->setup();

	while(pace->running)
	{
		elapsedTime = ((double)clock()-start)/CLOCKS_PER_SEC;
		start = clock();
		PollPACE();
		float time = glfwGetTime()/2;

		//Update shit
		switch(keyPressed)
		{
			case 258:
				if(!pressed)
					break;
				pressed = 0;
				if(showCursor)
				{
					PACE_hide_cursor();
					showCursor = 0;
					break;
				}
				PACE_show_cursor();
				showCursor = 1;
				break;
			case 'W':
				camera->transform.px -= camera->transform.forward[0] * speed * elapsedTime;
				camera->transform.py -= camera->transform.forward[1] * speed * elapsedTime;
				camera->transform.pz -= camera->transform.forward[2] * speed * elapsedTime;
				break;
			case 'A':
				camera->transform.px -= camera->transform.right[0] * speed * elapsedTime;
				camera->transform.py -= camera->transform.right[1] * speed * elapsedTime;
				camera->transform.pz -= camera->transform.right[2] * speed * elapsedTime;
				break;
			case 'S':
				camera->transform.px += camera->transform.forward[0] * speed * elapsedTime;
				camera->transform.py += camera->transform.forward[1] * speed * elapsedTime;
				camera->transform.pz += camera->transform.forward[2] * speed * elapsedTime;
				break;
			case 'D':
				camera->transform.px += camera->transform.right[0] * speed * elapsedTime;
				camera->transform.py += camera->transform.right[1] * speed * elapsedTime;
				camera->transform.pz += camera->transform.right[2] * speed * elapsedTime;
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
		UpdateWindowContent();
//		testExt->update();
	}

	ClearPACE();

	return 0;
}
