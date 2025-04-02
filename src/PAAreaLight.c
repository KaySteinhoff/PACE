#include <PACEErrorHandling.h>
#include <PACEGraphics.h>

int TYPE_TAG_PAAREA_LIGHT = -1;

unsigned int CreatePAAreaLight(PAAreaLight *light, vec3 direction, vec3 ambientColor, vec3 lightColor)
{
	light->direction[0] = direction[0];
	light->direction[1] = direction[1];
	light->direction[2] = direction[2];

	light->ambientColor[0] = ambientColor[0];
	light->ambientColor[1] = ambientColor[1];
	light->ambientColor[2] = ambientColor[2];

	light->lightColor[0] = lightColor[0];
	light->lightColor[1] = lightColor[1];
	light->lightColor[2] = lightColor[2];

	light->shadows = 1;
	unsigned int err = 0;
	PAShader vertex = 0, fragment = 0;
	if((err = CompileShader(&vertex, GL_VERTEX_SHADER, "#version 110\nattibute vec3 aPos;uniform mat4 view;uniform mat4 model;uniform mat4 perspective;void main(){ gl_Position = perspective * model * view * vec4(aPos, 1.0); }")))
		return err;
	if((err = CompileShader(&fragment, GL_FRAGMENT_SHADER, "#version 110\nvoid main(){ }")))
		return err;
	if((err = CreatePAMaterial(&light->shader, 2, vertex, fragment)))
		return err;

	glGenFramebuffers(1, &light->depthMapFBO);

	glGenTextures(1, &light->depthMap);
	glBindTexture(GL_TEXTURE_2D, light->depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, light->depthMapFBO);//Set shadow framebuffer as current framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, light->depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);//Reset framebuffer to rendered scene

	light->transform.rx = 50;
	light->transform.ry = 30;
	return err;
}

IPALight newAreaLight(PAAreaLight *light)
{
	if(!light)
		return (IPALight){ 0 };

	return (IPALight){
		.typeTag = TYPE_TAG_PAAREA_LIGHT,
		.data = light
	};
}

void AreaRender(void *raw_data)
{
	PAAreaLight *this = (PAAreaLight*)raw_data;

	mat4x4_apply_transform(this->transform.transformMatrix, &this->transform);

	glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
