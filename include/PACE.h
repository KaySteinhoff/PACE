#ifndef PACE_ENGINE_H_
#define PACE_ENGINE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <linmath.h>
#include <PACEInterfaces.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#define _USE_MATH_DEFINES

enum PAViewMode
{
	PAProjection,
	PAOrtho
};

extern void (*PACE_key_callback)(int, int, int, int);
extern void (*PACE_mouse_moved_callback)(double, double);
extern void (*PACE_window_resize_callback)(int, int);

typedef struct Mouse
{
	int32_t x;
	int32_t y;

	int button;
	int action;
	int mods;
}Mouse;

extern Mouse mouse;

typedef struct PACamera PACamera;
typedef struct PACE PACE;
typedef struct PAMesh PAMesh;
typedef struct PAShader PAShader;
typedef struct PATexture PATexture;
typedef struct PAScene PAScene;

typedef struct PAAreaLight PAAreaLight;

typedef struct PATransform
{
	struct PATransform *parent;

	float px;
	float py;
	float pz;

	float rx;
	float ry;
	float rz;

	float sx;
	float sy;
	float sz;

	vec3 right;
	vec3 up;
	vec3 forward;

	mat4x4 transformMatrix;
}PATransform;

#define DEFAULT_TRANSFORM(T) PATransform T = {.parent = NULL, .px = 0, .py = 0, .pz = 0, .rx = 0, .ry = 0, .rz = 0, .sx = 1, .sy = 1, .sz = 1, .right[0] = 1, .right[1] = 0, .right[2] = 0, .up[0] = 0, .up[1] = 1, .up[2] = 0, .forward[0] = 0, .forward[1] = 0, .forward[2] = 1}

LINMATH_H_FUNC void mat4x4_apply_transform(mat4x4 M, PATransform *transform)
{
	mat4x4 rotation, scale;

	mat4x4_identity(M);//Used as translation matrix
	mat4x4_identity(rotation);
	mat4x4_identity(scale);

	mat4x4_rotate_X(rotation, rotation, transform->rx);
	mat4x4_rotate_Y(rotation, rotation, transform->ry);
	mat4x4_rotate_Z(rotation, rotation, transform->rz);
	mat4x4_translate_in_place(M, transform->px, transform->py, transform->pz);
	mat4x4_scale_aniso(scale, scale, transform->sx, transform->sy, transform->sz);

	mat4x4_mul(M, M, rotation);
	mat4x4_mul(M, M, scale);

	transform->forward[0] = cos(transform->rx) * sin(transform->ry);
	transform->forward[1] = sin(transform->rx);
	transform->forward[2] = cos(transform->rx) * cos(transform->ry);

	transform->right[0] = sin(transform->ry-M_PI/2.0);
	transform->right[2] = cos(transform->ry-M_PI/2.0);

	vec3_mul_cross(transform->up, transform->right, transform->forward);
}

typedef struct PAPickingTexture
{
	GLuint fbo;
	GLuint pbo;
	GLuint pickingTexture;
	GLuint depthTexture;

	int32_t width;
	int32_t height;

	PAShader *pickingShader;
}PAPickingTexture;

typedef struct
{
	uint32_t objectID;
	uint32_t drawID;
	uint32_t primID;
}PixelInfo;

PAPickingTexture* CreatePickingTexture(uint32_t width, uint32_t height);
PixelInfo ReadPixelInfo(PAPickingTexture *papickingTexture, uint32_t x, uint32_t y);

struct PACamera
{
	PATransform transform;

	enum PAViewMode viewMode;

	mat4x4 identMatrix;
	mat4x4 projectionMatrix;
	mat4x4 uiMatrix;
	mat4x4 orthoMatrix;

	float nearPlane;
	float farPlane;

	PAPickingTexture *pickingTexture;
};

PACamera* CreateCamera(uint32_t width, uint32_t height, float nearPlane, float farPlane);
void RescaleCamera(PACamera *camera, uint32_t width, uint32_t height);
void TransformCamera(PACamera *camera);

struct PACE
{
	GLFWwindow *window;
	int running;

	PAScene *loadedScene;

	PACamera *currentCamera;
};

PACE* GetInstance();
PACE* InitPACE(const char *windowTitle, uint32_t width, uint32_t height, PACamera *camera);
void PACESetKeyCallback(void (*func)(int, int, int, int));
void PACESetMouseMovedCallback(void (*func)(double, double));
void PACESetWindowResizeCallback(void (*func)(int, int));
void PACE_hide_cursor();
void PACE_show_cursor();
void PollPACE();
void UpdateWindowContent();
void ClearPACE();

struct PAScene
{
	IPALight *lights;
	IPADraw *uis;
	IPADraw *meshes;
	size_t UICount;
	size_t UICapacity;
	size_t MeshCount;
	size_t MeshCapacity;
	size_t LightCount;
	size_t LightCapacity;
};

PAScene* CreateScene();
int AddLightToScene(PAScene *scene, IPALight light);
int AddMeshToScene(PAScene *scene, IPADraw mesh);
int AddUIToScene(PAScene *scene, IPADraw ui);
int RemoveMeshFromScene(PAScene *scene, int index, IPADraw mesh);
int RemoveUIFromScene(PAScene *scene, int index, IPADraw ui);
int RemoveLightFromScene(PAScene *scene, int index, IPALight light);
void PurgePAScene(PAScene *scene);

struct PAMesh
{
	uint32_t numVertices;

	PATransform transform;
	GLuint vao;
	PAShader *shader;
};

IPADraw newMesh(PAShader *shader, float *vertices, uint32_t numVertices);
void MeshDraw(void *raw_data, mat4x4 perspective);
float* CalculateNormals(float *vertices, uint32_t numVertices);
void PurgePAMesh(PAMesh *mesh);

typedef struct PAFont
{
	FT_Library fl;
	FT_Face ff;
	FT_GlyphSlot fg;

	struct Character
	{
		GLuint texture;
		float sx, sy;
		float bx, by;
		uint32_t advance;
	}*chars;
}PAFont;

PAFont* LoadFont(const char *path);

typedef struct PAText
{
	GLuint colorUniform;

	PAShader *shader;

	GLfloat color[3];
	GLuint vao;

	PAFont *font;
	int fontSize;

	const char *text;
	int x, y;
	int width, height;
}PAText;

IPADraw newText(int x, int y, const char *text, int fontSize, PAFont *font);
void TextDraw(void *raw_data, mat4x4 perspective);

struct PATexture
{
	GLuint textureID;

	int width;
	int height;
	int nrChannels;
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

struct PAAreaLight
{
	vec3 direction;

	vec3 ambientColor;
	vec3 lightColor;

	GLuint depthMapFBO;
	GLuint depthMap;

	uint8_t shadows;

	PATransform transform;
};

IPALight newAreaLight(vec3 direction, vec3 ambientLight, vec3 lightColor);
void AreaEnable(void *raw_data);
void AreaDisable(void *raw_data);

#endif
