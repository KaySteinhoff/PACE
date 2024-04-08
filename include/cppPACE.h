#ifndef PACE_ENGINE
#define PACE_ENGINE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <linmath.h>

/*typedef struct PAPickingTexture
{
	GLuint fbo;
	GLuint pickingTexture;
	GLuint depthTexture;

	struct PixelInfo
	{
		uint32_t objectID;
		uint32_t drawID;
		uint32_t primID;
	}pixelInfo;

	PAShader *pickingShader;
}PAPickingTexture;

PAPickingTexture* CreatePickingTexture(uint32_t width, uint32_t height);
void EnablePickingTexture(PAPickingTexture *papickingTexture);
void DisablePickingTexture(PAPickingTexture *papickingTexture);
void ReadPixelInfo(PAPickingTexture *papickingTexture, uint32_t x, uint32_t y);
void PickObjects(PACE *pace, PAPickingTexture *papickingTexture);
*/

//DONE
class PATransform
{
public:
	float px = 0;
	float py = 0;
	float pz = 0;

	float rx = 0;
	float ry = 0;
	float rz = 0;

	float sx = 1;
	float sy = 1;
	float sz = 1;

	mat4x4 transformMatrix;

	PATransform();
	PATransform(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz);

	void ApplyTransform(mat4x4 M);
};

//DONE
class PATexture
{
public:
	GLuint textureID;

	int width;
	int height;
	int nrChannels;

	unsigned char *data;

	PATexture();
	static PATexture* LoadTextureFromFile(const char *path, GLint byteFormat, GLenum format);
};

//DONE
class PAShader
{
private:
	GLuint vbo;

	GLint fragmentShader;
	GLint vertexShader;

	GLint ID;

public:
	PATexture *texture;

	GLuint modelLocation;
	GLuint viewLocation;
	GLuint perspectiveLocation;

	static PAShader* LoadShaderFromSource(char *vertexPath, char *fragmentPath);
	static PAShader* CompileShader(const char *vertexShader, const char *fragmentShader);
	void Enable();
	int SetInt(const char *name, int value);
	int SetFloat(const char *name, float value);
};

//DONE
class PAMesh
{
private:
	GLuint vao;
	float *vertices;
public:
	uint32_t numVertices;
	uint32_t numFaces;

	PATransform transform;
	PAShader *shader;

	PAMesh();
	int SetVertices(float *vertices, uint32_t numVertices);
	void Draw();
	~PAMesh();
};

//DONE
class PAScene
{
private:
	int allocMeshes;
	int allocUIs;

public:
	PAMesh **ui;
	PAMesh **meshes;
	int numUIs;
	int numMeshes;

	PAScene();
	int AddUI(PAMesh *ui);
	int AddMesh(PAMesh *mesh);
	int RemoveMesh(int index, PAMesh *mesh);
	int RemoveUI(int index, PAMesh *ui);
	~PAScene();
};

//DONE
class Cursor
{
public:
	static double x;
	static double y;

	static void Hide();
	static void Show();
};

//DONE
class PACE
{
private:
	inline static PACE *instance = NULL;

public:
	GLFWwindow *window;
	int running;
	PAScene *loadedScene;

	PATransform cameraTransform;
//	PAPickingTexture *papickingTexture;

	mat4x4 identMatrix;
	mat4x4 viewMatrix;
	mat4x4 projectionMatrix;
	mat4x4 orthoMatrix;

	float nearPlane;
	float farPlane;

	vec3 forward;
	vec3 right;
	vec3 up;

	void (*PACE_key_callback)(int, int, int, int);
	void (*PACE_mouse_moved_callback)(double, double);

	static void INIT(uint32_t width, uint32_t height, float nearPlane, float farPlane);
	static PACE* GetInstance();
	void SetKeyCallback(void (*func)(int, int, int, int));
	void SetMouseMovedCallback(void (*func)(double, double));
	void Poll();
	void UpdateContents();
	~PACE();

};

#endif
