# PAAreaLight

## Struct
```C
struct PAAreaLight
{
	vec3 direction;

	vec3 ambientColor;
	vec3 lightColor;

	PAMaterial shader;
	GLuint depthMapFBO;
	GLuint depthMap;

	unsigned char shadows;
	PATransform transform;
};
```

__Fields__
|Fieldname|Utility|
|---|---|
|direction|The direction the light is coming from|
|ambientColor|The ambient light(i.e. the light in the shadows)|
|lightColor|The light that casts on the rendered parts which are getting illuminated by the light source|
|<a href="PAMaterial.md">shader</a>|A PAMaterial instance serving as the light shader|
|depthMapFBO|The depth map framebuffer object|
|depthMap|The depth map index|
|shadows|A boolean whether to render shadows or not|
|<a href="PATransform.md">transform</a>|The transform of the light source|

## Functions

### CreatePAAreaLight

```C
unsigned int CreatePAAreaLight(PAAreaLight *light, vec3 direction, vec3 ambientLight, vec3 lightColor);
```

__Arguments__

__Description__

__Example__

### newLight

```C
IPALight newLight(PAAreaLight *light);
```

__Arguments__

__Description__

__Example__

### AreaRender

```C
void AreaRender(void *raw_data);
```

__Arguments__

__Description__

__Example__

