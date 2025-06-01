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
|Name|Utility|
|---|---|
|light|A pointer to the light that is to be initialized|
|direction|A float[3] providing the direction the light is facing|
|ambientLight|A float[3] containing the color that is to be rendered onto surfaces not illuminated by the light|
|lightColor|A float[3] containing the color that is to be render onto surfaces illuminated by the light|

__Description__<br>
Initializes the given PAAreaLight object to be ready for rendering the light.

__Example__<br>
__Currently not implemented__

### newLight

```C
IPALight newLight(PAAreaLight *light);
```

__Arguments__
|Name|Utility|
|---|---|
|light|A pointer to the object instance data|

__Description__
Creates a IPALight object ready to be used in lighting related tasks.

__Example__
```C
PAAreaLight areaLight = { 0 };
IPALight light = newLight(&areaLight);

AddLightToScene(&activeScene, light);
```

### AreaRender

```C
void AreaRender(void *raw_data);
```

__Arguments__
The <a href="IPALight.md">IPALight</a>->Render implementation for PAAreaLight.

__Description__
Renders the PAAreaLight.<br>
Normally not used by the end-user.<br>

__Example__<br>
__Currently not implemented__
