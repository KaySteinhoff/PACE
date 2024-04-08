#include <cppPACE.h>
#include <linmath.h>

PATransform::PATransform() { }

PATransform::PATransform(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz)
{
	this->px = px;
	this->py = py;
	this->pz = pz;

	this->rx = rx;
	this->ry = ry;
	this->rz = rz;

	this->sx = sx;
	this->sy = sy;
	this->sz = sz;

	mat4x4_identity(transformMatrix);
}

void PATransform::ApplyTransform(mat4x4 M)
{
	mat4x4 rotation, scale;

	mat4x4_identity(M);//Used as translation matrix
	mat4x4_identity(rotation);
	mat4x4_identity(scale);

	mat4x4_rotate_X(rotation, rotation, rx);
	mat4x4_rotate_Y(rotation, rotation, ry);
	mat4x4_rotate_Z(rotation, rotation, rz);
	mat4x4_translate_in_place(M, px, py, pz);
	mat4x4_scale_aniso(scale, scale, sx, sy, sz);

	mat4x4_mul(M, M, rotation);
	mat4x4_mul(M, M, scale);
}
