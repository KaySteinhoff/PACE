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

	float ratio = width/(float)height;
	mat4x4_perspective(camera->projectionMatrix, 3.141592653/4.0, ratio, nearPlane, farPlane);
	mat4x4_ortho(camera->uiMatrix, 0, width, height, 0, 1, -1);
	mat4x4_ortho(camera->orthoMatrix, -ratio, ratio, -1, 1, nearPlane, farPlane);

	camera->nearPlane = nearPlane;
	camera->farPlane = farPlane;

	camera->viewMode = PAProjection;

	return camera;
}

void RescaleCamera(PACamera *camera, uint32_t width, uint32_t height)
{
	float ratio = width/(float)height;
	mat4x4_perspective(camera->projectionMatrix, 3.14159263/4.0, ratio, camera->nearPlane, camera->farPlane);
	mat4x4_ortho(camera->uiMatrix, 0, width, height, 0, 1, -1);
	mat4x4_ortho(camera->orthoMatrix, -ratio, ratio, -1, 1, camera->nearPlane, camera->farPlane);
}

void TransformCamera(PACamera *camera)
{
	mat4x4_apply_transform(camera->transform.transformMatrix, &camera->transform);
	camera->transform.transformMatrix[3][0] = -(camera->transform.px * camera->transform.right[0] + camera->transform.py * camera->transform.right[1] + camera->transform.pz * camera->transform.right[2]);
	camera->transform.transformMatrix[3][1] = -(camera->transform.px * camera->transform.up[0] + camera->transform.py * camera->transform.up[1] + camera->transform.pz * camera->transform.up[2]);
	camera->transform.transformMatrix[3][2] = -(camera->transform.px * camera->transform.forward[0] + camera->transform.py * camera->transform.forward[1] + camera->transform.pz * camera->transform.forward[2]);
}
