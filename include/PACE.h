#ifndef PACE_ENGINE
#define PACE_ENGINE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <linmath.h>

extern void (*PACE_key_callback)(int, int, int, int);
extern void (*PACE_mouse_moved_callback)(double, double);

typedef struct PACE PACE;
typedef struct PAMesh PAMesh;
typedef struct PAShader PAShader;
typedef struct PATexture PATexture;
typedef struct PAScene PAScene;

typedef struct PAPickingTexture
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

typedef struct PATransform
{
	float px;
	float py;
	float pz;

	float rx;
	float ry;
	float rz;

	float sx;
	float sy;
	float sz;

	mat4x4 transformMatrix;
}PATransform;

#define DEFAULT_TRANSFORM(T) PATransform T = {.px = 0, .py = 0, .pz = 0, .rx = 0, .ry = 0, .rz = 0, .sx = 1, .sy = 1, .sz = 1}

LINMATH_H_FUNC void mat4x4_apply_transform(mat4x4 M, PATransform transform)
{
	mat4x4 rotation, scale;

	mat4x4_identity(M);//Used as translation matrix
	mat4x4_identity(rotation);
	mat4x4_identity(scale);

	mat4x4_rotate_X(rotation, rotation, transform.rx);
	mat4x4_rotate_Y(rotation, rotation, transform.ry);
	mat4x4_rotate_Z(rotation, rotation, transform.rz);
	mat4x4_translate_in_place(M, transform.px, transform.py, transform.pz);
	mat4x4_scale_aniso(scale, scale, transform.sx, transform.sy, transform.sz);

	mat4x4_mul(M, M, rotation);
	mat4x4_mul(M, M, scale);
}

struct PACE
{
	GLFWwindow *window;
	int running;

	PAScene *loadedScene;
	PAScene *scenes;
	uint16_t numScenes;
	uint16_t allocScenes;

	PAShader *defaultShader;
	mat4x4 identMatrix;
	mat4x4 viewMatrix;
	mat4x4 projectionMatrix;
	mat4x4 orthoMatrix;

	PATransform cameraTransform;
	PAPickingTexture *papickingTexture;

	float nearPlane;
	float farPlane;

	vec3 forward;
	vec3 right;
	vec3 up;
};

PACE* CreatePACE(uint32_t width, uint32_t height, float nearPlane, float farPlane);
void PACESetKeyCallback(void (*func)(int, int, int, int));
void PACESetMouseMovedCallback(void (*func)(double, double));
void PACE_hide_cursor(PACE *pace);
void PACE_show_cursor(PACE *pace);
void PollPACE();
void UpdateWindowContent(PACE *pace);
void ClearPACE(PACE *pace);

struct PAScene
{
	PAMesh **ui;
	PAMesh **meshes;
	int numUIs;
	int numMeshes;
	int allocMeshes;
	int allocUIs;
};

PAScene* CreateScene();
int AddMeshToScene(PAScene *scene, PAMesh *mesh);
int AddUIToScene(PAScene *scene, PAMesh *ui);
int RemoveMeshFromScene(PAScene *scene, int index, PAMesh *mesh);
void PurgePAScene(PAScene *scene, int purgeMeshes);

struct PAMesh
{
	float *vertices;
	uint32_t numVertices;
	uint32_t numFaces;

	float *normals;
	uint32_t numNormals;

	float *uvs;
	uint32_t numUVs;

	PATransform transform;
	GLuint vao;
	PAShader *shader;
};

PAMesh* CreateMesh();
int SetPAMeshVertices(PAMesh *mesh, float *vertices, uint32_t numVertices);
void DrawMesh(PAMesh *mesh);
void PurgePAMesh(PAMesh *mesh);

struct PATexture
{
	GLuint textureID;

	int width;
	int height;
	int nrChannels;

	unsigned char *data;
};

PATexture* CreateTexture(int width, int height, int nrChannels, unsigned char *data);
PATexture* LoadTextureFromFile(const char *path, GLint byteFormat, GLenum format);

struct PAShader
{
	GLuint vbo;

	GLint fragmentShader;
	GLint vertexShader;

	GLint ID;

	PATexture *texture;

	GLuint modelLocation;
	GLuint viewLocation;
	GLuint perspectiveLocation;
};

void EnableShader(PAShader *shader);
PAShader* LoadShaderFromSource(char *vertexSource, char *fragmentSource);
int SetInt(PAShader *shader, const char *name, int value);
int SetFloat(PAShader *shader, const char *name, float value);
PAShader* CompileShader(const char *vertexShader, const char *fragmentShader);

#endif
