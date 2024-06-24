#include <PACE.h>

IPALight newAreaLight(vec3 direction, vec3 ambientColor, vec3 lightColor)
{
	PAAreaLight *data = malloc(sizeof(PAAreaLight));
	if(!data)
		return (IPALight){ 0 };

	data->direction[0] = direction[0];
	data->direction[1] = direction[1];
	data->direction[2] = direction[2];

	data->ambientColor[0] = ambientColor[0];
	data->ambientColor[1] = ambientColor[1];
	data->ambientColor[2] = ambientColor[2];

	data->lightColor[0] = lightColor[0];
	data->lightColor[1] = lightColor[1];
	data->lightColor[2] = lightColor[2];

	data->shadows = 1;

	glGenFramebuffers(1, &data->depthMapFBO);

	glGenTextures(1, &data->depthMap);
	glBindTexture(GL_TEXTURE_2D, data->depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, data->depthMapFBO);//Set shadow framebuffer as current framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, data->depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);//Reset framebuffer to rendered scene

	data->transform.rx = 50;
	data->transform.ry = 30;

	return (IPALight){
		.typeTag = TYPE_TAG_AREA_LIGHT,
		.data = data
	};
}

void AreaEnable(void *raw_data)
{
	PAAreaLight *this = (PAAreaLight*)raw_data;

	mat4x4_apply_transform(this->transform.transformMatrix, &this->transform);

	glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void AreaDisable(void *raw_data)
{
	PAAreaLight *this = (PAAreaLight*)raw_data;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
