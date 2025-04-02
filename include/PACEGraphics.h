
#ifndef PACE_GRAPHICS_H_
#define PACE_GRAPHICS_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <linmath.h>
#include <PACEInterfaces.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#define _USE_MATH_DEFINES

typedef enum
{
	PAProjection,
	PAOrtho
}PAViewMode;

typedef struct PACamera PACamera;
typedef struct PAMesh PAMesh;
typedef GLint PAShader;
typedef struct PAMaterial PAMaterial;
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

struct PACamera
{
	PATransform transform;

	PAViewMode viewMode;

	mat4x4 identMatrix;
	mat4x4 perspectiveMatrix;
	mat4x4 uiMatrix;
	mat4x4 orthoMatrix;

	float nearPlane;
	float farPlane;
};

unsigned int CreatePACamera(PACamera *camera, uint32_t width, uint32_t height, float nearPlane, float farPlane);
unsigned int RescaleCamera(PACamera *camera, uint32_t width, uint32_t height);
unsigned int TransformCamera(PACamera *camera);

struct PATexture
{
	GLuint textureID;

	int width;
	int height;
	int nrChannels;
};

unsigned int CreatePATexture(PATexture *texture, int width, int height, int nrChannels, GLint byteFormat, GLenum format, unsigned char *data);
unsigned int LoadTextureFromFile(PATexture *texture, const char *path, GLint byteFormat, GLenum format);

// PAShader (already fully defined above)
unsigned int LoadShaderFromSource(PAShader *partialShader, GLenum shaderType, char *path);
unsigned int CompileShader(PAShader *shader, GLenum shaderType, const char *shaderSource);

struct PAMaterial
{
	GLuint vbo;

	GLint ID;

	PATexture *texture;

	GLuint modelLocation;
	GLuint viewLocation;
	GLuint perspectiveLocation;
};

unsigned int CreatePAMaterial(PAMaterial *maerial, int n, ...);
unsigned int PAMaterialSetPATexture(PAMaterial *material, PATexture *texture);
unsigned int EnableMaterial(PAMaterial *material);
unsigned int SetInt(PAMaterial *material, const char *name, int value);
unsigned int SetFloat(PAMaterial *material, const char *name, float value);

struct PAScene
{
	IPALight *lights;
	size_t LightCount;
	size_t LightCapacity;

	IPADraw *uis;
	size_t UICount;
	size_t UICapacity;

	IPADraw *meshes;
	size_t MeshCount;
	size_t MeshCapacity;

	IPACollider *colliders;
	size_t ColliderCount;
	size_t ColliderCappacity;
};

unsigned int CreatePAScene(PAScene *scene);
unsigned int AddLightToScene(PAScene *scene, IPALight light);
unsigned int AddMeshToScene(PAScene *scene, IPADraw mesh);
unsigned int AddUIToScene(PAScene *scene, IPADraw ui);
unsigned int RemoveMeshFromScene(PAScene *scene, int index, IPADraw mesh);
unsigned int RemoveUIFromScene(PAScene *scene, int index, IPADraw ui);
unsigned int RemoveLightFromScene(PAScene *scene, int index, IPALight light);

struct PAMesh
{
	uint32_t numVertices;

	PATransform transform;
	GLuint vao;
	PAMaterial *shader;
};

unsigned int CreatePAMesh(PAMesh *mesh, PAMaterial *material, float *vertices, uint32_t numVertices);
IPADraw newMesh(PAMesh *mesh);
void MeshDraw(void *raw_data, mat4x4 perspective);
float* CalculateNormals(float *vertices, uint32_t numVertices);

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

unsigned int LoadFont(PAFont *font, const char *path);

typedef struct PAText
{
	GLuint colorUniform;

	PAMaterial shader;

	GLfloat color[3];
	GLuint vao;

	PAFont *font;
	int fontSize;

	const char *text;
	int x, y;
	int width, height;
}PAText;

unsigned int CreatePAText(PAText *text, int x, int y, const char *txt, int fontSize, PAFont *font);
IPADraw newText(PAText *text);
void TextDraw(void *raw_data, mat4x4 perspective);
void TextCalculateSize(void *raw_data);

struct PAAreaLight
{
	vec3 direction;

	vec3 ambientColor;
	vec3 lightColor;

	PAMaterial shader;

	GLuint depthMapFBO;
	GLuint depthMap;

	uint8_t shadows;

	PATransform transform;
};

unsigned int CreatePAAreaLight(PAAreaLight *light, vec3 direction, vec3 ambientLight, vec3 lightColor);
IPALight newAreaLight(PAAreaLight *light);
void AreaRender(void *raw_data);

#endif

