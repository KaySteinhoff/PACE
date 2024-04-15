#include <PACE.h>

PACamera* CreateCamera(uint32_t width, uint32_t height, float nearPlane, float farPlane)
{
	PACamera *camera = malloc(sizeof(PACamera));

	if(!camera)
		return NULL;

	DEFAULT_TRANSFORM(defaultTransform);
	camera->transform = defaultTransform;
	mat4x4_identity(camera->identMatrix);
	mat4x4_identity(camera->transform.transformMatrix);
	mat4x4_identity(camera->projectionMatrix);
	mat4x4_identity(camera->orthoMatrix);

	mat4x4_perspective(camera->projectionMatrix, 3.141592653/4.0, width/(float)height, nearPlane, farPlane);
	mat4x4_ortho(camera->orthoMatrix, 0, width, height, 0, 1, -1);

	camera->nearPlane = nearPlane;
	camera->farPlane = farPlane;

	camera->viewMode = PAProjection;

	camera->forward[0] = 0;
	camera->forward[1] = 0;
	camera->forward[2] = 1;

	camera->up[0] = 0;
	camera->up[1] = 1;
	camera->up[2] = 0;

	camera->right[0] = 1;
	camera->right[1] = 0;
	camera->right[2] = 0;

	return camera;
}

void RescaleCamera(PACamera *camera, uint32_t width, uint32_t height)
{
	mat4x4_perspective(camera->projectionMatrix, 3.14159263/4.0, width/(float)height, camera->nearPlane, camera->farPlane);
	mat4x4_ortho(camera->orthoMatrix, 0, width, height, 0, 1, -1);
}

void TransformCamera(PACamera *camera)
{
	mat4x4_apply_transform(camera->transform.transformMatrix, camera->transform);
	camera->transform.transformMatrix[3][0] = -(camera->transform.px * camera->right[0] + camera->transform.py * camera->right[1] + camera->transform.pz * camera->right[2]);
	camera->transform.transformMatrix[3][1] = -(camera->transform.px * camera->up[0] + camera->transform.py * camera->up[1] + camera->transform.pz * camera->up[2]);
	camera->transform.transformMatrix[3][2] = -(camera->transform.px * camera->forward[0] + camera->transform.py * camera->forward[1] + camera->transform.pz * camera->forward[2]);
}