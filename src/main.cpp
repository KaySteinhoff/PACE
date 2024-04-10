#include <cppPACE.h>
#include <math.h>
#include <3dpc.h>
#include <time.h>

#define USE_MATH_DEFINES

float speed = 4231941*0.01, elapsedTime = 0, mouseSpeed = 5;
int keyPressed = -1;
clock_t start;

void mouse_callback(double x, double y)
{
	PACE *inst = PACE::GetInstance();

	inst->cameraTransform.ry += (x-400)*elapsedTime*mouseSpeed;
	inst->cameraTransform.rx += (y-300)*elapsedTime*mouseSpeed;

	inst->forward[0] = cos(inst->cameraTransform.rx) * sin(inst->cameraTransform.ry);
	inst->forward[1] = sin(inst->cameraTransform.rx);
	inst->forward[2] = cos(inst->cameraTransform.rx) * cos(inst->cameraTransform.ry);

	inst->right[0] = sin(inst->cameraTransform.ry-M_PI/2.0);
	inst->right[2] = cos(inst->cameraTransform.ry-M_PI/2.0);

	vec3_mul_cross(inst->up, inst->right, inst->forward);
	glfwSetCursorPos(inst->window, 400, 300);
}

void key_callback(int key, int scancode, int action, int mods)
{
	if(action == 0)
	{
		keyPressed = -1;
		return;
	}
	keyPressed = key;
}

//TODO:Copy code from main.c into main.cpp
int main()
{
	PACE::INIT(800, 600, 0.1, 100000);

	PACE *pace = PACE::GetInstance();

	if(!pace)
	{
		printf("Failed to initialize PACE!\n");
		return -1;
	}

	pace->SetKeyCallback(key_callback);
	pace->SetMouseMovedCallback(mouse_callback);
	Cursor::Hide();

	PAMesh *skybox = LoadMeshFromFile("skybox.3dpc", PAShader::LoadShaderFromSource((char*)"shaders/skybox.vert", (char*)"shaders/skybox.frag"));
	skybox->shader->texture = PATexture::LoadTextureFromFile("skybox.jpg", GL_RGB, GL_RGB);
	skybox->transform.sx = 255;
	skybox->transform.sy = 255;
	skybox->transform.sz = 255;

	PAMesh *mercury = LoadMeshFromFile("planet.3dpc", PAShader::LoadShaderFromSource((char*)"shaders/default.vert", (char*)"shaders/default.frag"));
	mercury->shader->texture = PATexture::LoadTextureFromFile("mercury.jpg", GL_RGB, GL_RGB);

	PAMesh *venus = new PAMesh();
	venus->shader = PAShader::LoadShaderFromSource((char*)"shaders/default.vert", (char*)"shaders/default.frag");
	venus->shader->texture = PATexture::LoadTextureFromFile("venus.jpg", GL_RGB, GL_RGB);
	venus->SetVertices(mercury->data, mercury->numVertices);
	venus->transform.pz = -197255*0.01;

	PAMesh *earth = new PAMesh();
	earth->shader = PAShader::LoadShaderFromSource((char*)"shaders/default.vert", (char*)"shaders/default.frag");
	earth->shader->texture = PATexture::LoadTextureFromFile("earth.jpg", GL_RGB, GL_RGB);
	earth->SetVertices(mercury->data, mercury->numVertices);
	earth->transform.pz = -359608*0.01;

	PAMesh *mars = new PAMesh();
	mars->shader = PAShader::LoadShaderFromSource((char*)"shaders/default.vert", (char*)"shaders/default.frag");
	mars->shader->texture = PATexture::LoadTextureFromFile("mars.jpg", GL_RGB, GL_RGB);
	mars->SetVertices(mercury->data, mercury->numVertices);
	mars->transform.pz = -666667*0.01;

	PAMesh *jupiter = new PAMesh();
	jupiter->shader = PAShader::LoadShaderFromSource((char*)"shaders/default.vert", (char*)"shaders/default.frag");
	jupiter->shader->texture = PATexture::LoadTextureFromFile("jupiter.jpg", GL_RGB, GL_RGB);
	jupiter->SetVertices(mercury->data, mercury->numVertices);
	jupiter->transform.pz = -2825098*0.01;

	PAMesh *saturn = new PAMesh();
	saturn->shader = PAShader::LoadShaderFromSource((char*)"shaders/default.vert", (char*)"shaders/default.frag");
	saturn->shader->texture = PATexture::LoadTextureFromFile("saturn.jpg", GL_RGB, GL_RGB);
	saturn->SetVertices(mercury->data, mercury->numVertices);
	saturn->transform.pz = -5369020*0.01;

	PAMesh *uranus = new PAMesh();
	uranus->shader = PAShader::LoadShaderFromSource((char*)"shaders/default.vert", (char*)"shaders/default.frag");
	uranus->shader->texture = PATexture::LoadTextureFromFile("uranus.jpg", GL_RGB, GL_RGB);
	uranus->SetVertices(mercury->data, mercury->numVertices);
	uranus->transform.pz = -11027843*0.01;

	PAMesh *neptune = new PAMesh();
	neptune->shader = PAShader::LoadShaderFromSource((char*)"shaders/default.vert", (char*)"shaders/default.frag");
	neptune->shader->texture = PATexture::LoadTextureFromFile("neptune.jpg", GL_RGB, GL_RGB);
	neptune->SetVertices(mercury->data, mercury->numVertices);
	neptune->transform.pz = -17408235*0.01;

	PAScene scene = PAScene();
	scene.AddMesh(skybox);
	scene.AddMesh(mercury);
	scene.AddMesh(venus);
	scene.AddMesh(earth);
	scene.AddMesh(mars);
	scene.AddMesh(jupiter);
	scene.AddMesh(saturn);
	scene.AddMesh(uranus);
	scene.AddMesh(neptune);

	printf("added all planets!\n");

	pace->loadedScene = &scene;

	while(pace->running)
	{
		elapsedTime = ((double)clock()-start)/CLOCKS_PER_SEC;
		start = clock();
		pace->Poll();
		float time = glfwGetTime()/2.0;

		switch(keyPressed)
		{
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
/*		earth->transform.ry = time;
		mars->transform.ry = time;
		jupiter->transform.ry = time;
		saturn->transform.ry = time;
		uranus->transform.ry = time;
		neptune->transform.ry = time;
*/
		printf("updating...\n");
		pace->UpdateContents();
	}

	return 0;
}
