#include <PACE.h>

float verts[24] = {
	0, 0, 0, 0,
	0, 0, 0, 1,
	0, 0, 1, 1,

	0, 0, 0, 0,
	0, 0, 1, 1,
	0, 0, 1, 0
};


PAText* CreateText(int x, int y, const char *text, int fontSize, PAFont *font)
{
	PAText *textObj = malloc(sizeof(PAText));

	if(!textObj)
		return NULL;

	textObj->shader = CompileShader("#version 110\nattribute vec4 coord;varying vec2 texcoord;uniform mat4 perspective;void main(void){gl_Position = perspective* vec4(coord.xy, 0, 1);texcoord = coord.zw;}",
					"#version 110\nvarying vec2 texcoord;uniform sampler2D tex;uniform vec3 color;void main(){vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(tex, texcoord).r);gl_FragColor = vec4(color, 1.0) * sampled;}");
	if(!textObj->shader)
	{
		free(textObj);
		return NULL;
	}

	textObj->x = x;
	textObj->y = y;
	textObj->text = text;
	textObj->fontSize = fontSize;
	textObj->color[0] = 1;
	textObj->color[1] = 1;
	textObj->color[2] = 1;
	textObj->font = font;

	textObj->colorUniform = glGetUniformLocation(textObj->shader->ID, "color");

	glGenVertexArrays(1, &textObj->vao);

	glBindVertexArray(textObj->vao);
	glBindBuffer(GL_ARRAY_BUFFER, textObj->shader->vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)*4, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return textObj;
}

void SetTextColor(PAText *obj, GLfloat r, GLfloat g, GLfloat b)
{
	obj->color[0] = r;
	obj->color[1] = g;
	obj->color[2] = b;
}

void DrawText(PAText *obj)
{
	obj->width = 0;
	obj->height = 0;
	int tmpX = obj->x;

	glUseProgram(obj->shader->ID);
	glBindVertexArray(obj->vao);

	glClear(GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(obj->shader->perspectiveLocation, 1, GL_FALSE, (const GLfloat*)GetInstance()->currentCamera->uiMatrix);
	glUniform3f(obj->colorUniform, obj->color[0], obj->color[1], obj->color[2]);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(obj->vao);

	float pixelWidth = obj->fontSize * GetInstance()->dpiWidth / 72.0f;
	float pixelHeight = obj->fontSize * GetInstance()->dpiHeight / 72.0f;

	for(char *c = (char*)obj->text; *c != '\0'; ++c)
	{
		struct Character ch = obj->font->chars[(unsigned char)*c];

		float xpos = tmpX + (ch.bx/128.0)*pixelWidth;
		float ypos = obj->y - (ch.by/128.0)*pixelHeight;

		float w = (ch.sx/128.0)*pixelWidth;
		float h = (ch.sy/128.0)*pixelHeight;

		printf("%c: %d, %d\n", *c, w, h);

		//Triangle 1
		verts[0] = xpos;
		verts[1] = ypos;

		verts[4] = xpos;
		verts[5] = ypos + h;

		verts[8] = xpos + w;
		verts[9] = ypos + h;

		//Triangle 2
		verts[12] = xpos;
		verts[13] = ypos;

		verts[16] = xpos + w;
		verts[17] = ypos + h;

		verts[20] = xpos + w;
		verts[21] = ypos;

		glBindTexture(GL_TEXTURE_2D, ch.texture);
		glBindBuffer(GL_ARRAY_BUFFER, obj->shader->vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		tmpX += ((ch.advance>>6)/128.0)*pixelWidth;
		obj->width += ((ch.advance>>6)/128.0)*pixelWidth;

		if(ch.sy > obj->height)
			obj->height = (ch.sy/128.0)*pixelWidth;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
