#include <PACE.h>

IPALight newDirectionalLight(vec3 direction, vec3 ambientColor, vec3 lightColor)
{
	PADirectionalLight *data = malloc(sizeof(PADirectionalLight));
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

	return (IPALight){
		.typeTag = TYPE_TAG_DIRECTIONAL_LIGHT,
		.data = data
	};
}

void DirectionalRender(void *raw_data)
{
	PADirectionalLight *this = (PADirectionalLight*)raw_data;
}

int DirectionalIsInShadow(void *raw_data, vec3 position)
{
	PADirectionalLight *this = (PADirectionalLight*)raw_data;

	return 1;
}
