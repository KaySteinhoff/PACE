#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main()
{
	mat4 modView = view;
	modView[3][0] = 0;
	modView[3][1] = 0;
	modView[3][2] = 0;

	gl_Position = perspective * modView * model * vec4(aPos, 1.0);
	TexCoord = aTexCoord.xy;
}
